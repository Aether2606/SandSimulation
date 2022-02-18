#include "Shader.h"
#include <fstream>

namespace Fidelia
{
	void Shader::Compile(const std::string& path)
	{
		ShaderFile sources = ReadFile(path);

		u32 vertex = glCreateShader(GL_VERTEX_SHADER);
		u32 fragment = glCreateShader(GL_FRAGMENT_SHADER);

		const char* vs = sources[GL_VERTEX_SHADER].c_str();
		const char* fs = sources[GL_FRAGMENT_SHADER].c_str();

		glShaderSource(vertex, 1, &vs, nullptr);
		glShaderSource(fragment, 1, &fs, nullptr);

		glCompileShader(vertex);
		glCompileShader(fragment);
		CheckErrors(vertex, "VERTEX");
		CheckErrors(fragment, "FRAGMENT");

		m_ID = glCreateProgram();

		glAttachShader(m_ID, vertex);
		glAttachShader(m_ID, fragment);
		glLinkProgram(m_ID);

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void Shader::Use()
	{
		glUseProgram(m_ID);
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
	}

	ShaderFile Shader::ReadFile(const std::string& path)
	{
		enum class ShaderType
		{
			NONE = -1, VERTEX = 0, FRAGMENT = 1, GEOMETRY = 2
		} type = ShaderType::NONE;

		ShaderFile shaderSources;
		std::ifstream file(path);

		if (file)
		{
			std::string line;
			std::stringstream ss[3];
			while (std::getline(file, line))
			{
				if (line.find("#shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos) type = ShaderType::VERTEX;
					else if (line.find("fragment") != std::string::npos) type = ShaderType::FRAGMENT;
					else if (line.find("geometry") != std::string::npos) type = ShaderType::GEOMETRY;
				}
				else
				{
					ss[(int)type] << line << '\n';
				}
			}
			file.close();

			shaderSources[GL_VERTEX_SHADER] = ss[(int)ShaderType::VERTEX].str();
			shaderSources[GL_FRAGMENT_SHADER] = ss[(int)ShaderType::FRAGMENT].str();
			shaderSources[GL_GEOMETRY_SHADER] = ss[(int)ShaderType::GEOMETRY].str();
		}

		return shaderSources;
	}

	void Shader::CheckErrors(const unsigned int& shader, const char* name)
	{
		int status = GL_TRUE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			int loglen = 0;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &loglen);
			std::vector<char> log(loglen);
			GLsizei written = 0;
			glGetShaderInfoLog(shader, loglen, &written, log.data());
			std::cout << "Failed to compile " << name << "shader: " << log.data() << std::endl;
		}
	}
}