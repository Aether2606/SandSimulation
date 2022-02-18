#include "Core/Application.h"
#include "Graphics/Renderer.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 1000

namespace Fidelia
{
	void Application::Init()
	{
		WindowProps props;
		props.title = "Sand Simulation";
		props.width = WINDOW_WIDTH;
		props.height = WINDOW_HEIGHT;
		m_Window.Init(props);

		Renderer::Init(WINDOW_WIDTH, WINDOW_HEIGHT);
		m_Simulation.Init(WINDOW_WIDTH, WINDOW_HEIGHT, Renderer::GetTexture());
	}

	void Application::Run()
	{
		while (!m_Window.ShouldClose())
		{
			m_Window.OnUpdate();
			m_Simulation.OnUpdate();

			Renderer::Clear();
			Renderer::Render();
		}
	}

	void Application::Shutdown()
	{
		m_Simulation.Shutdown();
		Renderer::Shutdown();
		m_Window.Shutdow();
	}
}
