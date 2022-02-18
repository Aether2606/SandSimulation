#pragma once
#include "Core/Base.h"

#define GLEW_STATIC
#include <GL/glew.h>

#pragma warning(push, 0)
#include <GLFW/glfw3.h>
#pragma warning(pop)

namespace Fidelia
{
	struct WindowProps
	{
		std::string title = "Sand Simulation";
		u32 width = 1000;
		u32 height = 1000;
		bool maximized = false;
		bool resizable = false;
	};

	class Window
	{
	public:
		void Init(const WindowProps& props = WindowProps());
		void Shutdow();
		void OnUpdate();

		inline const bool ShouldClose() const { return glfwWindowShouldClose(m_pWindow); }
	private:
	private:
		GLFWwindow* m_pWindow = nullptr;
		GLFWmonitor* m_pMonitor = nullptr;
		const GLFWvidmode* m_pVideoMode = nullptr;

		struct WindowData
		{
			std::string title;
			u32 width, height;
			bool vSync;
		} m_Data;
	};
}
