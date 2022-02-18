#pragma once
#include "Core/Base.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace Fidelia
{
	class Texture
	{
	public:
		void Create(u32 width, u32 height);
		void Use() const;
		void Destroy();

		void SetData(BYTE* data);

		inline u32 GetWidth() const { return m_Width; }
		inline u32 GetHeight() const { return m_Height; }
		inline u8 GetChannels() const { return m_Channels; }

		inline BYTE* GetData() const { return m_ImageData; }
	private:
		u32 m_InternalFormat = GL_RGB;
		u32 m_ImageFormat = GL_RGB;
		u32 m_WrapS = GL_REPEAT;
		u32 m_WrapT = GL_REPEAT;
		u32 m_FilterMax = GL_NEAREST;
		u32 m_FilterMin = GL_NEAREST;

		u32 m_Width = 0, m_Height = 0;
		u8 m_Channels = 3;
		u32 m_ID = 0;
		BYTE* m_ImageData = nullptr;
	};
}
