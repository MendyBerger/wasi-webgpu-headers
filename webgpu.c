#include "webgpu/webgpu.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>


#include <stdio.h>


#include "imports.h"

typedef struct WGPUAdapterImpl {
    wasi_webgpu_webgpu_own_gpu_adapter_t adapter;
    uint32_t refCount;
} WGPUAdapterImpl;
typedef struct WGPUBindGroupImpl {
    wasi_webgpu_webgpu_own_gpu_bind_group_t bind_group;
    uint32_t refCount;
} WGPUBindGroupImpl;
typedef struct WGPUBindGroupLayoutImpl {
    wasi_webgpu_webgpu_own_gpu_bind_group_layout_t bind_group_layout;
    uint32_t refCount;
} WGPUBindGroupLayoutImpl;
typedef struct WGPUBufferImpl {
    wasi_webgpu_webgpu_own_gpu_buffer_t buffer;
    uint32_t refCount;
    void* mapping;
    size_t offset;
    size_t size;
} WGPUBufferImpl;
typedef struct WGPUCommandBufferImpl {
    wasi_webgpu_webgpu_own_gpu_command_buffer_t command_buffer;
    uint32_t refCount;
} WGPUCommandBufferImpl;
typedef struct WGPUCommandEncoderImpl {
    wasi_webgpu_webgpu_own_gpu_command_encoder_t command_encoder;
    uint32_t refCount;
} WGPUCommandEncoderImpl;
typedef struct WGPUComputePassEncoderImpl {
    wasi_webgpu_webgpu_own_gpu_compute_pass_encoder_t compute_pass_encoder;
    uint32_t refCount;
} WGPUComputePassEncoderImpl;
typedef struct WGPUComputePipelineImpl {
    wasi_webgpu_webgpu_own_gpu_compute_pipeline_t compute_pipeline;
    uint32_t refCount;
} WGPUComputePipelineImpl;
typedef struct WGPUDeviceImpl {
    wasi_webgpu_webgpu_own_gpu_device_t device;
    uint32_t refCount;
} WGPUDeviceImpl;
typedef struct WGPUInstanceImpl {
    wasi_webgpu_webgpu_own_gpu_t gpu;
    uint32_t refCount;
} WGPUInstanceImpl;
// typedef struct WGPUPipelineLayoutImpl {
// } WGPUPipelineLayoutImpl;
// typedef struct WGPUQuerySetImpl {
// } WGPUQuerySetImpl;
typedef struct WGPUQueueImpl {
    wasi_webgpu_webgpu_own_gpu_queue_t queue;
    uint32_t refCount;
} WGPUQueueImpl;
// typedef struct WGPURenderBundleImpl {
// } WGPURenderBundleImpl;
// typedef struct WGPURenderBundleEncoderImpl {
// } WGPURenderBundleEncoderImpl;
// typedef struct WGPURenderPassEncoderImpl {
// } WGPURenderPassEncoderImpl;
// typedef struct WGPURenderPipelineImpl {
// } WGPURenderPipelineImpl;
// typedef struct WGPUSamplerImpl {
// } WGPUSamplerImpl;
typedef struct WGPUShaderModuleImpl {
    wasi_webgpu_webgpu_own_gpu_shader_module_t module;
    uint32_t refCount;
} WGPUShaderModuleImpl;
// typedef struct WGPUSurfaceImpl {
// } WGPUSurfaceImpl;
// typedef struct WGPUTextureImpl {
// } WGPUTextureImpl;
// typedef struct WGPUTextureViewImpl {
// } WGPUTextureViewImpl;

wasi_webgpu_webgpu_gpu_feature_name_t featureNativeToWasi(WGPUFeatureName const * feature);
WGPUFeatureName featureWasiToNative(wasi_webgpu_webgpu_gpu_feature_name_t const * feature);

WGPUInstance wgpuCreateInstance(WGPUInstanceDescriptor const* descriptor)
{
    WGPUInstanceImpl* instance = (WGPUInstanceImpl*)malloc(sizeof(WGPUInstanceImpl));
    instance->refCount = 1;
    instance->gpu = wasi_webgpu_webgpu_get_gpu();
    return instance;
}

WGPUStatus wgpuGetInstanceCapabilities(WGPUInstanceCapabilities* capabilities)
{
    abort();
}

WGPUProc wgpuGetProcAddress(WGPUStringView procName)
{
    abort();
}

void wgpuAdapterGetFeatures(WGPUAdapter adapter, WGPUSupportedFeatures* features)
{
    abort();
}

WGPUStatus wgpuAdapterGetInfo(WGPUAdapter adapter, WGPUAdapterInfo* info)
{
    abort();
}

WGPUStatus wgpuAdapterGetLimits(WGPUAdapter adapter, WGPULimits* limits)
{
    abort();
}

WGPUBool wgpuAdapterHasFeature(WGPUAdapter adapter, WGPUFeatureName feature)
{
    // abort();
    return false;
}

WGPUFuture wgpuAdapterRequestDevice(WGPUAdapter adapter, WGPUDeviceDescriptor const* descriptor, WGPURequestDeviceCallbackInfo callbackInfo)
{
    wasi_webgpu_webgpu_gpu_device_descriptor_t descriptor_impl = {};

    // This is stubbed out - there are a signficiant number of options yet to be supported

    if (descriptor->requiredLimits)
    {
        WGPULimits const* limits = descriptor->requiredLimits;

        descriptor_impl.required_limits.is_some = true;
        descriptor_impl.required_limits.val = wasi_webgpu_webgpu_constructor_record_option_gpu_size64();
        wasi_webgpu_webgpu_borrow_record_option_gpu_size64_t limits_ref =
            wasi_webgpu_webgpu_borrow_record_option_gpu_size64(descriptor_impl.required_limits.val);

        if (limits->maxBindGroups)
        {
            imports_string_t str = {};
            str.ptr = (uint8_t*)"maxBindGroups";
            str.len = strlen((char*)str.ptr);
            uint64_t maxBindGroups = limits->maxBindGroups;
            wasi_webgpu_webgpu_method_record_option_gpu_size64_add(limits_ref, &str, &maxBindGroups);
        }
        if (limits->maxBindGroupsPlusVertexBuffers)
        {
            imports_string_t str = {};
            str.ptr = (uint8_t*)"maxBindGroupsPlusVertexBuffers";
            str.len = strlen((char*)str.ptr);
            uint64_t maxBindGroupsPlusVertexBuffers = limits->maxBindGroupsPlusVertexBuffers;
            wasi_webgpu_webgpu_method_record_option_gpu_size64_add(limits_ref, &str, &maxBindGroupsPlusVertexBuffers);
        }
        // ...

        wasi_webgpu_webgpu_record_option_gpu_size64_drop_own(descriptor_impl.required_limits.val);
    }

    if (descriptor->requiredFeatures) // TODO: Not nullable so should we remove this check?
    {
        descriptor_impl.required_features.is_some = true;
        descriptor_impl.required_features.val = (wasi_webgpu_webgpu_list_gpu_feature_name_t){
            .ptr = malloc(descriptor->requiredFeatureCount * sizeof(wasi_webgpu_webgpu_gpu_feature_name_t)),
            .len = descriptor->requiredFeatureCount,
        };
        
        for (size_t i = 0; i < descriptor->requiredFeatureCount; i++)
        {
            descriptor_impl.required_features.val.ptr[i] = featureNativeToWasi(&descriptor->requiredFeatures[i]);
        }
    }

    wasi_webgpu_webgpu_own_gpu_device_t dev;
    wasi_webgpu_webgpu_request_device_error_t err;
    bool success = wasi_webgpu_webgpu_method_gpu_adapter_request_device(
        wasi_webgpu_webgpu_borrow_gpu_adapter(adapter->adapter),
        descriptor ? &descriptor_impl : NULL,
        &dev,
        &err
    );

    WGPUDeviceImpl * device = (WGPUDeviceImpl*)malloc(sizeof(WGPUDeviceImpl));
    device->refCount = 1;
    device->device = dev;

    WGPURequestDeviceCallback callback = callbackInfo.callback;
    callback(WGPURequestDeviceStatus_Success, device, WGPU_STRING_VIEW_INIT, callbackInfo.userdata1, callbackInfo.userdata2);
    return (WGPUFuture) { .id = 1 };
}

