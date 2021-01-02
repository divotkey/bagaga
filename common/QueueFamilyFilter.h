#pragma once

#include <memory>
#include <vector>
#include <vulkan/vulkan.h>

class QueueFamilyFilter {
public:

    /**
     * Virtual destructor.
     */
    virtual ~QueueFamilyFilter() {}

    /**
     * Tests whether this filter accepts a queue family.
     * 
     * @param prop  the properties of the queue famile to test
     * @return `true` if this filter accepts this queue family
     */
    virtual bool Accept(const VkQueueFamilyProperties & prop) const = 0;

};

class AllQueueFamilyFilter : public QueueFamilyFilter {
public:

    AllQueueFamilyFilter & AddFilter(std::unique_ptr<QueueFamilyFilter> filter);

    // Inherited via QueueFamilyFilter
    virtual bool Accept(const VkQueueFamilyProperties & prop) const override;

private:
    std::vector<std::unique_ptr<QueueFamilyFilter>> filters;
};

class FlagQueueFamilyFilter : public QueueFamilyFilter {
public:

    FlagQueueFamilyFilter(VkQueueFlags flag);

    // Inherited via QueueFamilyFilter
    virtual bool Accept(const VkQueueFamilyProperties & prop) const override;

private:
    VkQueueFlags flag;
};

class PresentationQueueFamilyFilter : public QueueFamilyFilter {
public:

    PresentationQueueFamilyFilter(VkSurfaceKHR surface);

    // Inherited via QueueFamilyFilter
    virtual bool Accept(const VkQueueFamilyProperties & prop) const override;

private:
    VkSurfaceKHR surface;
};

