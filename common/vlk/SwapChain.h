// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class PhysicalDevice;
class LogicalDevice;

// Forward declaration
class LogicalDevice;

/**
 * Wraps a Vulkan swap chain.
 */
class SwapChain final {
public:

    /**
     * Constructor.
     * 
     * @param handle    the swap chain handle
     * @param device    the physical device this swap chain belongs to
     * @param format    the image format of this swap chain
     * @param extent    the resolution of the swap chain images
     */
    SwapChain(VkSwapchainKHR handle, std::shared_ptr<LogicalDevice> device, VkFormat format, VkExtent2D extent);

    /**
     * Destructor.
     */
    ~SwapChain();

    /**
     * Returns the image format of this swap chain.
     * 
     * @return the image format
     */
    const VkFormat & GetImageFormat() const {
        return imageFormat;
    }

    /**
     * Returns the image resolution of this swap chain
     * 
     * @return the image resolution in pixels
     */
    const VkExtent2D & GetExtent() const {
        return extent;
    }

    /**
     * Returns the image width of this swap chain.
     * 
     * @return image width in pixels
     */
    uint32_t GetImageWidth() const {
        return GetExtent().width;
    }

    /**
     * Returns the image height of this swap chain.
     * 
     * @return height width in pixels
     */
    uint32_t GetImageHeight() const {
        return GetExtent().height;
    }
    
private:
    /** The handle of the swap chain. */
    VkSwapchainKHR swapChain;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /** The images of the swap chain. */
    std::vector<VkImage> images;

    /** The view to the swap chain images. */
    std::vector<VkImageView> imageViews;

    /** The image format of this swap chain. */
    VkFormat imageFormat;

    /** The resolution of the swap chain images. */
    VkExtent2D extent;
    

    /**
     * Creates an image view of an image.
     * 
     * @param image     the handle to the image
     */
    VkImageView CreateImageView(const VkImage & image) const;

    friend class SwapChainBuilder;
};

/**
 * Utility class used to build swap chain instances.
 */
class SwapChainBuilder final {
public:

    /**
     * Constructor.
     */
    SwapChainBuilder();


    /**
     * Specifies the surface format used for the swap chain.
     * 
     * @param format    the image format
     * @param cs        the color space
     * 
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & SurfaceFormat(VkFormat format, VkColorSpaceKHR cs) {
        ImageFormat(format);
        ColorSpace(cs);
        return *this;
    }

    /***
     * Specifies the image format used for the swap chain.
     * 
     * @param format    the image format
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ImageFormat(VkFormat format);

    /***
     * Specifies the colro space format used for the swap chain images.
     * 
     * @param cs    the color space
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ColorSpace(VkColorSpaceKHR cs);

    /**
     * Chooses an image format for the swap chain based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ChooseSurfaceFormat(const PhysicalDevice & device, VkSurfaceKHR surface);

    /**
     * Cooses an image format for the swap chain based on the given device.
     * 
     * @param device    the physical device
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ChooseSurfaceFormat(const VkPhysicalDevice device, VkSurfaceKHR surface);

    /***
     * Specifies the presentation mode used for the swap chain.
     * 
     * @param mode  the presentation mode
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & PresentMode(VkPresentModeKHR mode);

   /**
     * Specifies the resolution of the swap chain images.
     * 
     * @param width     the width in pixels
     * @param height    the height in pixels
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ImageExtent(uint32_t width, uint32_t height);

    /**
     * Chooses an image extent chain based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @param fbw       the width of the framebuffer in pixels
     * @param fbh       the height of the framebuffer in pixels
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & CooseImageExtend(const VkPhysicalDevice device, VkSurfaceKHR surface, int fbw, int fbh);

    /**
     * Chooses an image extent chain based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @param fbw       the width of the framebuffer in pixels
     * @param fbh       the height of the framebuffer in pixels
     * @return reference to this builder used for method chaining
     * 
     */
    SwapChainBuilder & CooseImageExtend(const PhysicalDevice & device, VkSurfaceKHR surface, int fbw, int fbh);

    /***
     * Specifies the number of images in the swap chain.
     * 
     * @param cnt   the number of images
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ImageCount(uint32_t cnt);

    /**
     * Chooses an image extent chain based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & CooseImageCount(const PhysicalDevice & device, VkSurfaceKHR surface);

    /**
     * Specifies the transformation applied to images of the swap chain.
     * 
     * @param tx    the transformation
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & Transform(VkSurfaceTransformFlagBitsKHR tx);

    /**
     * Chooses a transformation for images of the swap chain based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & CooseTransform(const PhysicalDevice & device, VkSurfaceKHR surface);

    /**
     * Chooses an appropriate configuration based on the given device.
     * 
     * @param device    the physical device
     * @param surface   the window surface
     * @param fbw       the width of the framebuffer in pixels
     * @param fbh       the height of the framebuffer in pixels
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & ChooseConfiguration(const PhysicalDevice & device, VkSurfaceKHR surface, int fbw, int fbh);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    SwapChainBuilder & Reset();

    /**
     * Builds the swap chain according to the current configuration.
     * 
     * @param device    the logical device to use
     * @param surface   the window surface
     * @return the newly created swap chain
     * @throws std::runtime_error in case the swap chain could not be created
     */
    std::unique_ptr<SwapChain> Build(std::shared_ptr<LogicalDevice> device, VkSurfaceKHR surface);

private:
    /** The image format used of the swap chain. */
    VkFormat imageFormat;   

    /** The color space used for the swap chain. */
    VkColorSpaceKHR colorSpace;

    /** The presentation mode of the swap chain. */
    VkPresentModeKHR presentMode;

    /** The old swap chain in case the swap chain needs to be recrated. */
    VkSwapchainKHR oldSwapChain;

    /** The resolution of the swap chain images. */
    VkExtent2D extent;

    /** Number of images in the swap chain. */
    uint32_t imageCount;


    /** The transformation applied to images of the swap chain. */
    VkSurfaceTransformFlagBitsKHR transform;
};