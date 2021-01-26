// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;

/**
 * Wraps a Vulkan sempahore object.
 */
class Semaphore final {
public:

    /**
     * Factory method for new semaphore objects.
     * 
     * @param device    the physical device this command pool object belongs to
     * @throws std::runtime_error in case the semaphore could not be created
     */
    static std::unique_ptr<Semaphore> Create(std::shared_ptr<LogicalDevice> device);

    /**
     * Deleted copy constructor.
     */
    Semaphore(const Semaphore &) = delete;

    /**
     * Deleted assignment operator.
     */
    Semaphore & operator =(const Semaphore &) = delete;

    /**
     * Destructor.
     */
    ~Semaphore();

    /**
     * Returns the Vulkan handle of this command semaphore object.
     * 
     * @return the handle
     */
    VkSemaphore GetHandle() const {
        return semaphore;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkSemaphore() const { 
        return GetHandle();
    }    

private:    
    /** The Vulkan handle to the semaphore object. */
    VkSemaphore semaphore;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;

    /**
     * Constructor.
     * 
     * @param handle    the handle to the semaphore
     * @param device    the physical device this command pool object belongs to
     */
    Semaphore(VkSemaphore handle, std::shared_ptr<LogicalDevice> device);
};
