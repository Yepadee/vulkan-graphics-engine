#include <window.hpp>

#include <stdexcept>

namespace yge
{

    Window::Window(int width, int height, std::string windowName)
        : width_(width), height_(height), windowName_(windowName)
    {
        initWindow();
    }

    Window::~Window()
    {
        glfwDestroyWindow(window_);
        glfwTerminate();
    }

    void Window::initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window_ = glfwCreateWindow(width_, height_, windowName_.c_str(), nullptr, nullptr);
        //glfwSetFramebufferSizeCallback(window_, framebufferResizeCallback);
    }

    bool Window::shouldClose()
    {
        return glfwWindowShouldClose(window_);
    }

    VkExtent2D Window::getExtent()
    {
        return {
            static_cast<uint32_t>(width_),
            static_cast<uint32_t>(height_)
        };
    }

    void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window_, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

}
