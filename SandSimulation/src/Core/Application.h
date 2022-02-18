#pragma once
#include "Core/Window.h"
#include "Simulation/Simulation.h"

namespace Fidelia
{
	class Application
	{
	public:
		void Init();
		void Run();
		void Shutdown();
	private:
		Window m_Window;
		Simulation m_Simulation;
	};
}