void wgpuAdapterAddRef(WGPUAdapter adapter)
{
    adapter->refCount ++;
}

void wgpuAdapterRelease(WGPUAdapter adapter)
{
    adapter->refCount --;
    if(adapter->refCount < 1)
    {
        free(adapter);
    }
}

void wgpuAdapterInfoFreeMembers(WGPUAdapterInfo adapterInfo)
{
    abort();
}

void wgpuBindGroupSetLabel(WGPUBindGroup bindGroup, WGPUStringView label)
{
    abort();
}

void wgpuBindGroupAddRef(WGPUBindGroup bindGroup)
{
    abort();
}

void wgpuBindGroupRelease(WGPUBindGroup bindGroup)
{
    bindGroup->refCount --;
    if(bindGroup->refCount < 1)
    {
        free(bindGroup);
    }
}

void wgpuBindGroupLayoutSetLabel(WGPUBindGroupLayout bindGroupLayout, WGPUStringView label)
{
    abort();
}

void wgpuBindGroupLayoutAddRef(WGPUBindGroupLayout bindGroupLayout)
{
    abort();
}

void wgpuBindGroupLayoutRelease(WGPUBindGroupLayout bindGroupLayout)
{
    bindGroupLayout->refCount --;
    if(bindGroupLayout->refCount < 1)
    {
        free(bindGroupLayout);
    }
}

void wgpuBufferDestroy(WGPUBuffer buffer)
{
    abort();
}

void const* wgpuBufferGetConstMappedRange(WGPUBuffer buffer, size_t offset, size_t size)
{
    return wgpuBufferGetMappedRange(buffer, offset, size);
}

WGPUBufferMapState wgpuBufferGetMapState(WGPUBuffer buffer)
{
    wasi_webgpu_webgpu_gpu_buffer_map_state_t state = wasi_webgpu_webgpu_method_gpu_buffer_map_state(wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer));
    switch (state) {
        case WASI_WEBGPU_WEBGPU_GPU_BUFFER_MAP_STATE_UNMAPPED:
            return WGPUBufferMapState_Unmapped;
        case WASI_WEBGPU_WEBGPU_GPU_BUFFER_MAP_STATE_PENDING:
            return WGPUBufferMapState_Pending;
        case WASI_WEBGPU_WEBGPU_GPU_BUFFER_MAP_STATE_MAPPED:
            return WGPUBufferMapState_Mapped;
        default:
            printf("unknown state: %d\n", state);
            abort();
    }
}

void* wgpuBufferGetMappedRange(WGPUBuffer buffer, size_t offset, size_t size)
{
    wasi_webgpu_webgpu_gpu_size64_t offset_val = (wasi_webgpu_webgpu_gpu_size64_t)offset;
    wasi_webgpu_webgpu_gpu_size64_t size_val = (wasi_webgpu_webgpu_gpu_size64_t)size;

    wasi_webgpu_webgpu_gpu_size64_t* offset_wasi = &offset_val;

    wasi_webgpu_webgpu_gpu_size64_t* size_wasi = NULL;
    if (size != WGPU_WHOLE_MAP_SIZE) {  // Or whatever condition makes sense
        size_wasi = &size_val;
    }

    imports_list_u8_t ret;
    wasi_webgpu_webgpu_get_mapped_range_error_t err;

    bool success = wasi_webgpu_webgpu_method_gpu_buffer_get_mapped_range_get_with_copy(
        wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
        offset_wasi,
        size_wasi,
        &ret,
        &err
    );

    assert(success);

    // TODO: do we need to malloc a new buffer?
    void* data = malloc(ret.len);
    memcpy(data, ret.ptr, ret.len);

    buffer->mapping = data;
    buffer->offset = offset;
    buffer->size = size;

    return (void*)data;
}

uint64_t wgpuBufferGetSize(WGPUBuffer buffer)
{
    return (uint64_t)wasi_webgpu_webgpu_method_gpu_buffer_size(wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer));
}

WGPUBufferUsage wgpuBufferGetUsage(WGPUBuffer buffer)
{
    wasi_webgpu_webgpu_gpu_flags_constant_t usage = wasi_webgpu_webgpu_method_gpu_buffer_usage(wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer));
    return usage;
}

WGPUFuture wgpuBufferMapAsync(WGPUBuffer buffer, WGPUMapMode mode, size_t offset, size_t size,
    WGPUBufferMapCallbackInfo callbackInfo)
{
    wasi_webgpu_webgpu_gpu_size64_t offset_val = (wasi_webgpu_webgpu_gpu_size64_t)offset;
    wasi_webgpu_webgpu_gpu_size64_t size_val = (wasi_webgpu_webgpu_gpu_size64_t)size;

    // If you want to pass NULL for 0 values, or if you want to check for invalid values:
    wasi_webgpu_webgpu_gpu_size64_t* offset_wasi = NULL;
    if (offset >= 0) {  // Or whatever condition makes sense
        offset_wasi = &offset_val;
    }

    wasi_webgpu_webgpu_gpu_size64_t* size_wasi = NULL;
    if (size != WGPU_WHOLE_MAP_SIZE) {  // Or whatever condition makes sense
        size_wasi = &size_val;
    }

    wasi_webgpu_webgpu_map_async_error_t err;

    wasi_webgpu_webgpu_method_gpu_buffer_map_async(
        wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
        mode,
        offset_wasi,
        size_wasi,
        &err
    );
    return (WGPUFuture) { .id = 1 };
}

WGPUStatus wgpuBufferReadMappedRange(WGPUBuffer buffer, size_t offset, void* data, size_t size)
{
    abort();
}

void wgpuBufferSetLabel(WGPUBuffer buffer, WGPUStringView label)
{
    abort();
}

