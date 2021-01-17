// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <string>
#include <memory>
#include <vector>

// Vulkan SDK includes
#include <vulkan/vulkan.h>

// Local includes
#include "vlk/NameList.h"

using namespace std;

/**
 * Wraps a Vulkan instance.
 */
class VulkanInstance {
public:

    /**
     * Returns the API version of the Vulkan instance.
     * 
     * @param string represening the API version
     */
    static string GetVersion();

    /**
     * Returns the names of available Vulkan instance layers.
     */
    static vector<string> GetAvailableLayers();

    /**
     * Tests wheter certain instance layers are available.
     * 
     * @param layers    the required layers
     * @return `true` if the layers are available, `false` otherwise
     */
    static bool CheckLayers(const vector<string> & layers);

    /**
     * Tests wheter certain instance layers are available.
     * 
     * @param layers    the required layers
     * @return `true` if the layers are available, `false` otherwise
     */
    static bool CheckLayers(const vector<const char*> & layers);

    /**
     * Returns the names of available Vulkan instance extensions.
     */
    static vector<string> GetAvailableExtensions();


    /**
     * Deleted copy constructor.
     * 
     * Vulkan instance should not be duplicated
     */    
    VulkanInstance(const VulkanInstance &) = delete;

    /**
     * Deleted copy assignment operator.
     * 
     * Vulkan instance should not be duplicated
     */    
    VulkanInstance& operator=(const VulkanInstance &) = delete;

    /**
     * Virtual destructor.
     */
    virtual ~VulkanInstance();

    /**
     * Returns the const handle to the Vulkan instance.
     * 
     * @return the vulkan instance
     */
    const VkInstance GetHandle() const {
        return vkInstance;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkInstance() const { 
        return GetHandle();
    }

private:
    /** The Vulkan instance. */
    VkInstance vkInstance;

    /**
     * Private constructor.
     */
    VulkanInstance(VkInstance instance);

    friend class VulkanInstanceBuilder;
};


/**
 * Builds Vulkan instances.
 */
class VulkanInstanceBuilder {
public:

    /**
     * Constructor.
     */
    VulkanInstanceBuilder();

    /**
     * Adds an extension.
     * 
     * @param extensionName the name of the extension
     * @return reference to this builder for method chaining
     * @throws logic_error in case the extension has already been added
     */
    VulkanInstanceBuilder & AddExtension(const string & extensionName);

    /**
     * Adds serveral extensions at once.
     * 
     * @param extensionNames    the vector with extension names
     * @return reference to this builder for method chaining
     * @throws logic_error in case the extension has already been added
     */
    VulkanInstanceBuilder & AddExtensions(const vector<string> & extensionNames);

    /**
     * Adds serveral extensions at once.
     * 
     * @param extensionNames    the vector with extension names
     * @return reference to this builder for method chaining
     * @throws logic_error in case the extension has already been added
     */
    VulkanInstanceBuilder & AddExtensions(const vector<const char *> & extensionNames);

    /**
     * Tests whether the extenions has already been added.
     * 
     * @param extensionName the name of the extenions to test
     * @return `true` in case the extension has already been added
     */
    bool HasExtension(const string & extensionName) const noexcept;

    /**
     * Adds a layer.
     * 
     * @return reference to this builder for method chaining
     * @throws logic_error in case the layer has already been added
     */
    VulkanInstanceBuilder & AddLayer(const string & layerName);

    /**
     * Adds serveral layers at once.
     * 
     * @param layerNames    the vector with layer names
     * @return reference to this builder for method chaining
     * @throws logic_error in case the extension has already been added
     */
    VulkanInstanceBuilder & AddLayers(const vector<string> & layerNames);

    /**
     * Adds serveral layers at once.
     * 
     * @param layerNames    the vector with layer names
     * @return reference to this builder for method chaining
     * @throws logic_error in case the extension has already been added
     */
    VulkanInstanceBuilder & AddLayers(const vector<const char *> & layerNames);

    /**
     * Tests whether the layer has already been added.
     * 
     * @param extensionName the name of the layer to test
     * @return `true` in case the layer has already been added
     */
    bool HasLayer(const string & layerName) const noexcept;


    /**
     * Resets this builder to its initial state.
     * 
     * @return reference to this builder for method chaining
     */
    VulkanInstanceBuilder & Reset() noexcept;

    /**
     * Builds the Vulkan instance according to the current configuration.
     * 
     * @return the Vulkan instance
     */
    unique_ptr<VulkanInstance> Build();

private:

    /** The names of instance extensions. */
    NameList extensionNames;

   /** The names of device layers. */
    NameList layerNames;
};