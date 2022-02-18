#pragma once
#include "Core/Base.h"
#include "Graphics/Texture.h"

#define PARTICLE_AIR 0
#define PARTICLE_SAND 1
#define PARTICLE_WATER 2
#define PARTICLE_WOOD 3
#define PARTICLE_FIRE 4
#define PARTICLE_SMOKE 5
#define PARTICLE_OIL 6
#define PARTICLE_ACID 7

#define FIRE_LIFETIME 1000
#define SMOKE_LIFETIME 200

#define INVALID_POS UINT_MAX

namespace Fidelia
{
	struct ParticleColor
	{
		u8 red, green, blue;
	};

	//@important! for now size is 74bits
	struct Particle
	{
		u8 id;
		u32 lifeTime;
		ParticleColor color;
		bool dirty;
	};

	struct Particles
	{
		Particle air{ PARTICLE_AIR, UINT_MAX, {0, 0, 0}, false };
		Particle sand{ PARTICLE_SAND, UINT_MAX, {212, 193, 158}, true };
		Particle water{ PARTICLE_WATER, UINT_MAX, {212, 241, 249}, true };
		Particle wood{ PARTICLE_WOOD, UINT_MAX, {186, 140, 99}, true };
		Particle fire{ PARTICLE_FIRE, FIRE_LIFETIME, {247, 55, 24}, true };
		Particle smoke{ PARTICLE_SMOKE, SMOKE_LIFETIME, {132, 136, 132}, true };
		Particle oil{ PARTICLE_OIL, UINT_MAX, {49, 51, 48}, true };
		Particle acid{ PARTICLE_ACID, UINT_MAX, {255, 191, 26}, true };
	};

	class Simulation
	{
	public:
		void Init(const u32 width, const u32 height, Texture texture);
		void Shutdown();

		void OnUpdate();
	private:
		inline bool InBounds(u32 x, u32 y)
		{
			if (y < 0 || y > m_Height - 1) return false;
			if (x < 0 || x > m_Width - 1) return false;
			return true;
		}

		inline bool InBounds(vec2 pos)
		{
			if (pos.y < 0 || pos.y > m_Height - 1) return false;
			if (pos.x < 0 || pos.x > m_Width - 1) return false;
			return true;
		}

		inline size_t PositionToIndex(vec2 pos)
		{
			return pos.y * m_Height + pos.x;
		}

		inline size_t PositionToIndex(u32 x, u32 y) 
		{ 
			return y * m_Height + x; 
		}

		void PlaceParticles();
		void Simulate();
		void NotifyNeighbors(const vec2 position);
		void SaveTexture();

		bool UpdateSand(const vec2 from, vec2 to, const Particle particle);
		bool UpdateWater(const vec2 from, vec2 to, const Particle particle);
		bool UpdateFire(const vec2 from, vec2 to, const Particle particle);
		bool UpdateSmoke(const vec2 from, vec2 to, const Particle particle);
		bool UpdateOil(const vec2 from, vec2 to, const Particle particle);
		bool UpdateAcid(const vec2 from, vec2 to, const Particle particle);
	private:
		const u32 m_BrushSize = 20;
		const u32 m_Gravity = 5;
		const u32 m_FireSpreadChance = 10;
		const u32 m_AcidCorrodeChance = 1;
		std::array<vec2, 8> m_NeighborPositions;
		std::vector<vec2> m_ToNotify;

		u32 m_Width = 0, m_Height = 0;
		u32 m_MaxParticleCount = 0;

		Texture m_Texture;
		Particle* m_Particles;

		Particles m_ParticleList;
		Particle m_CurrentParticle = m_ParticleList.sand;
	};
}