void wgpuBufferUnmap(WGPUBuffer buffer)
{

    if (buffer->mapping != NULL) {

        wasi_webgpu_webgpu_gpu_size64_t offset_val = (wasi_webgpu_webgpu_gpu_size64_t)buffer->offset;
        wasi_webgpu_webgpu_gpu_size64_t size_val = (wasi_webgpu_webgpu_gpu_size64_t)buffer->size;

        wasi_webgpu_webgpu_gpu_size64_t* offset_wasi = &offset_val;

        wasi_webgpu_webgpu_gpu_size64_t* size_wasi = NULL;
        if (buffer->size != WGPU_WHOLE_MAP_SIZE) {
            size_wasi = &size_val;
        }

        size_t data_len;
        if(buffer->size == WGPU_WHOLE_MAP_SIZE) {
            data_len = wgpuBufferGetSize(buffer);
        } else {
            data_len = buffer->size;
        }
        imports_list_u8_t data = {
            .ptr = (uint8_t*)buffer->mapping,
            .len = data_len,
        };

        wasi_webgpu_webgpu_get_mapped_range_error_t err_set;
        wasi_webgpu_webgpu_gpu_size64_t buffer_size = (wasi_webgpu_webgpu_gpu_size64_t)wgpuBufferGetSize(buffer);
        wasi_webgpu_webgpu_gpu_size64_t offset_my = 0;
        bool success_set = wasi_webgpu_webgpu_method_gpu_buffer_get_mapped_range_set_with_copy(
            wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
            &data,
            offset_wasi,
            size_wasi,
            &err_set
        );
        assert(success_set);
        free(buffer->mapping);
        buffer->mapping = NULL;
    }

    wasi_webgpu_webgpu_unmap_error_t err_unmap;
    bool success_unmap = wasi_webgpu_webgpu_method_gpu_buffer_unmap(wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer), &err_unmap);
    assert(success_unmap);
}

WGPUStatus wgpuBufferWriteMappedRange(WGPUBuffer buffer, size_t offset, void const* data, size_t size)
{
    abort();
}

void wgpuBufferAddRef(WGPUBuffer buffer)
{
    buffer->refCount++;
}

void wgpuBufferRelease(WGPUBuffer buffer)
{
    buffer->refCount --;
    if(buffer->refCount < 1)
    {
        free(buffer);
    }
}

void wgpuCommandBufferSetLabel(WGPUCommandBuffer commandBuffer, WGPUStringView label)
{
    abort();
}

void wgpuCommandBufferAddRef(WGPUCommandBuffer commandBuffer)
{
    abort();
}

void wgpuCommandBufferRelease(WGPUCommandBuffer commandBuffer)
{
    commandBuffer->refCount --;
    if(commandBuffer->refCount < 1)
    {
        free(commandBuffer);
    }
}

WGPUComputePassEncoder wgpuCommandEncoderBeginComputePass(WGPUCommandEncoder commandEncoder,
    WGPUComputePassDescriptor const* descriptor)
{
    wasi_webgpu_webgpu_own_gpu_compute_pass_encoder_t compute_pass_encoder = wasi_webgpu_webgpu_method_gpu_command_encoder_begin_compute_pass(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(commandEncoder->command_encoder),
        &(wasi_webgpu_webgpu_gpu_compute_pass_descriptor_t) {
            .label = (imports_option_string_t) {
                .is_some = false,
            },
            .timestamp_writes = (wasi_webgpu_webgpu_option_gpu_compute_pass_timestamp_writes_t) {
                .is_some = false,
            },
        }
    );
    WGPUComputePassEncoderImpl* compute_pass_encoder_struct = (WGPUComputePassEncoderImpl*)malloc(sizeof(WGPUComputePassEncoderImpl));
    compute_pass_encoder_struct->compute_pass_encoder = compute_pass_encoder;
    compute_pass_encoder_struct->refCount ++;
    return compute_pass_encoder_struct;
}

WGPURenderPassEncoder wgpuCommandEncoderBeginRenderPass(WGPUCommandEncoder commandEncoder,
    WGPURenderPassDescriptor const* descriptor)
{
    abort();
}

void wgpuCommandEncoderClearBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer buffer, uint64_t offset, uint64_t size)
{
    abort();
}

void wgpuCommandEncoderCopyBufferToBuffer(WGPUCommandEncoder commandEncoder, WGPUBuffer source, uint64_t sourceOffset,
    WGPUBuffer destination, uint64_t destinationOffset, uint64_t size)
{
    wasi_webgpu_webgpu_method_gpu_command_encoder_copy_buffer_to_buffer(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(commandEncoder->command_encoder),
        wasi_webgpu_webgpu_borrow_gpu_buffer(source->buffer),
        sourceOffset,
        wasi_webgpu_webgpu_borrow_gpu_buffer(destination->buffer),
        destinationOffset,
        size
    );
}

void wgpuCommandEncoderCopyBufferToTexture(WGPUCommandEncoder commandEncoder, WGPUTexelCopyBufferInfo const* source,
    WGPUTexelCopyTextureInfo const* destination, WGPUExtent3D const* copySize)
{
    abort();
}

void wgpuCommandEncoderCopyTextureToBuffer(WGPUCommandEncoder commandEncoder, WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyBufferInfo const* destination, WGPUExtent3D const* copySize)
{
    abort();
}

void wgpuCommandEncoderCopyTextureToTexture(WGPUCommandEncoder commandEncoder, WGPUTexelCopyTextureInfo const* source,
    WGPUTexelCopyTextureInfo const* destination, WGPUExtent3D const* copySize)
{
    abort();
}

WGPUCommandBuffer wgpuCommandEncoderFinish(WGPUCommandEncoder commandEncoder,
    WGPUCommandBufferDescriptor const* descriptor)
{
    wasi_webgpu_webgpu_own_gpu_command_buffer_t command_buffer = wasi_webgpu_webgpu_method_gpu_command_encoder_finish(
        wasi_webgpu_webgpu_borrow_gpu_command_encoder(commandEncoder->command_encoder),
        &(wasi_webgpu_webgpu_gpu_command_buffer_descriptor_t) {
            .label = (imports_option_string_t) {
                .is_some = false,
            },
        }
    );

    WGPUCommandBufferImpl* command_buffer_struct = (WGPUCommandBufferImpl*)malloc(sizeof(WGPUCommandBufferImpl));
    command_buffer_struct->command_buffer = command_buffer;
    command_buffer_struct->refCount ++;
    return command_buffer_struct;
}

void wgpuCommandEncoderInsertDebugMarker(WGPUCommandEncoder commandEncoder, WGPUStringView markerLabel)
{
    abort();
}

void wgpuCommandEncoderPopDebugGroup(WGPUCommandEncoder commandEncoder)
{
    abort();
}

void wgpuCommandEncoderPushDebugGroup(WGPUCommandEncoder commandEncoder, WGPUStringView groupLabel)
{
    abort();
}

void wgpuCommandEncoderResolveQuerySet(WGPUCommandEncoder commandEncoder, WGPUQuerySet querySet, uint32_t firstQuery,
    uint32_t queryCount, WGPUBuffer destination, uint64_t destinationOffset)
{
    abort();
}

void wgpuCommandEncoderSetLabel(WGPUCommandEncoder commandEncoder, WGPUStringView label)
{
    abort();
}

void wgpuCommandEncoderWriteTimestamp(WGPUCommandEncoder commandEncoder, WGPUQuerySet querySet, uint32_t queryIndex)
{
    abort();
}

void wgpuCommandEncoderAddRef(WGPUCommandEncoder commandEncoder)
{
    abort();
}

void wgpuCommandEncoderRelease(WGPUCommandEncoder commandEncoder)
{
    commandEncoder->refCount --;
    if(commandEncoder->refCount < 1)
    {
        free(commandEncoder);
    }
}

void wgpuComputePassEncoderDispatchWorkgroups(WGPUComputePassEncoder computePassEncoder, uint32_t workgroupCountX,
    uint32_t workgroupCountY, uint32_t workgroupCountZ)
{
    wasi_webgpu_webgpu_method_gpu_compute_pass_encoder_dispatch_workgroups(
        wasi_webgpu_webgpu_borrow_gpu_compute_pass_encoder(computePassEncoder->compute_pass_encoder),
        workgroupCountX,
        &workgroupCountY,
        &workgroupCountZ
    );
}

