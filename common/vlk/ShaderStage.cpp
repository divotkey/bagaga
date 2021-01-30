// Experimental Vulkan-Integration (EVI)
// Copyright 2020, 2021 Roman Divotkey. All rights reserved.

// C++ Standard Library includes
#include <stdexcept>
#include <cassert>

// Local includes
#include "LogicalDevice.h"
#include "ShaderStage.h"

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
    if (codeSize % 4 != 0) {
        throw invalid_argument("Code size for shader module must be a multiple of four, got " 
            + to_string(codeSize));
    }
    code.insert(code.end(), shaderCode, shaderCode + codeSize);
    return *this;
}

ShaderModuleBuilder & ShaderModuleBuilder::Code(const std::vector<unsigned char> & shaderCode)
{
    return Code(shaderCode.data(), shaderCode.size());
}

ShaderModuleBuilder & ShaderModuleBuilder::Reset()
{
    code.clear();
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

    return unique_ptr<ShaderModule>(new ShaderModule(handle, device));
}

/////////////////////////////////////////////////
/////// ShaderStageInfo
/////////////////////////////////////////////////

ShaderStageInfo::ShaderStageInfo(
        VkPipelineShaderStageCreateInfo shaderStage, 
        std::shared_ptr<ShaderModule> shaderModule, 
        const std::string & entryPoint)
    : shaderStage(shaderStage), shaderModule(shaderModule), entryPoint(entryPoint)
{
    UpdateAndValidate();
}

ShaderStageInfo::ShaderStageInfo(const ShaderStageInfo & o)
    : shaderStage(o.shaderStage), shaderModule(o.shaderModule), entryPoint(o.entryPoint)
{
    UpdateAndValidate();
}

ShaderStageInfo & ShaderStageInfo::operator= (const ShaderStageInfo & rhs)
{
    shaderStage = rhs.shaderStage;
    shaderModule = rhs.shaderModule;
    entryPoint = rhs.entryPoint;
    UpdateAndValidate();
    return *this;
}

void ShaderStageInfo::UpdateAndValidate()
{
    shaderStage.pName = entryPoint.c_str();
    shaderStage.module = *shaderModule;
}

/////////////////////////////////////////////////
/////// ShaderStageInfoBuilder
/////////////////////////////////////////////////

const string ShaderStageBuilder::kDefaultEntryPoint = "main";

ShaderStageBuilder::ShaderStageBuilder()
{
    Reset();
}

ShaderStageBuilder & ShaderStageBuilder::CreateFlags(VkPipelineShaderStageCreateFlags flags)
{
    createFlags = flags;
    return *this;
}

ShaderStageBuilder & ShaderStageBuilder::Stage(VkShaderStageFlagBits flag)
{
    stage = flag;
    return *this;
}

ShaderStageBuilder & ShaderStageBuilder::EntryPoint(const char * name)
{
    entryPoint = name;
    return *this;
}

ShaderStageBuilder & ShaderStageBuilder::Module(std::shared_ptr<ShaderModule> shaderModule)
{
    this->shaderModule = shaderModule;
    return *this;
}

ShaderStageBuilder & ShaderStageBuilder::Reset()
{
    stage = VK_SHADER_STAGE_VERTEX_BIT;
    entryPoint = kDefaultEntryPoint;
    shaderModule = nullptr;
    createFlags = 0;
    return *this;
}

ShaderStageInfo ShaderStageBuilder::Build() const
{
    if (!shaderModule) {
        throw logic_error("Unable to build shader stage info, no shader module specified");
    }

    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.flags = createFlags;
    shaderStageInfo.stage = stage;
    shaderStageInfo.pSpecializationInfo = nullptr;

    return ShaderStageInfo(shaderStageInfo, shaderModule, entryPoint);
}

