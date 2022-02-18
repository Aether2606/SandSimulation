#pragma once
#include "Core/Base.h"
#include "Graphics/Shader.h"
#include "Graphics/Texture.h"

namespace Fidelia
{
	typedef u32 BufferId;

	struct RendererData
	{
		static constexpr u32 maxQuads = 1;
		static constexpr u32 maxVertices = maxQuads * 4;
		static constexpr u32 maxIndices = maxQuads * 6;
		static constexpr u32 maxTextureSlots = 32;


		BufferId quadVAO;
		BufferId quadVBO;
		BufferId quadIBO;


		Shader shader;
		Texture texture;
	};

	class Renderer
	{
	public:
		static void Init(const u32 width, const u32 height);
		static void Shutdown();

		static void Clear();
		static void Render();

		static Texture GetTexture() { return s_RenderData.texture; }
	private:
		static RendererData s_RenderData;
	};
}
