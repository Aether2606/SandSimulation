#pragma once
#include "Core/Base.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace Fidelia
{
	typedef u32 ShaderId;
	typedef std::unordered_map<GLenum, std::string> ShaderFile;

	class Shader
	{
	public:
		void Compile(const std::string& path);
		void Use();

		void SetInt(const std::string& name, int value) const;
		void SetFloat(const std::string& name, float value) const;
	private:
		ShaderFile ReadFile(const std::string& path);
		void CheckErrors(const unsigned int& shader, const char* name);
	private:
		ShaderId m_ID;
	};
}