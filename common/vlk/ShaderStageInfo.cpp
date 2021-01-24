// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <cassert>
#include <stdexcept>

// Local includes
#include "LogicalDevice.h"
#include "ShaderStageInfo.h"

using namespace std;

/////////////////////////////////////////////////
/////// ShaderModule
/////////////////////////////////////////////////

ShaderModule::ShaderModule(VkShaderModule handle, std::shared_ptr<LogicalDevice> device)
    : shaderModule(handle)
    , device(device)
{
    assert(device);
    assert(handle);
}

ShaderModule::~ShaderModule()
{
    if (shaderModule && device) {
        vkDestroyShaderModule(*device, shaderModule, nullptr);
    }
}


/////////////////////////////////////////////////
/////// ShaderModuleBuilder
/////////////////////////////////////////////////

ShaderModuleBuilder::ShaderModuleBuilder()
{
    Reset();
}

ShaderModuleBuilder & ShaderModuleBuilder::Code(const unsigned char * shaderCode, size_t codeSize)
{
    code.insert(code.end(), shaderCode, shaderCode + codeSize);
    return *this;
}

ShaderModuleBuilder & ShaderModuleBuilder::Code(const std::vector<unsigned char> & shaderCode)
{
    code = shaderCode;
    return *this;    
}

ShaderModuleBuilder & ShaderModuleBuilder::Reset()
{
    code.resize(0);
    code.shrink_to_fit();
    return *this;
}

std::unique_ptr<ShaderModule> ShaderModuleBuilder::Build(std::shared_ptr<LogicalDevice> device) const
{
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule handle;
    VkResult res = vkCreateShaderModule(*device, &createInfo, nullptr, &handle);
    if (res != VK_SUCCESS) {
        throw std::runtime_error("Failed to create shader module, error " + to_string(res));
    }

    return make_unique<ShaderModule>(handle, device);
}


/////////////////////////////////////////////////
/////// ShaderStageInfoBuilder
/////////////////////////////////////////////////

const string ShaderStageInfoBuilder::kDefaultEntryPoint = "main";

ShaderStageInfoBuilder::ShaderStageInfoBuilder()
{
    Reset();
}

ShaderStageInfoBuilder & ShaderStageInfoBuilder::Stage(VkShaderStageFlagBits flag)
{
    stage = flag;
    return *this;
}

ShaderStageInfoBuilder & ShaderStageInfoBuilder::EntryPoint(const char * name)
{
    entryPoint = name;
    return *this;
}

ShaderStageInfoBuilder & ShaderStageInfoBuilder::Module(VkShaderModule shaderModule)
{
    this->shaderModule = shaderModule;
    return *this;
}

ShaderStageInfoBuilder & ShaderStageInfoBuilder::Reset()
{
    stage = VK_SHADER_STAGE_VERTEX_BIT;
    entryPoint = kDefaultEntryPoint;
    shaderModule = VK_NULL_HANDLE;
    return *this;
}

VkPipelineShaderStageCreateInfo ShaderStageInfoBuilder::Build() const
{
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = stage;
    shaderStageInfo.module = shaderModule;
    shaderStageInfo.pName = entryPoint.c_str();

    return shaderStageInfo;
}

