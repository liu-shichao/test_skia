/*
 * Copyright 2021 Google LLC
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef skgpu_graphite_MtlCommandBuffer_DEFINED
#define skgpu_graphite_MtlCommandBuffer_DEFINED

#include "src/gpu/graphite/CommandBuffer.h"
#include "src/gpu/graphite/DrawPass.h"
#include "src/gpu/graphite/GpuWorkSubmission.h"
#include "src/gpu/graphite/Log.h"

#include <memory>

#include "include/core/SkTypes.h"
#include "include/ports/SkCFObject.h"

#import <Metal/Metal.h>

namespace skgpu::graphite {
class MtlBlitCommandEncoder;
class MtlComputeCommandEncoder;
class MtlRenderCommandEncoder;
class MtlSharedContext;

class MtlCommandBuffer final : public CommandBuffer {
public:
    static sk_sp<MtlCommandBuffer> Make(id<MTLCommandQueue>, const MtlSharedContext*);
    ~MtlCommandBuffer() override;

    bool isFinished() {
        return (*fCommandBuffer).status == MTLCommandBufferStatusCompleted ||
               (*fCommandBuffer).status == MTLCommandBufferStatusError;

    }
    void waitUntilFinished(const SharedContext*) {
        // TODO: it's not clear what do to if status is Enqueued. Commit and then wait?
        if ((*fCommandBuffer).status == MTLCommandBufferStatusScheduled ||
            (*fCommandBuffer).status == MTLCommandBufferStatusCommitted) {
            [(*fCommandBuffer) waitUntilCompleted];
        }
        if (!this->isFinished()) {
            SKGPU_LOG_E("Unfinished command buffer status: %d",
                        (int)(*fCommandBuffer).status);
            SkASSERT(false);
        }
    }
    bool commit();

private:
    MtlCommandBuffer(sk_cfp<id<MTLCommandBuffer>> cmdBuffer, const MtlSharedContext* sharedContext);

    bool onAddRenderPass(const RenderPassDesc&,
                         const Texture* colorTexture,
                         const Texture* resolveTexture,
                         const Texture* depthStencilTexture,
                         const std::vector<std::unique_ptr<DrawPass>>& drawPasses) override;
    bool onAddComputePass(const ComputePassDesc&,
                          const ComputePipeline*,
                          const std::vector<ResourceBinding>& bindings) override;

    // Methods for populating a MTLRenderCommandEncoder:
    bool beginRenderPass(const RenderPassDesc&,
                         const Texture* colorTexture,
                         const Texture* resolveTexture,
                         const Texture* depthStencilTexture);
    void endRenderPass();

    void addDrawPass(const DrawPass*);

    void bindGraphicsPipeline(const GraphicsPipeline*);
    void setBlendConstants(float* blendConstants);

    void bindUniformBuffer(const BindBufferInfo& info, UniformSlot);
    void bindDrawBuffers(const BindBufferInfo& vertices,
                         const BindBufferInfo& instances,
                         const BindBufferInfo& indices);
    void bindVertexBuffers(const Buffer* vertexBuffer, size_t vertexOffset,
                           const Buffer* instanceBuffer, size_t instanceOffset);
    void bindIndexBuffer(const Buffer* indexBuffer, size_t offset);

    void bindTextureAndSampler(const Texture*, const Sampler*, unsigned int bindIndex);

    void setScissor(unsigned int left, unsigned int top,
                    unsigned int width, unsigned int height);
    void setViewport(float x, float y, float width, float height,
                     float minDepth, float maxDepth);

    void draw(PrimitiveType type, unsigned int baseVertex, unsigned int vertexCount);
    void drawIndexed(PrimitiveType type, unsigned int baseIndex, unsigned int indexCount,
                     unsigned int baseVertex);
    void drawInstanced(PrimitiveType type,
                       unsigned int baseVertex, unsigned int vertexCount,
                       unsigned int baseInstance, unsigned int instanceCount);
    void drawIndexedInstanced(PrimitiveType type, unsigned int baseIndex,
                              unsigned int indexCount, unsigned int baseVertex,
                              unsigned int baseInstance, unsigned int instanceCount);

    // Methods for populating a MTLComputeCommandEncoder:
    void beginComputePass();
    void bindComputePipeline(const ComputePipeline*);
    void bindBuffer(const Buffer* buffer, unsigned int offset, unsigned int index);
    void dispatchThreadgroups(const WorkgroupSize& globalSize, const WorkgroupSize& localSize);
    void endComputePass();

    // Methods for populating a MTLBlitCommandEncoder:
    bool onCopyTextureToBuffer(const Texture*,
                               SkIRect srcRect,
                               const Buffer*,
                               size_t bufferOffset,
                               size_t bufferRowBytes) override;
    bool onCopyBufferToTexture(const Buffer*,
                               const Texture*,
                               const BufferTextureCopyData* copyData,
                               int count) override;

    MtlBlitCommandEncoder* getBlitCommandEncoder();
    void endBlitCommandEncoder();

    sk_cfp<id<MTLCommandBuffer>> fCommandBuffer;
    sk_sp<MtlRenderCommandEncoder> fActiveRenderCommandEncoder;
    sk_sp<MtlComputeCommandEncoder> fActiveComputeCommandEncoder;
    sk_sp<MtlBlitCommandEncoder> fActiveBlitCommandEncoder;

    size_t fCurrentVertexStride = 0;
    size_t fCurrentInstanceStride = 0;
    id<MTLBuffer> fCurrentIndexBuffer;
    size_t fCurrentIndexBufferOffset = 0;

    const MtlSharedContext* fSharedContext;
};

} // namespace skgpu::graphite

#endif // skgpu_graphite_MtlCommandBuffer_DEFINED
