#include "async_futures.h"

#include <stdlib.h>

// ---- Canonical component-model async built-ins (WASI Preview 3) -------------
//
// These are the same "$root" intrinsics wit-bindgen emits into its generated
// bindings; we re-declare them here so this module stands alone. The C symbol
// names are arbitrary — the (import_module, import_name) pair is what binds to
// the host built-in, and wasm-ld deduplicates identical imports across objects.

__attribute__((__import_module__("$root"), __import_name__("[subtask-drop]")))
extern void __af_subtask_drop(uint32_t handle);

__attribute__((__import_module__("$root"), __import_name__("[waitable-set-new]")))
extern uint32_t __af_waitable_set_new(void);

__attribute__((__import_module__("$root"), __import_name__("[waitable-join]")))
extern void __af_waitable_join(uint32_t waitable, uint32_t set);

__attribute__((__import_module__("$root"), __import_name__("[waitable-set-wait]")))
extern uint32_t __af_waitable_set_wait(uint32_t set, uint32_t* payload);

__attribute__((__import_module__("$root"), __import_name__("[waitable-set-poll]")))
extern uint32_t __af_waitable_set_poll(uint32_t set, uint32_t* payload);

// waitable-set.wait / .poll return the event kind and write [waitable, code]
// into the payload buffer. We only care about subtask-completion events.
#define AF_EVENT_NONE 0
#define AF_EVENT_SUBTASK 1

// ---- Registry --------------------------------------------------------------

typedef struct future_entry {
    async_future_t id;
    uint32_t subtask;                   // valid iff `started`
    bool started;                       // subtask live + joined to the set
    void (*on_done)(void* userdata);
    void* userdata;
} future_entry;

static future_entry** g_pending = NULL;
static size_t g_pending_len = 0;
static size_t g_pending_cap = 0;
static async_future_t g_next_id = 1;
static uint32_t g_set = 0;
static bool g_set_init = false;

static uint32_t waitable_set(void) {
    if (!g_set_init) {
        g_set = __af_waitable_set_new();
        g_set_init = true;
    }
    return g_set;
}

async_future_t async_register(uint32_t status, void (*on_done)(void* userdata), void* userdata) {
    future_entry* f = malloc(sizeof(future_entry));
    if (!f) abort();
    f->id = g_next_id++;
    f->on_done = on_done;
    f->userdata = userdata;
    if (ASYNC_SUBTASK_STATE(status) == ASYNC_SUBTASK_RETURNED) {
        f->started = false; // completed synchronously; ready to dispatch
    } else {
        f->started = true;
        f->subtask = ASYNC_SUBTASK_HANDLE(status);
        __af_waitable_join(f->subtask, waitable_set());
    }
    if (g_pending_len == g_pending_cap) {
        g_pending_cap = g_pending_cap ? g_pending_cap * 2 : 8;
        g_pending = realloc(g_pending, g_pending_cap * sizeof(*g_pending));
        if (!g_pending) abort();
    }
    g_pending[g_pending_len++] = f;
    return f->id;
}

bool async_future_pending(async_future_t id) {
    for (size_t i = 0; i < g_pending_len; i++) {
        if (g_pending[i]->id == id) return true;
    }
    return false;
}

static bool any_started(void) {
    for (size_t i = 0; i < g_pending_len; i++) {
        if (g_pending[i]->started) return true;
    }
    return false;
}

// Mark the subtask identified by `waitable` as returned so it becomes ready.
static void mark_returned(uint32_t waitable) {
    for (size_t i = 0; i < g_pending_len; i++) {
        if (g_pending[i]->started && g_pending[i]->subtask == waitable) {
            __af_waitable_join(g_pending[i]->subtask, 0); // remove from the set
            __af_subtask_drop(g_pending[i]->subtask);
            g_pending[i]->started = false;
            return;
        }
    }
}

// Fire the callback for a ready (started == false) future, then remove + free
// it. Removal happens before dispatch so the callback may re-enter this module.
// The userdata is owned by this module and freed once the callback has run.
static void complete_at(size_t i) {
    future_entry* f = g_pending[i];
    g_pending[i] = g_pending[g_pending_len - 1];
    g_pending_len--;
    f->on_done(f->userdata);
    free(f->userdata);
    free(f);
}

// Core engine: dispatch every future whose subtask has completed. When `block`
// is set and nothing is ready yet, wait for one live subtask first. Returns the
// number of futures dispatched.
static size_t drain(bool block) {
    size_t dispatched = 0;
    for (;;) {
        // Harvest all currently-available completion events (non-blocking).
        while (any_started()) {
            uint32_t payload[2] = {0, 0}; // [waitable, code]
            uint32_t kind = __af_waitable_set_poll(waitable_set(), payload);
            if (kind == AF_EVENT_NONE) break;
            if (kind == AF_EVENT_SUBTASK && ASYNC_SUBTASK_STATE(payload[1]) == ASYNC_SUBTASK_RETURNED) {
                mark_returned(payload[0]);
            }
        }

        // Fire callbacks for everything now ready.
        bool progressed = false;
        for (size_t i = 0; i < g_pending_len;) {
            if (!g_pending[i]->started) {
                complete_at(i); // swaps the last element into i, so don't advance
                dispatched++;
                progressed = true;
            } else {
                i++;
            }
        }

        // If nothing was ready this pass but we're allowed to block and a live
        // subtask could still complete, wait for one event and loop to harvest
        // + dispatch it. Otherwise we're done.
        if (block && !progressed && any_started()) {
            uint32_t payload[2] = {0, 0};
            uint32_t kind = __af_waitable_set_wait(waitable_set(), payload);
            if (kind == AF_EVENT_SUBTASK && ASYNC_SUBTASK_STATE(payload[1]) == ASYNC_SUBTASK_RETURNED) {
                mark_returned(payload[0]);
            }
            continue;
        }
        return dispatched;
    }
}

void async_dispatch_ready(void) {
    drain(false);
}

bool async_block_on_any(void) {
    return drain(true) > 0;
}

void async_block_on(async_future_t id) {
    while (async_future_pending(id)) {
        if (!async_block_on_any()) return; // no live subtask can make progress
    }
}

void async_block_on_all(void) {
    while (g_pending_len > 0) {
        if (!async_block_on_any()) {
            // Nothing left is waitable; dispatch any synchronously-ready leftovers.
            async_dispatch_ready();
            return;
        }
    }
}
