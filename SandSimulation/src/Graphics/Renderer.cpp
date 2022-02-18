#include "Graphics/Renderer.h"

#define SHADER_VERTEX_POSITION 0
#define SHADER_UV_POSITION 1

#define GLEW_STATIC
#include <GL/glew.h>

namespace Fidelia
{
	RendererData Renderer::s_RenderData;

	void Renderer::Init(const u32 width, const u32 height)
	{
		s_RenderData.shader.Compile("assets\\shaders\\default.glsl");
		s_RenderData.texture.Create(width, height);

		//@note: I legit don't remember the command to draw without an IBO so i'll just do it even tho it's completely overkill

		glGenVertexArrays(1, &s_RenderData.quadVAO);
		glGenBuffers(1, &s_RenderData.quadVBO);
		glGenBuffers(1, &s_RenderData.quadIBO);

		float vertices[] = {
			 1.0f,  1.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 3,
			1, 2, 3
		};

		glBindVertexArray(s_RenderData.quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, s_RenderData.quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_RenderData.quadIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(SHADER_VERTEX_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glVertexAttribPointer(SHADER_UV_POSITION, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(SHADER_VERTEX_POSITION);
		glEnableVertexAttribArray(SHADER_UV_POSITION);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	}

	void Renderer::Shutdown()
	{
		glDeleteVertexArrays(1, &s_RenderData.quadVAO);
		glDeleteBuffers(1, &s_RenderData.quadVBO);
		glDeleteBuffers(1, &s_RenderData.quadIBO);
	}

	void Renderer::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::Render()
	{
		s_RenderData.shader.Use();
		s_RenderData.texture.Use();

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}