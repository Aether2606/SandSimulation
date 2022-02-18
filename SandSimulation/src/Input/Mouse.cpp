#include "Input/Mouse.h"

namespace Fidelia
{
	vec2 Mouse::m_Position = vec2(0.0f);
	bool Mouse::m_RightButton = false;
	bool Mouse::m_LeftButton = false;

	void Mouse::SetPosition(double xpos, double ypos)
	{
		m_Position = vec2(xpos, ypos);
	}

	void Mouse::SetMouseRightButton(bool pressed)
	{
		m_RightButton = pressed;
	}

	void Mouse::SetMouseLeftButton(bool pressed)
	{
		m_LeftButton = pressed;
	}
}