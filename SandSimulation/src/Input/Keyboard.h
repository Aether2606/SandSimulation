#pragma once
#include "Input/KeyCodes.h"

namespace Fidelia
{
	class Keyboard
	{
	public:
		inline static bool IsKeyPressed(KeyCode key) { return s_Pressed[static_cast<u16>(key)]; }
	private:
		friend class Window;
		inline static void SetKey(KeyCode key, bool pressed) { s_Pressed[key] = pressed; }
	private:
		static bool s_Pressed[1024];
	};
}
