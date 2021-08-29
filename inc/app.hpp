#pragma once

#include <window.hpp>
#include <pipeline.hpp>
#include <device.hpp>
#include <swap_chain.hpp>

// std
#include <memory>
#include <vector>

namespace yge
{

    class App
    {
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;

        App();
        ~App();

        App(const App &) = delete;
        App &operator=(const App &) = delete;

        void run();

    private:
        void createPipelineLayout();
        void createPipeline();
        void createCommandBuffers();
        void drawFrame();

        Window window_{WIDTH, HEIGHT, "Test app"};
        Device device_{window_};
        SwapChain swapChain_{device_, window_.getExtent()};
        std::unique_ptr<Pipeline> pipeline_;
        VkPipelineLayout pipelineLayout_;
        std::vector<VkCommandBuffer> commandBuffers_;
    };

}