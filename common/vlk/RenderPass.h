// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;
class SwapChain;


/**
 * Wraps a Vulkan render pass object.
 */
class RenderPass final {
public:

    /**
     * Deleted copy constructor.
     */
    RenderPass(const RenderPass &) = delete;

    /**
     * Deleted assignment operator.
     */
    RenderPass & operator =(const RenderPass &) = delete;

    /**
     * Destructor.
     */
    ~RenderPass();

    /**
     * Returns the Vulkan handle to the render pass object.
     * 
     * @return the Vulkan handle
     */
    VkRenderPass GetHandle() const {
        return renderPass;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkRenderPass() const { 
        return GetHandle();
    }

private:
    /** Handle to the render pass object. */
    VkRenderPass renderPass;

    /** The logical Vulkan device used to create this render pass object. */
    std::shared_ptr<LogicalDevice> device;

    /**
     * Constructor.
     * 
     * @param handle    handle to the render pass object
     * @param device    the logical vulkan device used to create this object
     */
    RenderPass(VkRenderPass handle, std::shared_ptr<LogicalDevice> device);

    friend class RenderPassBuilder;
};

/**
 * Utility class used to build Vulkan supass dependecy structures.
 */
class SubpassDependencyBuilder {
public:

    SubpassDependencyBuilder();

    SubpassDependencyBuilder & SrcSubpass(uint32_t index);
    SubpassDependencyBuilder & DstSubpass(uint32_t index);
    SubpassDependencyBuilder & SrcStageMask(VkPipelineStageFlags stageMask);
    SubpassDependencyBuilder & DstStageMask(VkPipelineStageFlags stageMask);
    SubpassDependencyBuilder & SrcAccessMask(VkAccessFlags accessMask);
    SubpassDependencyBuilder & DstAccessMask(VkAccessFlags accessMask);
    SubpassDependencyBuilder & DependencyFlags(VkDependencyFlags flags);

    SubpassDependencyBuilder & Reset();

    VkSubpassDependency Build();
private:
    /** Is the subpass index of the first subpass in the dependency. */
    uint32_t srcSubpass;

    /** Is the subpass index of the second subpass in the dependency. */
    uint32_t dstSubpass;

    /** A bitmask of VkPipelineStageFlagBits specifying the source stage mask. */
    VkPipelineStageFlags srcStageMask;

    /** A bitmask of VkPipelineStageFlagBits specifying the destination stage mask. */
    VkPipelineStageFlags dstStageMask;

    /** A bitmask of VkAccessFlagBits specifying a source access mask. */
    VkAccessFlags srcAccessMask;

    /** A bitmask of VkAccessFlagBits specifying a destination access mask. */
    VkAccessFlags dstAccessMask;

    /** A bitmask of VkDependencyFlagBits. */
    VkDependencyFlags dependencyFlags;
};

/**
 * Utility class used to build Vulkan attachmend description structures.
 */
class AttachmentBuilder {
public:

    /**
     * Constructor.
     */
    AttachmentBuilder();

    AttachmentBuilder& Format(VkFormat format);

    AttachmentBuilder& Samples(VkSampleCountFlagBits samples);

    /**
     * Specifies how the contents of color/depth components are treated at the beginning.
     * 
     * @param op    the load operation
     * @return reference to this builder used for method chaining
     */
    AttachmentBuilder& LoadOp(VkAttachmentLoadOp op);

    /**
     * Specifies how the contents of color/depth components are treated at the end.
     * 
     * @param op    the store operation
     * @return reference to this builder used for method chaining
     */
    AttachmentBuilder& StoreOp(VkAttachmentStoreOp op);

    /**
     * Specifies how the contents of stencil components are treated at the beginning.
     * 
     * @param op    the load operation
     * @return reference to this builder used for method chaining
     */
    AttachmentBuilder& StencilLoadOp(VkAttachmentLoadOp op);

    /**
     * Specifies how the contents of stencil components are treated at the end.
     * 
     * @param op    the store operation
     * @return reference to this builder used for method chaining
     */
    AttachmentBuilder& StencilStoreOp(VkAttachmentStoreOp op);

    /**
     * Layout the attachment image subresource will be in when a render pass instance begins.
     * 
     * @param layout    the initial image layout
     * @return reference to this builder used for method chaining
     **/
    AttachmentBuilder& InitialLayout(VkImageLayout layout);

    /**
     * Layout the attachment image subresource will be transitioned to when a render pass instance ends.
     * 
     * @param layout    the final image layout
     * @return reference to this builder used for method chaining
     **/
    AttachmentBuilder& FinalLayout(VkImageLayout layout);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    AttachmentBuilder& Reset();

    /**
     * Builds the attachment description according to the current configuration.
     * 
     * @return the newly built attachment description
     */
    VkAttachmentDescription Build() const;

private:
    /** Bitmask specifying additional properties of the attachment. */
    VkAttachmentDescriptionFlags flags;

    /** Specifies the format of the image view that will be used for the attachment. */
    VkFormat format;

