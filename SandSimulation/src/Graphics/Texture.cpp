#include "Graphics/Texture.h"

namespace Fidelia
{
	void Texture::Create(u32 width, u32 height)
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &this->m_ID);

		m_Width = width;
		m_Height = height;

		size_t dataSize = m_Width * m_Height * m_Channels;

		m_ImageData = new BYTE[dataSize];

		Use();
		glTexImage2D(GL_TEXTURE_2D, 0, m_InternalFormat, m_Width, m_Height, 0, m_ImageFormat, GL_UNSIGNED_BYTE, m_ImageData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_WrapS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_WrapT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_FilterMin);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_FilterMax);
	}

	void Texture::Use() const
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->m_ID);
	}

	void Texture::Destroy()
	{
		glDeleteTextures(1, &this->m_ID);
	}

	void Texture::SetData(BYTE* data)
	{
		m_ImageData = data;
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, m_ImageFormat, GL_UNSIGNED_BYTE, data);
	}
}