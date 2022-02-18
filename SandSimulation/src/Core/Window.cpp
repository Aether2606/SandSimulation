#include "Core/Window.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace Fidelia
{
	void Window::Init(const WindowProps& props)
	{
		m_Data.title = props.title;
		m_Data.width = props.width;
		m_Data.height = props.height;

		int success = glfwInit();

		glfwWindowHint(GLFW_RESIZABLE, props.resizable ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_MAXIMIZED, props.maximized ? GL_TRUE : GL_FALSE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		m_pMonitor = glfwGetPrimaryMonitor();
		m_pVideoMode = glfwGetVideoMode(m_pMonitor);

		m_pWindow = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_pWindow);

		glewExperimental = true;
		GLenum successGLEW = glewInit();
		if (successGLEW != GLEW_OK) std::cout << "Failed to init GLEW" << std::endl;

		std::cout << "OpenGL Info:" << std::endl;
		std::cout << "\tVendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "\tRenderer: " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "\tVersion: " << glGetString(GL_VERSION) << std::endl;

		glfwSetCursorPosCallback(m_pWindow, [](GLFWwindow* window, double xpos, double ypos) {
			//@note: we invert the mouse position
			int width, height;
			glfwGetWindowSize(window, &width, &height);
			ypos = height / 2 - (ypos - height / 2);

			Mouse::SetPosition(xpos, ypos);
		});

		glfwSetMouseButtonCallback(m_pWindow, [](GLFWwindow* window, int button, int action, int mods) {
			Mouse::SetMouseRightButton(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS);
			Mouse::SetMouseLeftButton(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS);
		});

		glfwSetKeyCallback(m_pWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			Keyboard::SetKey(key, action == GLFW_PRESS ? true : false);
		});
	}

	void Window::Shutdow()
	{
		glfwDestroyWindow(m_pWindow);
		glfwTerminate();
	}

	void Window::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_pWindow);
	}
}