#include <app.hpp>
#include <device.hpp>

#include <stdexcept>
#include <array>

namespace yge
{
    App::App()
    {
        loadModels();
        createPipelineLayout();
        createPipeline();
        createCommandBuffers();
    }

    App::~App()
    {
        vkDestroyPipelineLayout(device_.device(), pipelineLayout_, nullptr);
    }

    void App::loadModels()
    {
        std::vector<Model::Vertex> vertices {
            {{0.0f, -0.5f}},
            {{0.5f, 0.5f}},
            {{-0.5f, 0.5f}}
        };

        model_ = std::make_unique<Model>(device_, vertices);
    }

    void App::run()
    {
        while (!window_.shouldClose())
        {
            glfwPollEvents();
            drawFrame();
        }

        vkDeviceWaitIdle(device_.device()); // Wait until resources are no longer in use
    }

    void App::createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(device_.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void App::createPipeline()
    {
        auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(swapChain_.width(), swapChain_.height());
        pipelineConfig.renderPass = swapChain_.getRenderPass();
        pipelineConfig.pipelineLayout = pipelineLayout_;
        pipeline_ = std::make_unique<Pipeline>(
            device_,
            "shaders/shader.vert.spv",
            "shaders/shader.frag.spv",
            pipelineConfig
        );
    }

    void App::createCommandBuffers()
    {
        commandBuffers_.resize(swapChain_.imageCount());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = device_.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t> (commandBuffers_.size());

        if (vkAllocateCommandBuffers(device_.device(), &allocInfo, commandBuffers_.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < commandBuffers_.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

            if (vkBeginCommandBuffer(commandBuffers_[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("faield to begin recording command for buffer!");
            }

            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = swapChain_.getRenderPass();
            renderPassInfo.framebuffer = swapChain_.getFrameBuffer(i);

            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain_.getSwapChainExtent();

            std::array<VkClearValue, 2> clearValues{};
            clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
            clearValues[1].depthStencil = {1.0f, 0};

            renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
            renderPassInfo.pClearValues = clearValues.data();

            vkCmdBeginRenderPass(commandBuffers_[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            pipeline_->bind(commandBuffers_[i]);
            model_->bind(commandBuffers_[i]);
            model_->draw(commandBuffers_[i]);

            vkCmdEndRenderPass(commandBuffers_[i]);
            if (vkEndCommandBuffer(commandBuffers_[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to record command buffer!");
            }
        }   
    }

    void App::drawFrame()
    {
        uint32_t imageIndex;
        auto result = swapChain_.acquireNextImage(&imageIndex);

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        result = swapChain_.submitCommandBuffers(&commandBuffers_[imageIndex], &imageIndex);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }
    }

} // namespace yge
