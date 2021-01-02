#include <stdexcept>
#include <string>
#include "QueueFamilyFilter.h"

AllQueueFamilyFilter & AllQueueFamilyFilter::AddFilter(std::unique_ptr<QueueFamilyFilter> filter)
{
    filters.push_back(std::move(filter));
    return *this;
}

bool AllQueueFamilyFilter::Accept(const VkQueueFamilyProperties & prop) const
{
    for (const auto & filter : filters) {
        if (!filter->Accept(prop)) {
            return false;
        }
    }

    return true;
}

/////////////////////////////////////////////////
/////// FlagQueueFamilyFilter
/////////////////////////////////////////////////

FlagQueueFamilyFilter::FlagQueueFamilyFilter(VkQueueFlags flag)
    : flag(flag)
{
    // Intentionally left empty.    
}

bool FlagQueueFamilyFilter::Accept(const VkQueueFamilyProperties & prop) const
{
    return prop.queueFlags & flag;
}

/////////////////////////////////////////////////
/////// PresentationQueueFamilyFilter
/////////////////////////////////////////////////

PresentationQueueFamilyFilter::PresentationQueueFamilyFilter(VkSurfaceKHR surface)
    : surface(surface)
{
    // Intentionally left empty.    
}


bool PresentationQueueFamilyFilter::Accept(const VkQueueFamilyProperties & prop) const
{
    VkBool32 presentSupport = false;
    // VkResult res = vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
    // if (res != VK_SUCCESS) {
    //     throw std::runtime_error(  
    //         "Unable to determine if queue family supports presentation to surface, error code " 
    //             + to_string(res));
    // }
    return false;
}


