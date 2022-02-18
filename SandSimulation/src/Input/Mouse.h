#pragma once
#include "Core/Base.h"

namespace Fidelia
{
	class Mouse
	{
	public:
		inline static const u32 GetX() noexcept { return static_cast<u32>(m_Position.x); }
		inline static const u32 GetY() noexcept { return static_cast<u32>(m_Position.y); }

		inline static const bool GetRightButton() noexcept { return m_RightButton; }
		inline static const bool GetLeftButton() noexcept { return m_LeftButton; }
	private:
		friend class Window;
		static void SetPosition(double xpos, double ypos);
		static void SetMouseRightButton(bool pressed);
		static void SetMouseLeftButton(bool pressed);
	private:
		static vec2 m_Position;
		static bool m_RightButton;
		static bool m_LeftButton;
	};
}