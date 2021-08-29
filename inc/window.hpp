#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace yge
{
    class Window
    {
    public:
        Window(int width, int height, std::string windowName);
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;

        bool shouldClose();
        VkExtent2D getExtent();

        void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
        

    private:
        const int width_;
        const int height_;
        std::string windowName_;

        GLFWwindow *window_;

        void initWindow();
    };
} // namespace yge