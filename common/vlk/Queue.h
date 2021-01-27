// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class Semaphore;
class CommandBuffer;
class SwapChain;

/**
 * Wraps a Vulkan queue.
 */
class Queue {
public:

    /**
     * Constructor.
     * 
     * @param handle    the handle to the queue object
     */
    Queue(VkQueue handle = VK_NULL_HANDLE);

    void ClearWaitSemaphores();
    void AddWaitSemaphore(std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags stages = 0);
    void SubmitCommandBuffer(CommandBuffer & cmdBuf);
    void ClearSignalSemaphores();
    void AddSignalSemaphore(std::shared_ptr<Semaphore> semaphore);

    void Present(SwapChain & swapChain, uint32_t imageIndex);  

    /**
     * Wait on the host for the completion of outstanding queue operations for a given queue.
     */
    void WaitIdle();

    /**
     * Returns the handle to the queue object.
     * 
     * @return the queue handle
     */
    VkQueue GetHandle() const {
        return queue;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkQueue() const { 
        return GetHandle();
    }

private:
    /** The Vulkan handle to the queue object. */
    VkQueue queue;

    std::vector<std::shared_ptr<Semaphore>> waitSemaphores;
    std::vector<VkPipelineStageFlags> waitStages;
    std::vector<VkSemaphore> waitSemaphoreHandles;
    std::vector<std::shared_ptr<Semaphore>> signalSemaphores;
    std::vector<VkSemaphore> signalSemaphoreHandles;

    friend class LogicalDeviceBuilder;
};