void wgpuComputePassEncoderDispatchWorkgroupsIndirect(WGPUComputePassEncoder computePassEncoder,
    WGPUBuffer indirectBuffer, uint64_t indirectOffset)
{
    abort();
}

void wgpuComputePassEncoderEnd(WGPUComputePassEncoder computePassEncoder)
{
    wasi_webgpu_webgpu_method_gpu_compute_pass_encoder_end(
        wasi_webgpu_webgpu_borrow_gpu_compute_pass_encoder(computePassEncoder->compute_pass_encoder)
    );
}

void wgpuComputePassEncoderInsertDebugMarker(WGPUComputePassEncoder computePassEncoder, WGPUStringView markerLabel)
{
    abort();
}

void wgpuComputePassEncoderPopDebugGroup(WGPUComputePassEncoder computePassEncoder)
{
    abort();
}

void wgpuComputePassEncoderPushDebugGroup(WGPUComputePassEncoder computePassEncoder, WGPUStringView groupLabel)
{
    abort();
}

void wgpuComputePassEncoderSetBindGroup(WGPUComputePassEncoder computePassEncoder, uint32_t groupIndex,
    WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    wasi_webgpu_webgpu_set_bind_group_error_t err;

    wasi_webgpu_webgpu_borrow_gpu_bind_group_t bind_group = wasi_webgpu_webgpu_borrow_gpu_bind_group(group->bind_group);

    bool success = wasi_webgpu_webgpu_method_gpu_compute_pass_encoder_set_bind_group(
        wasi_webgpu_webgpu_borrow_gpu_compute_pass_encoder(computePassEncoder->compute_pass_encoder),
        groupIndex,
        &bind_group,
        NULL,
        NULL,
        NULL,
        &err
    );
    assert(success);
}

void wgpuComputePassEncoderSetLabel(WGPUComputePassEncoder computePassEncoder, WGPUStringView label)
{
    abort();
}

void wgpuComputePassEncoderSetPipeline(WGPUComputePassEncoder computePassEncoder, WGPUComputePipeline pipeline)
{
    wasi_webgpu_webgpu_method_gpu_compute_pass_encoder_set_pipeline(
        wasi_webgpu_webgpu_borrow_gpu_compute_pass_encoder(computePassEncoder->compute_pass_encoder),
        wasi_webgpu_webgpu_borrow_gpu_compute_pipeline(pipeline->compute_pipeline)
    );
}

void wgpuComputePassEncoderAddRef(WGPUComputePassEncoder computePassEncoder)
{
    abort();
}

void wgpuComputePassEncoderRelease(WGPUComputePassEncoder computePassEncoder)
{
    computePassEncoder->refCount --;
    if(computePassEncoder->refCount < 1)
    {
        free(computePassEncoder);
    }
}

WGPUBindGroupLayout wgpuComputePipelineGetBindGroupLayout(WGPUComputePipeline computePipeline, uint32_t groupIndex)
{
    wasi_webgpu_webgpu_own_gpu_bind_group_layout_t bind_group_layout = wasi_webgpu_webgpu_method_gpu_compute_pipeline_get_bind_group_layout(
        wasi_webgpu_webgpu_borrow_gpu_compute_pipeline(computePipeline->compute_pipeline),
        groupIndex
    );
    WGPUBindGroupLayoutImpl* bind_group_layout_struct = (WGPUBindGroupLayoutImpl*)malloc(sizeof(WGPUBindGroupLayoutImpl));
    bind_group_layout_struct->bind_group_layout = bind_group_layout;
    bind_group_layout_struct->refCount = 1;
    return (WGPUBindGroupLayout)bind_group_layout_struct;
}

void wgpuComputePipelineSetLabel(WGPUComputePipeline computePipeline, WGPUStringView label)
{
    abort();
}

void wgpuComputePipelineAddRef(WGPUComputePipeline computePipeline)
{
    computePipeline->refCount++;
}

void wgpuComputePipelineRelease(WGPUComputePipeline computePipeline)
{
    computePipeline->refCount --;
    if(computePipeline->refCount < 1)
    {
        free(computePipeline);
    }
}

WGPUBindGroup wgpuDeviceCreateBindGroup(WGPUDevice device, WGPUBindGroupDescriptor const* descriptor)
{
    size_t entries_array_size = descriptor->entryCount * sizeof(wasi_webgpu_webgpu_gpu_bind_group_entry_t);
    wasi_webgpu_webgpu_gpu_bind_group_entry_t* entries_array = malloc(entries_array_size);

    for (size_t i = 0; i < descriptor->entryCount; i++) {
        wasi_webgpu_webgpu_gpu_binding_resource_t resource = {};
        if (descriptor->entries[i].buffer != NULL) {
            resource.tag = WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_BUFFER_BINDING;
            resource.val.gpu_buffer_binding = (wasi_webgpu_webgpu_gpu_buffer_binding_t) {
                .buffer = wasi_webgpu_webgpu_borrow_gpu_buffer(descriptor->entries[i].buffer->buffer),
                .offset = (imports_option_gpu_size64_t) {
                    .is_some = false,
                    // .val = descriptor->entries[i].offset,
                },
                .size = (imports_option_gpu_size64_t) {
                    .is_some = false,
                },
            };
        } else if (descriptor->entries[i].sampler != NULL) {
            resource.tag = WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_SAMPLER;
            printf("-------------------------------------    sampler\n");
            abort();
        } else if (descriptor->entries[i].textureView != NULL) {
            resource.tag = WASI_WEBGPU_WEBGPU_GPU_BINDING_RESOURCE_GPU_TEXTURE_VIEW;
            printf("-------------------------------------    textureView\n");
            abort();
        } else {
            abort();
        }

        entries_array[i] = (wasi_webgpu_webgpu_gpu_bind_group_entry_t) {
            .binding = descriptor->entries[i].binding,
            .resource = resource,
        };
    }

    wasi_webgpu_webgpu_list_gpu_bind_group_entry_t entries = {
        .ptr = entries_array,
        .len = descriptor->entryCount,
    };

    wasi_webgpu_webgpu_own_gpu_bind_group_t bind_group = wasi_webgpu_webgpu_method_gpu_device_create_bind_group(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device),
        &(wasi_webgpu_webgpu_gpu_bind_group_descriptor_t) {
            .layout = wasi_webgpu_webgpu_borrow_gpu_bind_group_layout(descriptor->layout->bind_group_layout),
            .entries = entries,
            .label = (imports_option_string_t) {
                .is_some = false,
            },
        }
    );
    WGPUBindGroupImpl* bind_group_struct = (WGPUBindGroupImpl*)malloc(sizeof(WGPUBindGroupImpl));
    bind_group_struct->bind_group = bind_group;
    bind_group_struct->refCount = 1;
    return (WGPUBindGroup)bind_group_struct;
}

WGPUBindGroupLayout wgpuDeviceCreateBindGroupLayout(WGPUDevice device, WGPUBindGroupLayoutDescriptor const* descriptor)
{
    abort();
}

