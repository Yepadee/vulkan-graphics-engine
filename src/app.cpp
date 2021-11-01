#include "app.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace yge {

struct SimplePushConstantData
{
  glm::mat2 transform{1.f}; // 2x2 Identity
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

App::App() {
  loadGameObjects();
  createPipelineLayout();
  createPipeline();
}

App::~App() { vkDestroyPipelineLayout(ygeDevice.device(), pipelineLayout, nullptr); }

void App::run() {
  while (!ygeWindow.shouldClose()) {
    glfwPollEvents();

    if (auto commandBuffer = renderer.beginFrame())
    {
      renderer.beginSwapChainRenderPass(commandBuffer);
      renderGameObjects(commandBuffer);
      renderer.endSwapChainRenderPass(commandBuffer);
      renderer.endFrame();
    }
  }

  vkDeviceWaitIdle(ygeDevice.device());
}

void App::loadGameObjects() {
  std::vector<Model::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
  auto ygeModel = std::make_shared<Model>(ygeDevice, vertices);
  auto triangle = GameObject::createGameObject();
  triangle.model = ygeModel;
  triangle.color = {.1f, .8f, .1f};
  triangle.transform2d.translation.x = .2f;
  triangle.transform2d.scale = {2.f, .5f};
  triangle.transform2d.rotation = .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

void App::createPipelineLayout() {

  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
  if (vkCreatePipelineLayout(ygeDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void App::createPipeline() {
  assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

  PipelineConfigInfo pipelineConfig{};
  Pipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = renderer.getSwapChainRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  ygePipeline = std::make_unique<Pipeline>(
      ygeDevice,
      "shaders/shader.vert.spv",
      "shaders/shader.frag.spv",
      pipelineConfig);
}


void App::renderGameObjects(VkCommandBuffer commandBuffer) {
  ygePipeline->bind(commandBuffer);

  for (auto& obj: gameObjects)
  {
    obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + 0.0001f, glm::two_pi<float>());
    SimplePushConstantData push{};
    push.offset = obj.transform2d.translation;
    push.color = obj.color;
    push.transform = obj.transform2d.mat2();

    vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);

    obj.model->bind(commandBuffer);
    obj.model->draw(commandBuffer);

  }
}

}  // namespace yge
