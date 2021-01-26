// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>
#include <string>

// Local includes
#include "LogicalDevice.h"
#include "Semaphore.h"

using namespace std;

unique_ptr<Semaphore> Semaphore::Create(std::shared_ptr<LogicalDevice> device)
{
    assert(device);
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    
    VkSemaphore handle;
    VkResult res = vkCreateSemaphore(*device, &semaphoreInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create semaphore, error " + to_string(res));
    }

    return unique_ptr<Semaphore>(new Semaphore(handle, device));
}

Semaphore::Semaphore(VkSemaphore handle, std::shared_ptr<LogicalDevice> device)
    : semaphore(handle), device(device)
{
    assert(handle);
    assert(device);
}

Semaphore::~Semaphore()
{
    vkDestroySemaphore(*device, semaphore, nullptr);
}