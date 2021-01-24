// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

#pragma once

// C++ Standard Library includes
#include <memory>
#include <string>
#include <vector>

// Vulkan includes
#include <vulkan/vulkan.h>

// Forward declaration
class LogicalDevice;

/**
 * Wraps a Vulkan shader module.
 */
class ShaderModule {
public:

    /**
     * Constructor.
     * 
     * @param handle    the swap chain handle
     * @param device    the physical device this swap chain belongs to
     */
    ShaderModule(VkShaderModule handle, std::shared_ptr<LogicalDevice> device);

    /**
     * Destructor.
     */
    ~ShaderModule();

    /**
     * Returns the handle of the shader module object.
     * 
     * @return the handle of the shader module object
     */
    VkShaderModule GetHandle() const {
        return shaderModule;
    }

    /**
     * Implicit conversion to Vulkan handle
     */
    operator VkShaderModule() const { 
        return GetHandle();
    }

private:
    /** The handle of shader module */
    VkShaderModule shaderModule;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;
};

/**
 * Utility class used to build shader module objects.
 */
class ShaderModuleBuilder {
public:

    /**
     * Constructor.
     */ 
    ShaderModuleBuilder();


    ShaderModuleBuilder & Code(const unsigned char * shaderCode, size_t codeSize);
    ShaderModuleBuilder & Code(const std::vector<unsigned char> & shaderCode);

    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ShaderModuleBuilder & Reset();

    /**
     * Builds the shader module object according to the current configuration.
     * 
     * @param device    the logical device to use
     * @return the newly created shader module
     * @throws std::runtime_error in case the shader module could not be created
     */
    std::unique_ptr<ShaderModule> Build(std::shared_ptr<LogicalDevice> device) const;

private:
    /** The byte code that is used to create the shader module. */
    std::vector<unsigned char> code;
};


/**
 * Utility class used to build shader stage info structures.
 */
class ShaderStageInfoBuilder {
public:

    /** The default entry point of shader. */
    static const std::string kDefaultEntryPoint;

    /**
     * Constructor.
     */ 
    ShaderStageInfoBuilder();

    ShaderStageInfoBuilder & Stage(VkShaderStageFlagBits flag);


    ShaderStageInfoBuilder & EntryPoint(const char * name);

    ShaderStageInfoBuilder & EntryPoint(const std::string & name)
    {
        return EntryPoint(name.c_str());
    }

   ShaderStageInfoBuilder & Module(VkShaderModule shaderModule);
 
    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ShaderStageInfoBuilder & Reset();

    /**
     * Builds the shader stage info structore.
     * 
     * @return the shader stage info structure
     */
    VkPipelineShaderStageCreateInfo Build() const;

private:
    /** Specifies a single pipeline stage. */
    VkShaderStageFlagBits stage;

    /** Specifies the entry point name of the shader for this stage. */
    std::string entryPoint;

    /** Object containing the shader for this stage. */
    VkShaderModule shaderModule;
};