#include <hazel/platform/linux/window.hpp>

namespace hazel
{
    static bool is_glfw_initialized = false;

    Window *Window::create(const WindowProps &props)
    {
        return new hazel::platform::linux::Window(props);
    }
} // namespace hazel

namespace hazel::platform::linux
{
    static void glfw_error_callback(int error_code, const char *description)
    {
        HAZEL_DEV_LOG_ERROR("GLFW error: {} (error code {})", description, error_code);
    }

    Window::Window(const WindowProps &props)
    {
        this->init(props);
    }

    Window::~Window()
    {
        this->shutdown();
    }

    void Window::init(const hazel::WindowProps &props)
    {
        // Since input are defined on different namespace, we cannot
        // inline its value
        hazel::Input::set_instance(new Input());

        this->window_data.title = props.title;
        this->window_data.width = props.width;
        this->window_data.height = props.height;

        HAZEL_DEV_LOG_INFO("Creating window \"{}\" ({} x {})", props.title, props.width, props.height);

        if (!is_glfw_initialized)
        {
            int success = glfwInit();
            assert(success == GLFW_TRUE);
            glfwSetErrorCallback(glfw_error_callback);
            is_glfw_initialized = true;
        }

        this->window = glfwCreateWindow(props.width, props.height, props.title.c_str(), nullptr, nullptr);
        assert(this->window != nullptr);
        glfwMakeContextCurrent(this->window);
        glfwSetWindowUserPointer(this->window, &this->window_data);
        this->set_vsync(true);

        assert(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

        // GLFW callbacks
        glfwSetWindowSizeCallback(this->window, [](GLFWwindow *window, int width, int height) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            data.width = width;
            data.height = height;

            hazel::event::WindowResizeEvent e(width, height);
            data.event_callback(e);
        });

        glfwSetWindowCloseCallback(this->window, [](GLFWwindow *window) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            hazel::event::WindowCloseEvent e;
            data.event_callback(e);
        });

        glfwSetKeyCallback(this->window, [](GLFWwindow *window, int key, int, int action, int) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                hazel::event::KeyPressEvent e(key, 0);
                data.event_callback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                hazel::event::KeyReleaseEvent e(key);
                data.event_callback(e);
                break;
            }
            case GLFW_REPEAT:
            {
                hazel::event::KeyPressEvent e(key, 1);
                data.event_callback(e);
                break;
            }
            }
        });

        glfwSetCharCallback(this->window, [](GLFWwindow *window, unsigned int codepoint) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            hazel::event::KeyTypeEvent e(codepoint);
            data.event_callback(e);
        });

        glfwSetMouseButtonCallback(this->window, [](GLFWwindow *window, int button, int action, int) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            switch (action)
            {
            case GLFW_PRESS:
            {
                hazel::event::MouseButtonPressEvent e(button);
                data.event_callback(e);
                break;
            }
            case GLFW_RELEASE:
            {
                hazel::event::MouseButtonReleaseEvent e(button);
                data.event_callback(e);
                break;
            }
            }
        });

        glfwSetScrollCallback(this->window, [](GLFWwindow *window, double xoffset, double yoffset) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            hazel::event::MouseScrollEvent e(xoffset, yoffset);
            data.event_callback(e);
        });

        glfwSetCursorPosCallback(this->window, [](GLFWwindow *window, double xpos, double ypos) {
            WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
            hazel::event::MouseMoveEvent e(xpos, ypos);
            data.event_callback(e);
        });
    }

    void Window::shutdown()
    {
        glfwDestroyWindow(this->window);
    }

    void Window::on_update()
    {
        glfwPollEvents();
        glfwSwapBuffers(this->window);
    }

    void Window::set_vsync(bool enabled)
    {
        enabled ? glfwSwapInterval(1) : glfwSwapInterval(0);
        this->window_data.vsync = enabled;
    }

    bool Window::get_vsync() const
    {
        return this->window_data.vsync;
    }

    unsigned short Window::get_width() const
    {
        return this->window_data.width;
    }

    unsigned short Window::get_height() const
    {
        return this->window_data.height;
    }

    void Window::set_event_callback(const EventCallbackFn &callback)
    {
        this->window_data.event_callback = callback;
    }
} // namespace hazel::platform::linux
