#pragma once

#include <device.hpp>

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <vector>

namespace yge
{
    class Model
    {
    public:

        struct Vertex {
            glm::vec2 position;
            glm::vec3 color;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        Model(Device& device, const std::vector<Vertex>& vertices);
        ~Model();

        Model(const Model &) = delete;
        Model &operator=(const Model &) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

    private:
        void createVertexBuffers(const std::vector<Vertex>& vertices);
        Device &device_;
        VkBuffer vertexBuffer_;
        VkDeviceMemory vertexBufferMemory_;
        uint32_t vertexCount_;
    };
}