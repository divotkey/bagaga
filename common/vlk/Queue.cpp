// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "Semaphore.h"
#include "CommandBuffer.h"
#include "SwapChain.h"
#include "Queue.h"

using namespace std;

Queue::Queue(VkQueue handle)
    : queue(handle)
{
    // Intentionally left empty.
}

void Queue::ClearWaitSemaphores()
{
    waitSemaphores.clear();
    waitSemaphoreHandles.clear();
    waitStages.clear();
}

void Queue::AddWaitSemaphore(std::shared_ptr<Semaphore> semaphore, VkPipelineStageFlags stages)
{
    assert(queue);
    assert(waitSemaphores.size() == waitSemaphoreHandles.size());
    assert(waitSemaphores.size() == waitStages.size());

    waitSemaphores.push_back(semaphore);
    waitSemaphoreHandles.push_back(semaphore->GetHandle());
    waitStages.push_back(stages);

    assert(waitSemaphores.size() == waitSemaphoreHandles.size() == waitStages.size());
}

void Queue::ClearSignalSemaphores()
{
    signalSemaphoreHandles.clear();
    signalSemaphores.clear();
}

void Queue::AddSignalSemaphore(std::shared_ptr<Semaphore> semaphore)
{
    signalSemaphores.push_back(semaphore);
    signalSemaphoreHandles.push_back(semaphore->GetHandle());
}


void Queue::SubmitCommandBuffer(CommandBuffer & cmdBuf)
{
    assert(queue);
    assert(waitSemaphores.size() == waitSemaphoreHandles.size() == waitStages.size());
    assert(signalSemaphores.size() == signalSemaphoreHandles.size() == waitStages.size());
    
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;    
    submitInfo.waitSemaphoreCount  = static_cast<uint32_t>(waitSemaphores.size());
    submitInfo.pWaitSemaphores = waitSemaphoreHandles.data();
    submitInfo.pWaitDstStageMask = waitStages.data();

    auto cmdBufHandle = cmdBuf.GetHandle();
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBufHandle;

    submitInfo.signalSemaphoreCount = static_cast<uint32_t>(signalSemaphores.size());
    submitInfo.pSignalSemaphores = signalSemaphoreHandles.data();

    VkResult res = vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
    if (res != VK_SUCCESS) {
        throw runtime_error("Failed to submit command buffer, error " + to_string(res));
    }
}


void Queue::Present(SwapChain & swapChain, uint32_t imageIndex)
{
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = static_cast<uint32_t>(waitSemaphores.size());
    presentInfo.pWaitSemaphores = waitSemaphoreHandles.data();
    auto swapChainHandle = swapChain.GetHandle();
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &swapChainHandle;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr;

    VkResult res = vkQueuePresentKHR(queue, &presentInfo);
    switch (res) {
    case VK_SUCCESS:
    case VK_SUBOPTIMAL_KHR:
        return;

    default:
        throw std::logic_error("Unable to queue an image for presentation, error " + to_string(res));
    }
}

void Queue::WaitIdle()
{
    VkResult res = vkQueueWaitIdle(queue);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to call wait idle on queue, error " + to_string(res));
    }
}