WGPUBuffer wgpuDeviceCreateBuffer(WGPUDevice device, WGPUBufferDescriptor const* descriptor)
{
    wasi_webgpu_webgpu_own_gpu_buffer_t buffer = wasi_webgpu_webgpu_method_gpu_device_create_buffer(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device),
        &(wasi_webgpu_webgpu_gpu_buffer_descriptor_t) {
            .size = descriptor->size,
            .usage = descriptor->usage,
            .mapped_at_creation = (imports_option_bool_t) {
                .is_some = true,
                .val = descriptor->mappedAtCreation,
            },
            .label = {
                .is_some = false,
            },
        }
    );
    WGPUBufferImpl* buffer_struct = (WGPUBufferImpl*)malloc(sizeof(WGPUBufferImpl));
    buffer_struct->buffer = buffer;
    buffer_struct->refCount = 1;
    return (WGPUBuffer)buffer_struct;
}

WGPUCommandEncoder wgpuDeviceCreateCommandEncoder(WGPUDevice device, WGPUCommandEncoderDescriptor const* descriptor)
{
    wasi_webgpu_webgpu_own_gpu_command_encoder_t command_encoder = wasi_webgpu_webgpu_method_gpu_device_create_command_encoder(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device),
        &(wasi_webgpu_webgpu_gpu_command_encoder_descriptor_t) {
            .label = (imports_option_string_t) {
                .is_some = false,
            },
        }
    );

    WGPUCommandEncoderImpl* command_encoder_struct = (WGPUCommandEncoderImpl*)malloc(sizeof(WGPUCommandEncoderImpl));
    command_encoder_struct->command_encoder = command_encoder;
    command_encoder_struct->refCount ++;
    return command_encoder_struct;
}

WGPUComputePipeline wgpuDeviceCreateComputePipeline(WGPUDevice device, WGPUComputePipelineDescriptor const* descriptor)
{
    imports_option_string_t entry_point;
    entry_point.is_some = descriptor->compute.entryPoint.data != NULL;
    if (entry_point.is_some) {
        entry_point.val = (imports_string_t) {
            .ptr = (uint8_t*)descriptor->compute.entryPoint.data,
            .len = strlen(descriptor->compute.entryPoint.data)
        };
    };

    uint8_t layout_tag = WASI_WEBGPU_WEBGPU_GPU_LAYOUT_MODE_AUTO;
    if (descriptor->layout != NULL) {
        layout_tag = WASI_WEBGPU_WEBGPU_GPU_LAYOUT_MODE_SPECIFIC;
        // TODO:
        abort();
    }

    wasi_webgpu_webgpu_own_gpu_compute_pipeline_t compute_pipeline = wasi_webgpu_webgpu_method_gpu_device_create_compute_pipeline(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device),
        &(wasi_webgpu_webgpu_gpu_compute_pipeline_descriptor_t) {
            .compute = (wasi_webgpu_webgpu_gpu_programmable_stage_t) {
                .module = wasi_webgpu_webgpu_borrow_gpu_shader_module(descriptor->compute.module->module),
                .entry_point = entry_point,
                .constants = (wasi_webgpu_webgpu_option_own_record_gpu_pipeline_constant_value_t) {
                    .is_some = false,
                },
            },
            .layout = (wasi_webgpu_webgpu_gpu_layout_mode_t) {
                .tag = layout_tag,
            },
            .label = (imports_option_string_t) {
                .is_some = false,
            },
        }
    );
    WGPUComputePipelineImpl* compute_pipeline_struct = (WGPUComputePipelineImpl*)malloc(sizeof(WGPUComputePipelineImpl));
    compute_pipeline_struct->compute_pipeline = compute_pipeline;
    compute_pipeline_struct->refCount = 1;
    return (WGPUComputePipeline)compute_pipeline_struct;
}

WGPUFuture wgpuDeviceCreateComputePipelineAsync(WGPUDevice device, WGPUComputePipelineDescriptor const* descriptor,
    WGPUCreateComputePipelineAsyncCallbackInfo callbackInfo)
{
    abort();
}

WGPUPipelineLayout wgpuDeviceCreatePipelineLayout(WGPUDevice device, WGPUPipelineLayoutDescriptor const* descriptor)
{
    abort();
}

WGPUQuerySet wgpuDeviceCreateQuerySet(WGPUDevice device, WGPUQuerySetDescriptor const* descriptor)
{
    abort();
}

WGPURenderBundleEncoder wgpuDeviceCreateRenderBundleEncoder(WGPUDevice device,
    WGPURenderBundleEncoderDescriptor const* descriptor)
{
    abort();
}

WGPURenderPipeline wgpuDeviceCreateRenderPipeline(WGPUDevice device, WGPURenderPipelineDescriptor const* descriptor)
{
    abort();
}

WGPUFuture wgpuDeviceCreateRenderPipelineAsync(WGPUDevice device, WGPURenderPipelineDescriptor const* descriptor,
    WGPUCreateRenderPipelineAsyncCallbackInfo callbackInfo)
{
    abort();
}

WGPUSampler wgpuDeviceCreateSampler(WGPUDevice device, WGPUSamplerDescriptor const* descriptor)
{
    abort();
}

WGPUShaderModule wgpuDeviceCreateShaderModule(WGPUDevice device, WGPUShaderModuleDescriptor const* descriptor)
{
    WGPUShaderSourceWGSL* wgsl_source = NULL;
    switch (descriptor->nextInChain->sType) {
        case WGPUSType_ShaderSourceWGSL:
            wgsl_source = (WGPUShaderSourceWGSL*)descriptor->nextInChain;
            break;
        default:
            // TODO:
            printf("default case\n");
            abort();
            // TODO: handle default case
            break;
    }

    imports_string_t code = {
        .ptr = (uint8_t*)wgsl_source->code.data,
        // .len = wgsl_source->code.length
        .len = strlen(wgsl_source->code.data)
    };

    wasi_webgpu_webgpu_gpu_shader_module_descriptor_t descriptor_wasi = {
        .code = code,
        // TODO: fill in compilation hints
        .compilation_hints = (wasi_webgpu_webgpu_option_list_gpu_shader_module_compilation_hint_t){
            .is_some = false,
        },
        // .label = descriptor->label,
        .label = (imports_option_string_t) {
            .is_some = false,
        },
    };

    wasi_webgpu_webgpu_own_gpu_shader_module_t module = wasi_webgpu_webgpu_method_gpu_device_create_shader_module(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device),
        &descriptor_wasi
    );
    WGPUShaderModuleImpl* shader_module = (WGPUShaderModuleImpl*)malloc(sizeof(WGPUShaderModuleImpl));
    shader_module->module = module;
    shader_module->refCount = 1;
    return shader_module;
}

WGPUTexture wgpuDeviceCreateTexture(WGPUDevice device, WGPUTextureDescriptor const* descriptor)
{
    abort();
}

void wgpuDeviceDestroy(WGPUDevice device)
{
    abort();
}

WGPUStatus wgpuDeviceGetAdapterInfo(WGPUDevice device, WGPUAdapterInfo* adapterInfo)
{
    abort();
}

void wgpuDeviceGetFeatures(WGPUDevice device, WGPUSupportedFeatures* features)
{
    abort();
}

WGPUStatus wgpuDeviceGetLimits(WGPUDevice device, WGPULimits* limits)
{
    abort();
}

WGPUFuture wgpuDeviceGetLostFuture(WGPUDevice device)
{
    abort();
}

