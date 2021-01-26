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
     * Deleted copy constructor.
     */
    ShaderModule(const ShaderModule &) = delete;

    /**
     * Deleted assignment operator.
     */
    ShaderModule & operator =(const ShaderModule &) = delete;

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
    /** The handle of shader module. */
    VkShaderModule shaderModule;

    /** The logical device this swap chain belongs to. */
    std::shared_ptr<LogicalDevice> device;


    /**
     * Constructor.
     * 
     * @param handle    the swap chain handle
     * @param device    the physical device this swap chain belongs to
     */
    ShaderModule(VkShaderModule handle, std::shared_ptr<LogicalDevice> device);

    friend class ShaderModuleBuilder;
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


    /**
     * Specifies the byte code for the shader model.
     * 
     * @param shaderCode    the byte code data
     * @param codeSize      the length of the byte code data in bytes
     */
    ShaderModuleBuilder & Code(const unsigned char * shaderCode, size_t codeSize);
    
    /**
     * Specifies the byte code for the shader model.
     * 
     * @param shaderCode    the byte code data
     */
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
 * Wraps a Vulkan shader stage info structure and contains the required data.
 */
class ShaderStageInfo {
public:

    /**
     * Constructor.
     * 
     * @param shaderStage
     * @param shaderModule
     * @param entryPoint
     */
    ShaderStageInfo(
        VkPipelineShaderStageCreateInfo shaderStage, 
        std::shared_ptr<ShaderModule> shaderModule, 
        const std::string &entryPoint);

    /**
     * Copy constructor.
     * 
     * @param o the other instance
     */
    ShaderStageInfo(const ShaderStageInfo & o);

    /**
     * Return the Vulkan shader stage info structure.
     * 
     * @return the shader stage info
     */
    const VkPipelineShaderStageCreateInfo & GetInfo() const {
        return shaderStage;
    }

    /**
     * Copy assignment operator.
     * 
     * @param rhs   the right hand side
     * @return reference to this instance
     */
    ShaderStageInfo & operator= (const ShaderStageInfo & rhs);

private:
    /** The shader stage info structure. */
    VkPipelineShaderStageCreateInfo shaderStage;

    /** Specifies the entry point name of the shader for this stage. */
    std::string entryPoint;

    /** The shader module of this stage. */
    std::shared_ptr<ShaderModule> shaderModule;

    void UpdateAndValidate();
};

/**
 * Utility class used to build shader stage info structures.
 */
class ShaderStageBuilder {
public:

    /** The default entry point of shader. */
    static const std::string kDefaultEntryPoint;

    /**
     * Constructor.
     */ 
    ShaderStageBuilder();


    /**
     * Specifies how the pipeline shader stage will be generated.
     * 
     * @param flags the create flags
     * @return reference to this builder used for method chaining
     */
    ShaderStageBuilder & CreateFlags(VkPipelineShaderStageCreateFlags flags);

    /**
     * Specifies a single pipeline stage.
     * 
     * @param flag  the stage flag bits
     * @return reference to this builder used for method chaining
     */
    ShaderStageBuilder & Stage(VkShaderStageFlagBits flag);

    /**
     * Specifies the name of the entry point of the shader stage.
     * 
     * @param name  the entryp point name
     * @return reference to this builder used for method chaining
     */
    ShaderStageBuilder & EntryPoint(const char * name);

    /**
     * Specifies the name of the entry point of the shader stage.
     * 
     * @param name  the entryp point name
     * @return reference to this builder used for method chaining
     */
    ShaderStageBuilder & EntryPoint(const std::string & name)
    {
        return EntryPoint(name.c_str());
    }

    ShaderStageBuilder & Module(std::shared_ptr<ShaderModule> shaderModule);
 
    /**
     * Resets this builder to its initial condition.
     * 
     * @return reference to this builder used for method chaining
     */
    ShaderStageBuilder & Reset();

    /**
     * Builds the shader stage info structore.
     * 
     * @return the shader stage info structure
     */
    ShaderStageInfo Build() const;

private:
    /** Specifies how the pipeline shader stage will be generated. */
    VkPipelineShaderStageCreateFlags createFlags;

    /** Specifies a single pipeline stage. */
    VkShaderStageFlagBits stage;

    /** Specifies the entry point name of the shader for this stage. */
    std::string entryPoint;

    /** Object containing the shader for this stage. */
    std::shared_ptr<ShaderModule> shaderModule;
};