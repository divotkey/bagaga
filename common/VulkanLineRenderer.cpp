/*  ____          _____          _____          
 * |  _ \   /\   / ____|   /\   / ____|   /\    
 * | |_) | /  \ | |  __   /  \ | |  __   /  \   
 * |  _ < / /\ \| | |_ | / /\ \| | |_ | / /\ \  
 * | |_) / ____ \ |__| |/ ____ \ |__| |/ ____ \ 
 * |____/_/    \_\_____/_/    \_\_____/_/    \_\
 *
 * Bagaga - Bloody Amazing Game Architecture Game
 * Copyright 2020 Bagaga Development Team. All rights reserved.                                             
 */

// C++ Standard Library includes
#include <cassert>

// AST Utilities includes
#include <IWindowManager.h>

// Local includes.
#include "vlk/shd/frag.h"
#include "vlk/shd/vert2.h"
#include "vlk/IVulkanRenderSystem.h"
#include "vlk/VertexInputInfo.h"
#include "vlk/VertexDescription.h"
#include "vlk/InputAssembly.h"
#include "vlk/Rasterizer.h"
#include "vlk/Multisampling.h"
#include "vlk/ColorBlendState.h"
#include "vlk/PipelineLayout.h"
#include "vlk/Buffer.h"
#include "vlk/Memory.h"
#include "vlk/GraphicsPipeline.h"
#include "vlk/RenderPass.h"
#include "vlk/CommandBuffer.h"
#include "vlk/LogicalDevice.h"
#include "VulkanLineRenderer.h"

using namespace std;
using namespace astu;

VulkanLineRenderer::VulkanLineRenderer(size_t maxLines, int renderPriority)
    : BaseVulkanRenderLayer("Vulkan Line Renderer", renderPriority)
    , drawColor(WebColors::Aqua)
{
    vertices.reserve(maxLines);
}

VulkanLineRenderer::~VulkanLineRenderer()
{
    // Intentionally left empty.
}

void VulkanLineRenderer::OnStartup()
{
    auto & renderSys = GetSM().GetService<IVulkanRenderSystem>();
    CreateGraphicsPipeline(renderSys);
    CreateVertexBuffer(renderSys);
    vertices.clear();

    const auto & wndMng = GetSM().GetService<IWindowManager>();
    transform.SetToIdentity()
        .Scale(2.0f / wndMng.GetWidth(), 2.0f / wndMng.GetHeight())
        .Translate(-1.0f, -1.0f);

    BaseVulkanRenderLayer::OnStartup();
}

void VulkanLineRenderer::OnShutdown()
{
    BaseVulkanRenderLayer::OnShutdown();
    vertexBuffer = nullptr;
    graphicsPipeline = nullptr;
}