WGPUQueue wgpuDeviceGetQueue(WGPUDevice device)
{
    wasi_webgpu_webgpu_own_gpu_queue_t queue = wasi_webgpu_webgpu_method_gpu_device_queue(
        wasi_webgpu_webgpu_borrow_gpu_device(device->device)
    );

    WGPUQueueImpl* queue_struct = (WGPUQueueImpl*)malloc(sizeof(WGPUQueueImpl));
    queue_struct->queue = queue;
    return queue_struct;
}

WGPUBool wgpuDeviceHasFeature(WGPUDevice device, WGPUFeatureName feature)
{
    // wasi_webgpu_webgpu_gpu_feature_name_t requested_feature = featureNativeToWasi(&feature);
    // wasi_webgpu_webgpu_own_gpu_supported_features_t available_features = wasi_webgpu_webgpu_method_gpu_device_features(
    //     wasi_webgpu_webgpu_borrow_gpu_device(device->device)
    // );
    // return wasi_webgpu_webgpu_method_gpu_supported_features_has(
    //     wasi_webgpu_webgpu_borrow_gpu_supported_features(available_features),
    //     featureNativeToWasi(&feature)
    // );
    if (feature == WGPUFeatureName_ShaderF16) {
        return true;
    }
    return false;
}

WGPUFuture wgpuDevicePopErrorScope(WGPUDevice device, WGPUPopErrorScopeCallbackInfo callbackInfo)
{
    // abort();
    return (WGPUFuture) { .id = 1 };
}

void wgpuDevicePushErrorScope(WGPUDevice device, WGPUErrorFilter filter)
{
    // abort();
}

void wgpuDeviceSetLabel(WGPUDevice device, WGPUStringView label)
{
    abort();
}

void wgpuDeviceAddRef(WGPUDevice device)
{
    device->refCount ++;
}

void wgpuDeviceRelease(WGPUDevice device)
{
    device->refCount --;
    if(device->refCount < 1)
    {
        free(device);
    }
}

WGPUSurface wgpuInstanceCreateSurface(WGPUInstance instance, WGPUSurfaceDescriptor const* descriptor)
{
    abort();
}

WGPUStatus wgpuInstanceGetWGSLLanguageFeatures(WGPUInstance instance, WGPUSupportedWGSLLanguageFeatures* features)
{
    abort();
}

WGPUBool wgpuInstanceHasWGSLLanguageFeature(WGPUInstance instance, WGPUWGSLLanguageFeatureName feature)
{
    abort();
}

void wgpuInstanceProcessEvents(WGPUInstance instance)
{
    abort();
}

WGPUFuture wgpuInstanceRequestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const* options,
    WGPURequestAdapterCallbackInfo callbackInfo)
{
    wasi_webgpu_webgpu_gpu_request_adapter_options_t wasi_options = {};
    wasi_webgpu_webgpu_own_gpu_adapter_t wasi_adapter;

    bool success = wasi_webgpu_webgpu_method_gpu_request_adapter(
        wasi_webgpu_webgpu_borrow_gpu(instance->gpu),
        &wasi_options,
        &wasi_adapter
    );

    WGPUAdapterImpl * adapter = (WGPUAdapterImpl*)malloc(sizeof(WGPUAdapterImpl));
    adapter->refCount = 1;
    adapter->adapter = wasi_adapter;

    callbackInfo.callback(WGPURequestAdapterStatus_Success, adapter, WGPU_STRING_VIEW_INIT, callbackInfo.userdata1, callbackInfo.userdata2);
    return (WGPUFuture) { .id = 1 };
}

WGPUWaitStatus wgpuInstanceWaitAny(WGPUInstance instance, size_t futureCount, WGPUFutureWaitInfo* futures,
    uint64_t timeoutNS)
{
    // abort();
    return WGPUWaitStatus_Success;
}

void wgpuInstanceAddRef(WGPUInstance instance)
{
    instance->refCount ++;
}

void wgpuInstanceRelease(WGPUInstance instance)
{
    instance->refCount --;
    if(instance->refCount < 1)
    {
        free(instance);
    }
}

void wgpuPipelineLayoutSetLabel(WGPUPipelineLayout pipelineLayout, WGPUStringView label)
{
    abort();
}

void wgpuPipelineLayoutAddRef(WGPUPipelineLayout pipelineLayout)
{
    abort();
}

void wgpuPipelineLayoutRelease(WGPUPipelineLayout pipelineLayout)
{
    abort();
}

void wgpuQuerySetDestroy(WGPUQuerySet querySet)
{
    abort();
}

uint32_t wgpuQuerySetGetCount(WGPUQuerySet querySet)
{
    abort();
}

WGPUQueryType wgpuQuerySetGetType(WGPUQuerySet querySet)
{
    abort();
}

void wgpuQuerySetSetLabel(WGPUQuerySet querySet, WGPUStringView label)
{
    abort();
}

void wgpuQuerySetAddRef(WGPUQuerySet querySet)
{
    abort();
}

void wgpuQuerySetRelease(WGPUQuerySet querySet)
{
    abort();
}

WGPUFuture wgpuQueueOnSubmittedWorkDone(WGPUQueue queue, WGPUQueueWorkDoneCallbackInfo callbackInfo)
{
    abort();
}

void wgpuQueueSetLabel(WGPUQueue queue, WGPUStringView label)
{
    abort();
}

void wgpuQueueSubmit(WGPUQueue queue, size_t commandCount, WGPUCommandBuffer const* commands)
{
    wasi_webgpu_webgpu_borrow_gpu_command_buffer_t wasi_commands[commandCount];
    for (size_t i = 0; i < commandCount; i++) {
        wasi_commands[i] = wasi_webgpu_webgpu_borrow_gpu_command_buffer(commands[i]->command_buffer);
    }
    wasi_webgpu_webgpu_list_borrow_gpu_command_buffer_t command_buffers = {
        .ptr = wasi_commands,
        .len = commandCount
    };
    wasi_webgpu_webgpu_method_gpu_queue_submit(
        wasi_webgpu_webgpu_borrow_gpu_queue(queue->queue),
        &command_buffers
    );
}

void wgpuQueueWriteBuffer(WGPUQueue queue, WGPUBuffer buffer, uint64_t bufferOffset, void const* data, size_t size)
{
    imports_list_u8_t data_list = {
        .ptr = (uint8_t*)data,
        .len = size,
    };

    wasi_webgpu_webgpu_write_buffer_error_t err;
    bool success = wasi_webgpu_webgpu_method_gpu_queue_write_buffer_with_copy(
        wasi_webgpu_webgpu_borrow_gpu_queue(queue->queue),
        wasi_webgpu_webgpu_borrow_gpu_buffer(buffer->buffer),
        bufferOffset,
        &data_list,
        NULL,
        NULL,
        &err
    );
    assert(success);
}

void wgpuQueueWriteTexture(WGPUQueue queue, WGPUTexelCopyTextureInfo const* destination, void const* data, size_t dataSize,
    WGPUTexelCopyBufferLayout const* dataLayout, WGPUExtent3D const* writeSize)
{
    abort();
}

void wgpuQueueAddRef(WGPUQueue queue)
{
    abort();
}

void wgpuQueueRelease(WGPUQueue queue)
{
    abort();
}

void wgpuRenderBundleSetLabel(WGPURenderBundle renderBundle, WGPUStringView label)
{
    abort();
}

void wgpuRenderBundleAddRef(WGPURenderBundle renderBundle)
{
    abort();
}