    /** Is the number of samples of the image. */
    VkSampleCountFlagBits samples;

    /** Specifies how the contents of color/depth components are treated at the beginning. */
    VkAttachmentLoadOp loadOp;

    /** Specifies how the contents of color/depth components are treated at the end. */
    VkAttachmentStoreOp storeOp;

    /** Specifies how the contents of stencil components are treated at the beginning. */
    VkAttachmentLoadOp stencilLoadOp;

    /** Specifies how the contents of stencil components are treated at the end. */
    VkAttachmentStoreOp stencilStoreOp;

    /** Layout the attachment image subresource will be in when a render pass instance begins. */
    VkImageLayout initialLayout;

    /** Layout the attachment image subresource will be transitioned to when a render pass instance ends. */
    VkImageLayout finalLayout;

};


/**
 * Wraps a render subpass description and holds the color
 * attachment references as well.
 */
class Subpass {
public:

    /**
     * Returns the Vulkan subpass description structure.
     * 
     * @return the subpass description
     */
    VkSubpassDescription GetDescription() const {
        return subpass;
    }

private:
    /** The subpass description. */
    VkSubpassDescription subpass;

    /** Attachments that are read from a shader. */
    std::vector<VkAttachmentReference> inputAttachments;

    /** Corresponds to an output location in the shader. */
    std::vector<VkAttachmentReference> colorAttachments;

    /** Used for multisampling color attachments. */
    std::vector<VkAttachmentReference> resolveAttachments;

    /** Attachment for depth and stencil data. */
    std::vector<VkAttachmentReference> depthStencilAttachments;

    /** Attachments that are not used by this subpass, but for which the data must be preserved. */
    std::vector<uint32_t> preserveAttachments;

    friend class SubpassBuilder;
};

/**
 * Utility class used to build Vulkan render subpass descriptions.
 */
class SubpassBuilder {
public:

    /**
     * Constructor.
     */
    SubpassBuilder();

    SubpassBuilder& Flags(VkSubpassDescriptionFlags flags);
    SubpassBuilder& PipelineBindPoint(VkPipelineBindPoint bindPoint);


    SubpassBuilder& AddInputAttachment(uint32_t index = VK_ATTACHMENT_UNUSED, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);
    SubpassBuilder& AddColorAttachment(uint32_t index = VK_ATTACHMENT_UNUSED, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);
    SubpassBuilder& AddResolveAttachment(uint32_t index = VK_ATTACHMENT_UNUSED, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);
    SubpassBuilder& AddDepthStencilAttachment(uint32_t index = VK_ATTACHMENT_UNUSED, VkImageLayout layout = VK_IMAGE_LAYOUT_UNDEFINED);
    SubpassBuilder& AddPreserveAttachment(uint32_t index);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    SubpassBuilder& Reset();

    /**
     * Builds the subpass according to the current configuration.
     * 
     * @return the newly built subpass
     */
    Subpass Build() const;

private:
    /** Specifies usage of the subpass. */
    VkSubpassDescriptionFlags flags;

    /** Specifies the pipeline type supported for this subpass. */
    VkPipelineBindPoint pipelineBindPoint;

    /** Attachments that are read from a shader. */
    std::vector<VkAttachmentReference> inputAttachments;

    /** Corresponds to an output location in the shader. */
    std::vector<VkAttachmentReference> colorAttachments;

    /** Used for multisampling color attachments. */
    std::vector<VkAttachmentReference> resolveAttachments;

    /** Attachment for depth and stencil data. */
    std::vector<VkAttachmentReference> depthStencilAttachments;

    /** Attachments that are not used by this subpass, but for which the data must be preserved. */
    std::vector<uint32_t> preserveAttachments;
};

/**
 * Utility class used to build Vulkan render pass objects.
 */
class RenderPassBuilder {
public:

    /**
     * Constructor.
     */
    RenderPassBuilder();

    /**
     * Adds an attachment to the render pass to build.
     * 
     * @param attachment    the attachment description
     * @return reference to this builder for method chaining
     */
    RenderPassBuilder&  AddAttachment(const VkAttachmentDescription & attachment);

    /**
     * Adds a supbass to this render pass to build.
     * 
     * @param subpass    the subpass to add
     */
    RenderPassBuilder& AddSubpass(const Subpass & subpass);

    /**
     * Adds a supbass dependency to this render pass to build.
     * 
     * @param dependency    the dependency to add
     */
    RenderPassBuilder& AddSubpassDependency(const VkSubpassDependency & dependency);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    RenderPassBuilder& Reset();

    /**
     * Builds the swap chain according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created render pass object
     * @throws std::runtime_error in case render pass object could not be created
     */
    std::unique_ptr<RenderPass> Build(std::shared_ptr<LogicalDevice> device) const;

private:
    /** The attachments of the render pass to build. */
    std::vector<VkAttachmentDescription> attachments;

    /** The render subpasses of the render pass to build. */
    std::vector<Subpass> subpasses;

    /** The subpass dependencies. */
    std::vector<VkSubpassDependency> subpassDependencies;

};