#pragma once

#include "device.hpp"
#include "game_object.hpp"
#include "pipeline.hpp"
#include "window.hpp"
#include "renderer.hpp"

// std
#include <memory>
#include <vector>

namespace yge {
class App {
 public:
  static constexpr int WIDTH = 800;
  static constexpr int HEIGHT = 600;

  App();
  ~App();

  App(const App &) = delete;
  App &operator=(const App &) = delete;

  void run();

 private:
  void loadGameObjects();
  void createPipelineLayout();
  void createPipeline();
  void renderGameObjects(VkCommandBuffer commandBuffer);

  Window ygeWindow{WIDTH, HEIGHT, "Vulkan Tutorial"};
  Device ygeDevice{ygeWindow};
  Renderer renderer{ygeWindow, ygeDevice};

  std::unique_ptr<Pipeline> ygePipeline;
  VkPipelineLayout pipelineLayout;
  std::vector<GameObject> gameObjects;
};
}  // namespace yge