void wgpuRenderBundleRelease(WGPURenderBundle renderBundle)
{
    abort();
}

void wgpuRenderBundleEncoderDraw(WGPURenderBundleEncoder renderBundleEncoder, uint32_t vertexCount,
    uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance)
{
    abort();
}

void wgpuRenderBundleEncoderDrawIndexed(WGPURenderBundleEncoder renderBundleEncoder, uint32_t indexCount,
    uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance)
{
    abort();
}

void wgpuRenderBundleEncoderDrawIndexedIndirect(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer indirectBuffer,
    uint64_t indirectOffset)
{
    abort();
}

void wgpuRenderBundleEncoderDrawIndirect(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer indirectBuffer,
    uint64_t indirectOffset)
{
    abort();
}

WGPURenderBundle wgpuRenderBundleEncoderFinish(WGPURenderBundleEncoder renderBundleEncoder,
    WGPURenderBundleDescriptor const* descriptor)
{
    abort();
}

void wgpuRenderBundleEncoderInsertDebugMarker(WGPURenderBundleEncoder renderBundleEncoder, WGPUStringView markerLabel)
{
    abort();
}

void wgpuRenderBundleEncoderPopDebugGroup(WGPURenderBundleEncoder renderBundleEncoder)
{
    abort();
}

void wgpuRenderBundleEncoderPushDebugGroup(WGPURenderBundleEncoder renderBundleEncoder, WGPUStringView groupLabel)
{
    abort();
}

void wgpuRenderBundleEncoderSetBindGroup(WGPURenderBundleEncoder renderBundleEncoder, uint32_t groupIndex,
    WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    abort();
}

void wgpuRenderBundleEncoderSetIndexBuffer(WGPURenderBundleEncoder renderBundleEncoder, WGPUBuffer buffer,
    WGPUIndexFormat format, uint64_t offset, uint64_t size)
{
    abort();
}

void wgpuRenderBundleEncoderSetLabel(WGPURenderBundleEncoder renderBundleEncoder, WGPUStringView label)
{
    abort();
}

void wgpuRenderBundleEncoderSetPipeline(WGPURenderBundleEncoder renderBundleEncoder, WGPURenderPipeline pipeline)
{
    abort();
}

void wgpuRenderBundleEncoderSetVertexBuffer(WGPURenderBundleEncoder renderBundleEncoder, uint32_t slot,
    WGPUBuffer buffer, uint64_t offset, uint64_t size)
{
    abort();
}

void wgpuRenderBundleEncoderAddRef(WGPURenderBundleEncoder renderBundleEncoder)
{
    abort();
}

void wgpuRenderBundleEncoderRelease(WGPURenderBundleEncoder renderBundleEncoder)
{
    abort();
}

void wgpuRenderPassEncoderBeginOcclusionQuery(WGPURenderPassEncoder renderPassEncoder, uint32_t queryIndex)
{
    abort();
}

void wgpuRenderPassEncoderDraw(WGPURenderPassEncoder renderPassEncoder, uint32_t vertexCount, uint32_t instanceCount,
    uint32_t firstVertex, uint32_t firstInstance)
{
    abort();
}

void wgpuRenderPassEncoderDrawIndexed(WGPURenderPassEncoder renderPassEncoder, uint32_t indexCount,
    uint32_t instanceCount, uint32_t firstIndex, int32_t baseVertex, uint32_t firstInstance)
{
    abort();
}

void wgpuRenderPassEncoderDrawIndexedIndirect(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer indirectBuffer,
    uint64_t indirectOffset)
{
    abort();
}

void wgpuRenderPassEncoderDrawIndirect(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer indirectBuffer,
    uint64_t indirectOffset)
{
    abort();
}

void wgpuRenderPassEncoderEnd(WGPURenderPassEncoder renderPassEncoder)
{
    abort();
}

void wgpuRenderPassEncoderEndOcclusionQuery(WGPURenderPassEncoder renderPassEncoder)
{
    abort();
}

void wgpuRenderPassEncoderExecuteBundles(WGPURenderPassEncoder renderPassEncoder, size_t bundleCount,
    WGPURenderBundle const* bundles)
{
    abort();
}

void wgpuRenderPassEncoderInsertDebugMarker(WGPURenderPassEncoder renderPassEncoder, WGPUStringView markerLabel)
{
    abort();
}

void wgpuRenderPassEncoderPopDebugGroup(WGPURenderPassEncoder renderPassEncoder)
{
    abort();
}

void wgpuRenderPassEncoderPushDebugGroup(WGPURenderPassEncoder renderPassEncoder, WGPUStringView groupLabel)
{
    abort();
}

void wgpuRenderPassEncoderSetBindGroup(WGPURenderPassEncoder renderPassEncoder, uint32_t groupIndex,
    WGPUBindGroup group, size_t dynamicOffsetCount, uint32_t const* dynamicOffsets)
{
    abort();
}

void wgpuRenderPassEncoderSetBlendConstant(WGPURenderPassEncoder renderPassEncoder, WGPUColor const* color)
{
    abort();
}

void wgpuRenderPassEncoderSetIndexBuffer(WGPURenderPassEncoder renderPassEncoder, WGPUBuffer buffer,
    WGPUIndexFormat format, uint64_t offset, uint64_t size)
{
    abort();
}

void wgpuRenderPassEncoderSetLabel(WGPURenderPassEncoder renderPassEncoder, WGPUStringView label)
{
    abort();
}

void wgpuRenderPassEncoderSetPipeline(WGPURenderPassEncoder renderPassEncoder, WGPURenderPipeline pipeline)
{
    abort();
}

void wgpuRenderPassEncoderSetScissorRect(WGPURenderPassEncoder renderPassEncoder, uint32_t x, uint32_t y,
    uint32_t width, uint32_t height)
{
    abort();
}

void wgpuRenderPassEncoderSetStencilReference(WGPURenderPassEncoder renderPassEncoder, uint32_t reference)
{
    abort();
}

void wgpuRenderPassEncoderSetVertexBuffer(WGPURenderPassEncoder renderPassEncoder, uint32_t slot, WGPUBuffer buffer,
    uint64_t offset, uint64_t size)
{
    abort();
}

void wgpuRenderPassEncoderSetViewport(WGPURenderPassEncoder renderPassEncoder, float x, float y, float width,
    float height, float minDepth, float maxDepth)
{
    abort();
}

void wgpuRenderPassEncoderAddRef(WGPURenderPassEncoder renderPassEncoder)
{
    abort();
}

void wgpuRenderPassEncoderRelease(WGPURenderPassEncoder renderPassEncoder)
{
    abort();
}

WGPUBindGroupLayout wgpuRenderPipelineGetBindGroupLayout(WGPURenderPipeline renderPipeline, uint32_t groupIndex)
{
    abort();
}

void wgpuRenderPipelineSetLabel(WGPURenderPipeline renderPipeline, WGPUStringView label)
{
    abort();
}

void wgpuRenderPipelineAddRef(WGPURenderPipeline renderPipeline)
{
    abort();
}

void wgpuRenderPipelineRelease(WGPURenderPipeline renderPipeline)
{
    abort();
}

void wgpuSamplerSetLabel(WGPUSampler sampler, WGPUStringView label)
{
    abort();
}

void wgpuSamplerAddRef(WGPUSampler sampler)
{
    abort();
}

void wgpuSamplerRelease(WGPUSampler sampler)
{
    abort();
}