void VulkanLineRenderer::CreateGraphicsPipeline(IVulkanRenderSystem & renderSys)
{
    GraphicsPipelineBuilder builder;

    builder.VertexInputState(
        VertexInputInfoBuilder()
        .AddVertexBindingDescription(
            VertexBindingDescriptionBuilder()
                .Binding(0)
                .Stride(sizeof(LineVertex))
                .InputRate(VK_VERTEX_INPUT_RATE_VERTEX)
                .Build())
        .AddVertexAttributeDescription(
            VertexAttributeDescriptionBuilder()
                .Location(0)
                .Binding(0)
                .Format(VK_FORMAT_R32G32_SFLOAT)
                .Offset(offsetof(LineVertex, pos))
                .Build())
        .AddVertexAttributeDescription(
            VertexAttributeDescriptionBuilder()
                .Location(1)
                .Binding(0)
                .Format(VK_FORMAT_R32G32B32_SFLOAT)
                .Offset(offsetof(LineVertex, col))
                .Build())
        .Build());

    builder.InputAssembly(
        InputAssemblyBuilder()
        .Topology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST)
        .PrimitiveRestart(false).Build()
        );

    builder.ViewportState(
        ViewportStateBuilder()
        .AddViewport(
            ViewportBuilder()
            .ChoosePositionAndSize(renderSys.GetSwapChain())
            .Build())
        .Build());

    builder.Rasterizer(
        RasterizerBuilder()
        .EnableDepthClamp(false)
        .EnableRasterizerDiscard(false)
        .PolygonMode(VK_POLYGON_MODE_FILL)
        .LineWidth(1.0f)
        .CullMode(VK_CULL_MODE_BACK_BIT)
        .FrontFace(VK_FRONT_FACE_CLOCKWISE)
        .EnableDepthBias(false)
        .DepthBiasConstantFactor(0.0f)
        .DepthBiasClamp(0.0f)
        .DepthBiasSlopeFactor(0.0f)
        .Build());

    builder.Multisampling(
        MultisamplingBuilder()
        .EnableSampleShading(false)
        .RasterizationSamples(VK_SAMPLE_COUNT_1_BIT)
        .MinSampleShading(1.0f)
        .EnableAlphaToCoverage(false)
        .EnableAlphaToOne(false)
        .Build());

    builder.ColorBlending(
        ColorBlendStateBuilder()
        .EnableLogicOp(false)
        .LogicOp(VK_LOGIC_OP_COPY)
        .AddAttachment(
            BlendAttachmentStateBuilder()
            .EnableBlend(false)
            .SrcColorBlendFactor(VK_BLEND_FACTOR_ONE)
            .DstColorBlendFactor(VK_BLEND_FACTOR_ZERO)
            .ColorBlendOp(VK_BLEND_OP_ADD)
            .SrcAlphaBlendFactor(VK_BLEND_FACTOR_ONE)
            .DstAlphaBlendFactor(VK_BLEND_FACTOR_ZERO)
            .AlphaBlendOp(VK_BLEND_OP_ADD)
            .Build())
        .Build());

    builder.Layout(
        PipelineLayoutBuilder()
        .Build(renderSys.GetLogicalDevice()));

    builder.RenderPass(renderSys.GetRenderPass(), 0);

    builder.AddShaderStage(
        ShaderStageBuilder()
        .Stage(VK_SHADER_STAGE_VERTEX_BIT)
        .EntryPoint("main")
        .Module(
            ShaderModuleBuilder()
            .Code(kVertexShader)
            .Build(renderSys.GetLogicalDevice()))
        .Build());

    builder.AddShaderStage(
        ShaderStageBuilder()
        .Stage(VK_SHADER_STAGE_FRAGMENT_BIT)
        .EntryPoint("main")
        .Module(
            ShaderModuleBuilder()
            .Code(kFragmentShader)
            .Build(renderSys.GetLogicalDevice()))
        .Build());

    builder
        .BasePipeline(renderSys.GetBaseGraphicsPipeline())
        .Flags(VK_PIPELINE_CREATE_DERIVATIVE_BIT);

    graphicsPipeline = builder.Build(renderSys.GetLogicalDevice());
}

void VulkanLineRenderer::CreateVertexBuffer(IVulkanRenderSystem & renderSys)
{
    vertexBuffer = BufferBuilder()
        .Size(sizeof(LineVertex) * vertices.capacity())
        .Usage(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT)
        .SharingMode(VK_SHARING_MODE_EXCLUSIVE)
        // not required because exclusive mode
        .AddQueueFamily(renderSys.GetLogicalDevice()->GetGraphicsQueueIndex())
        .Build(renderSys.GetLogicalDevice());

    const auto memRequirements = vertexBuffer->GetMemoryRequirements();

    std::shared_ptr<Memory> memory = MemoryBuilder()
        .AllocationSize(memRequirements.size)
        .ChooseMemoryTypeIndex(
            renderSys.GetPhysicalDevice(), 
            memRequirements.memoryTypeBits, 
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT 
            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
        .Build(renderSys.GetLogicalDevice());

    vertexBuffer->BindMemory(memory);
}

/////////////////////////////////////////////////
/////// Interface IVulkanRenderLayer
/////////////////////////////////////////////////

void VulkanLineRenderer::Render(CommandBuffer & cmdBuf)
{
    assert(vertices.size() % 2 == 0);

    if (vertices.size() == 0) {
        return;
    }

    // Update vertex data.
    vertexBuffer->GetMemory()->WriteData(
        reinterpret_cast<const unsigned char*>(vertices.data()), 
        sizeof(vertices[0]) * vertices.size());

    cmdBuf.BindPipeline(*graphicsPipeline, VK_PIPELINE_BIND_POINT_GRAPHICS);
    cmdBuf.BindVertexBuffer(*vertexBuffer);
    cmdBuf.Draw(vertices.size(), 1, 0, 0);

    vertices.clear();
}

/////////////////////////////////////////////////
/////// Interface ILineRenderer
/////////////////////////////////////////////////

void VulkanLineRenderer::DrawLine(double x1, double y1, double x2, double y2) 
{
    ;

    vertices.push_back(
        LineVertex(
            transform.TransformPoint(x1, y1),
            static_cast<float>(drawColor.r), 
            static_cast<float>(drawColor.g), 
            static_cast<float>(drawColor.b)
            )
        );

    vertices.push_back(
        LineVertex(
            transform.TransformPoint(x2, y2),
            static_cast<float>(drawColor.r), 
            static_cast<float>(drawColor.g), 
            static_cast<float>(drawColor.b)
            )
        );
}

void VulkanLineRenderer::SetDrawColor(const astu::Color & c)
{
    drawColor = c;
}