WGPUFuture wgpuShaderModuleGetCompilationInfo(WGPUShaderModule shaderModule,
    WGPUCompilationInfoCallbackInfo callbackInfo)
{
    abort();
}

void wgpuShaderModuleSetLabel(WGPUShaderModule shaderModule, WGPUStringView label)
{
    abort();
}

void wgpuShaderModuleAddRef(WGPUShaderModule shaderModule)
{
    shaderModule->refCount++;
}

void wgpuShaderModuleRelease(WGPUShaderModule shaderModule)
{
    shaderModule->refCount --;
    if(shaderModule->refCount < 1)
    {
        free(shaderModule);
    }
}

void wgpuSupportedFeaturesFreeMembers(WGPUSupportedFeatures supportedFeatures)
{
    abort();
}

void wgpuSupportedWGSLLanguageFeaturesFreeMembers(WGPUSupportedWGSLLanguageFeatures supportedWGSLLanguageFeatures)
{
    abort();
}

void wgpuSurfaceConfigure(WGPUSurface surface, WGPUSurfaceConfiguration const* config)
{
    abort();
}

WGPUStatus wgpuSurfaceGetCapabilities(WGPUSurface surface, WGPUAdapter adapter, WGPUSurfaceCapabilities* capabilities)
{
    abort();
}

void wgpuSurfaceGetCurrentTexture(WGPUSurface surface, WGPUSurfaceTexture* surfaceTexture)
{
    abort();
}

WGPUStatus wgpuSurfacePresent(WGPUSurface surface)
{
    abort();
}

void wgpuSurfaceSetLabel(WGPUSurface surface, WGPUStringView label)
{
    abort();
}

void wgpuSurfaceUnconfigure(WGPUSurface surface)
{
    abort();
}

void wgpuSurfaceAddRef(WGPUSurface surface)
{
    abort();
}

void wgpuSurfaceRelease(WGPUSurface surface)
{
    abort();
}

void wgpuSurfaceCapabilitiesFreeMembers(WGPUSurfaceCapabilities surfaceCapabilities)
{
    abort();
}

WGPUTextureView wgpuTextureCreateView(WGPUTexture texture, WGPUTextureViewDescriptor const* descriptor)
{
    abort();
}

void wgpuTextureDestroy(WGPUTexture texture)
{
    abort();
}

uint32_t wgpuTextureGetDepthOrArrayLayers(WGPUTexture texture)
{
    abort();
}

WGPUTextureDimension wgpuTextureGetDimension(WGPUTexture texture)
{
    abort();
}

WGPUTextureFormat wgpuTextureGetFormat(WGPUTexture texture)
{
    abort();
}

uint32_t wgpuTextureGetHeight(WGPUTexture texture)
{
    abort();
}

uint32_t wgpuTextureGetMipLevelCount(WGPUTexture texture)
{
    abort();
}

uint32_t wgpuTextureGetSampleCount(WGPUTexture texture)
{
    abort();
}

WGPUTextureUsage wgpuTextureGetUsage(WGPUTexture texture)
{
    abort();
}

uint32_t wgpuTextureGetWidth(WGPUTexture texture)
{
    abort();
}

void wgpuTextureSetLabel(WGPUTexture texture, WGPUStringView label)
{
    abort();
}

void wgpuTextureAddRef(WGPUTexture texture)
{
    abort();
}

void wgpuTextureRelease(WGPUTexture texture)
{
    abort();
}

void wgpuTextureViewSetLabel(WGPUTextureView textureView, WGPUStringView label)
{
    abort();
}

void wgpuTextureViewAddRef(WGPUTextureView textureView)
{
    abort();
}

void wgpuTextureViewRelease(WGPUTextureView textureView)
{
    abort();
}

wasi_webgpu_webgpu_gpu_feature_name_t featureNativeToWasi(WGPUFeatureName const* feature)
{
    switch (*feature) {
        case WGPUFeatureName_DepthClipControl:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DEPTH_CLIP_CONTROL;
        case WGPUFeatureName_Depth32FloatStencil8:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DEPTH32FLOAT_STENCIL8;
        case WGPUFeatureName_TextureCompressionBC:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_BC;
        case WGPUFeatureName_TextureCompressionBCSliced3D:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_BC_SLICED3D;
        case WGPUFeatureName_TextureCompressionETC2:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ETC2;
        case WGPUFeatureName_TextureCompressionASTC:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ASTC;
        case WGPUFeatureName_TextureCompressionASTCSliced3D:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ASTC_SLICED3D;
        case WGPUFeatureName_TimestampQuery:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TIMESTAMP_QUERY;
        case WGPUFeatureName_IndirectFirstInstance:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_INDIRECT_FIRST_INSTANCE;
        case WGPUFeatureName_ShaderF16:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_SHADER_F16;
        case WGPUFeatureName_RG11B10UfloatRenderable:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_RG11B10UFLOAT_RENDERABLE;
        case WGPUFeatureName_BGRA8UnormStorage:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_BGRA8UNORM_STORAGE;
        case WGPUFeatureName_Float32Filterable:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_FLOAT32_FILTERABLE;
        case WGPUFeatureName_Float32Blendable:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_FLOAT32_BLENDABLE;
        case WGPUFeatureName_ClipDistances:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_CLIP_DISTANCES;
        case WGPUFeatureName_DualSourceBlending:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DUAL_SOURCE_BLENDING;
        case WGPUFeatureName_Subgroups:
            return WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_SUBGROUPS;
        default:
            abort(); // TODO:
    }
}

WGPUFeatureName featureWasiToNative(wasi_webgpu_webgpu_gpu_feature_name_t const* feature)
{
    switch (*feature) {
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DEPTH_CLIP_CONTROL:
            return WGPUFeatureName_DepthClipControl;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DEPTH32FLOAT_STENCIL8:
            return WGPUFeatureName_Depth32FloatStencil8;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_BC:
            return WGPUFeatureName_TextureCompressionBC;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_BC_SLICED3D:
            return WGPUFeatureName_TextureCompressionBCSliced3D;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ETC2:
            return WGPUFeatureName_TextureCompressionETC2;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ASTC:
            return WGPUFeatureName_TextureCompressionASTC;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TEXTURE_COMPRESSION_ASTC_SLICED3D:
            return WGPUFeatureName_TextureCompressionASTCSliced3D;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_TIMESTAMP_QUERY:
            return WGPUFeatureName_TimestampQuery;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_INDIRECT_FIRST_INSTANCE:
            return WGPUFeatureName_IndirectFirstInstance;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_SHADER_F16:
            return WGPUFeatureName_ShaderF16;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_RG11B10UFLOAT_RENDERABLE:
            return WGPUFeatureName_RG11B10UfloatRenderable;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_BGRA8UNORM_STORAGE:
            return WGPUFeatureName_BGRA8UnormStorage;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_FLOAT32_FILTERABLE:
            return WGPUFeatureName_Float32Filterable;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_FLOAT32_BLENDABLE:
            return WGPUFeatureName_Float32Blendable;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_CLIP_DISTANCES:
            return WGPUFeatureName_ClipDistances;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_DUAL_SOURCE_BLENDING:
            return WGPUFeatureName_DualSourceBlending;
        case WASI_WEBGPU_WEBGPU_GPU_FEATURE_NAME_SUBGROUPS:
            return WGPUFeatureName_Subgroups;
        default:
            return WGPUFeatureName_Undefined;
    }
}
