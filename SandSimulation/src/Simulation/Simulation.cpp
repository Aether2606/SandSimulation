#include "Simulation/Simulation.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"

namespace Fidelia
{
	void Simulation::Init(const u32 width, const u32 height, Texture texture)
	{
		if (width == 0 || height == 0)
		{
			assert("width or height cannot be zero!");
			return;
		}

		m_Width = width;
		m_Height = height;
		m_Texture = texture;

		m_MaxParticleCount = m_Width * m_Height;

		m_Particles = new Particle[m_MaxParticleCount];

		ClearScreen();

		m_NeighborPositions[0] = vec2( 0,-1);
		m_NeighborPositions[1] = vec2( 0, 1);
		m_NeighborPositions[2] = vec2( 1, 0);
		m_NeighborPositions[3] = vec2(-1, 0);
		m_NeighborPositions[4] = vec2(-1, 1);
		m_NeighborPositions[5] = vec2( 1, 1);
		m_NeighborPositions[6] = vec2(-1,-1);
		m_NeighborPositions[7] = vec2( 1,-1);
	}

	void Simulation::Shutdown()
	{
		delete m_Particles;
	}

	void Simulation::OnUpdate()
	{
		PlaceParticles();
		Simulate();
		SaveTexture();
	}

	void Simulation::PlaceParticles()
	{
		u32 mouseX = Mouse::GetX();
		u32 mouseY = Mouse::GetY();

		if (Keyboard::IsKeyPressed(Key::D1))
		{
			m_CurrentParticle = m_ParticleList.sand;
		}
		else if (Keyboard::IsKeyPressed(Key::D2))
		{
			m_CurrentParticle = m_ParticleList.water;
		}
		else if (Keyboard::IsKeyPressed(Key::D3))
		{
			m_CurrentParticle = m_ParticleList.wood;
		}
		else if (Keyboard::IsKeyPressed(Key::D4))
		{
			m_CurrentParticle = m_ParticleList.fire;
		}
		else if (Keyboard::IsKeyPressed(Key::D5))
		{
			m_CurrentParticle = m_ParticleList.smoke;
		}
		else if (Keyboard::IsKeyPressed(Key::D6))
		{
			m_CurrentParticle = m_ParticleList.oil;
		}
		else if (Keyboard::IsKeyPressed(Key::D7))
		{
			m_CurrentParticle = m_ParticleList.acid;
		}
		else if (Keyboard::IsKeyPressed(Key::C))
		{
			ClearScreen();
		}

		if (Mouse::GetLeftButton())
		{
			u32 halfRadius = m_BrushSize / 2;

			if (InBounds(mouseX, mouseY))
			{				
				for (u32 y = 0; y < m_Height; ++y)
				{
					for (u32 x = 0; x < m_Width; ++x)
					{
						if (x < mouseX + halfRadius && x > mouseX - halfRadius
							&& y < mouseY + halfRadius && y > mouseY - halfRadius)
						{
							size_t position = PositionToIndex(x, y);
							if (InBounds(x, y))
							{
								if(m_Particles[position].id == PARTICLE_AIR) m_Particles[position] = m_CurrentParticle;
							}
						}
					}
				}
			}
		}
		else if (Mouse::GetRightButton())
		{
			u32 halfRadius = m_BrushSize / 2;

			if (InBounds(mouseX, mouseY))
			{
				for (u32 y = 0; y < m_Height; ++y)
				{
					for (u32 x = 0; x < m_Width; ++x)
					{
						if (x < mouseX + halfRadius && x > mouseX - halfRadius
							&& y < mouseY + halfRadius && y > mouseY - halfRadius)
						{
							size_t position = PositionToIndex(x, y);
							if (InBounds(x, y))
							{
								m_Particles[position] = m_ParticleList.air;
								m_ToNotify.push_back(vec2(x, y));
							}
						}
					}
				}
			}
		}
	}

	void Simulation::Simulate()
	{
		for (u32 y = 0; y < m_Height; ++y)
		{
			for (u32 x = 0; x < m_Width; ++x)
			{
				auto position = PositionToIndex(x, y);

				Particle& particle = m_Particles[position];
				if (!particle.dirty) continue;
				particle.dirty = false;

				if (particle.id == PARTICLE_AIR) continue;

				if (particle.lifeTime <= 0)
				{
					particle = m_ParticleList.air;
					m_ToNotify.push_back(vec2(x, y));
					continue;
				}

				if (particle.id == PARTICLE_SAND)
				{
					std::array<vec2, 3> desiredPositions;
					desiredPositions[0] = vec2(x, y - m_Gravity);
					desiredPositions[1] = vec2(x - 1, y - m_Gravity);
					desiredPositions[2] = vec2(x + 1, y - m_Gravity);

					for (const auto& it : desiredPositions)
					{
						if (UpdateSand(vec2(x, y), it, particle)) break;
					}
				}
				else if (particle.id == PARTICLE_WATER)
				{
					std::array<vec2, 5> desiredPositions;
					desiredPositions[0] = vec2(x, y - m_Gravity);
					desiredPositions[1] = vec2(x + m_Gravity, y - m_Gravity);
					desiredPositions[2] = vec2(x - m_Gravity, y - m_Gravity);
					desiredPositions[3] = vec2(x + m_Gravity, y);
					desiredPositions[4] = vec2(x - m_Gravity, y);

					for (const auto& it : desiredPositions)
					{
						if (UpdateWater(vec2(x, y), it, particle)) break;
					}
				}
				else if (particle.id == PARTICLE_FIRE)
				{
					particle.lifeTime--;
					particle.dirty = true;

					vec2 smokePos = vec2(x, y + 1);
					if (InBounds(smokePos))
					{
						size_t index = PositionToIndex(smokePos);
						if (m_Particles[index].id == PARTICLE_AIR)
						{
							if(rand() % 1000 < 10) m_Particles[index] = m_ParticleList.smoke;
						}
					}

					for (const auto& it : m_NeighborPositions)
					{
						if (UpdateFire(vec2(x, y), vec2(x + it.x, y + it.y), particle)) break;
					}
				}
				else if (particle.id == PARTICLE_SMOKE)
				{
					particle.lifeTime--;

					std::array<vec2, 3> desiredPositions;
					desiredPositions[0] = vec2(x, y + m_Gravity);
					desiredPositions[1] = vec2(x - 1, y + m_Gravity);
					desiredPositions[2] = vec2(x + 1, y + m_Gravity);

					for (const auto& it : desiredPositions)
					{
						if (UpdateSmoke(vec2(x, y), it, particle)) break;
					}
				}
				else if (particle.id == PARTICLE_OIL)
				{
					std::array<vec2, 5> desiredPositions;
					desiredPositions[0] = vec2(x, y - m_Gravity);
					desiredPositions[1] = vec2(x - m_Gravity, y - m_Gravity);
					desiredPositions[2] = vec2(x + m_Gravity, y - m_Gravity);
					desiredPositions[3] = vec2(x - m_Gravity, y);
					desiredPositions[4] = vec2(x + m_Gravity, y);

					for (const auto& it : desiredPositions)
					{
						if (UpdateOil(vec2(x, y), it, particle)) break;
					}
				}
				else if (particle.id == PARTICLE_ACID)
				{
					std::array<vec2, 5> desiredPositions;
					desiredPositions[0] = vec2(x, y - m_Gravity);
					desiredPositions[1] = vec2(x + m_Gravity, y - m_Gravity);
					desiredPositions[2] = vec2(x - m_Gravity, y - m_Gravity);
					desiredPositions[3] = vec2(x + m_Gravity, y);
					desiredPositions[4] = vec2(x - m_Gravity, y);

					for (const auto& it : desiredPositions)
					{
						if (UpdateAcid(vec2(x, y), it, particle)) break;
					}
				}
			}
		}

		for (const auto& it : m_ToNotify)
		{
			NotifyNeighbors(it);
		}
		m_ToNotify.clear();
	}

	void Simulation::SaveTexture()
	{
		auto data = m_Texture.GetData();

		size_t colorBitIt = 0;

		for (u32 y = 0; y < m_Height; ++y)
		{
			for (u32 x = 0; x < m_Width; ++x)
			{
				u32 pixelPosition = y * m_Height + x;

				ParticleColor color = m_Particles[pixelPosition].color;

				data[pixelPosition + colorBitIt] = static_cast<BYTE>(color.red);
				++colorBitIt;

				data[pixelPosition + colorBitIt] = static_cast<BYTE>(color.green);
				++colorBitIt;

				data[pixelPosition + colorBitIt] = static_cast<BYTE>(color.blue);
			}
		}

		m_Texture.SetData(data);
	}

	void Simulation::NotifyNeighbors(const vec2 position)
	{
		m_Particles[PositionToIndex(position)].dirty = true;
		for (const auto& it : m_NeighborPositions)
		{
			size_t index = PositionToIndex(position + it);
			if (InBounds(position + it))
			{
				m_Particles[index].dirty = true;
			}
		}
	}

	void Simulation::ClearScreen()
	{
		for (u32 y = 0; y < m_Height; ++y)
		{
			for (u32 x = 0; x < m_Width; ++x)
			{
				auto pos = PositionToIndex(x, y);
				m_Particles[pos] = m_ParticleList.air;
			}
		}
	}

	bool Simulation::UpdateSand(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if (!(id == PARTICLE_AIR || id == PARTICLE_WATER || id == PARTICLE_ACID || id == PARTICLE_OIL))
		{
			if (to.y < from.y -1)
			{
				to.y += 1;
				return UpdateSand(from, to, particle);
			}
		}
		else
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[fromIndex] = m_Particles[toIndex];
			m_Particles[toIndex] = particle;
			m_ToNotify.push_back(from);
			m_ToNotify.push_back(to);
			return true;
		}
		return false;
	}

	bool Simulation::UpdateWater(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if (id != PARTICLE_AIR && id != PARTICLE_FIRE)
		{
			if (to.y < from.y - 1)
			{
				to.y += 1;
				return UpdateWater(from, to, particle);
			}
			else
			{
				if (to.x > from.x + 1)
				{
					to.x -= 1;
					return UpdateWater(from, to, particle);
				}
				else if (to.x < from.x - 1)
				{
					to.x += 1;
					return UpdateWater(from, to, particle);
				}
			}
		}
		else if(id == PARTICLE_FIRE)
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[toIndex] = m_ParticleList.smoke;
			m_Particles[fromIndex] = m_ParticleList.air;
			m_ToNotify.push_back(to);
			return true;
		}
		else
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[fromIndex] = m_Particles[toIndex];
			m_Particles[toIndex] = particle;
			m_ToNotify.push_back(to);
			m_ToNotify.push_back(from);
			return true;
		}
		return false;
	}

	bool Simulation::UpdateFire(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if ((id == PARTICLE_WOOD || id == PARTICLE_SAND || id == PARTICLE_OIL))
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			if (rand() % 100 < m_FireSpreadChance)
			{
				m_Particles[toIndex] = particle;
				m_Particles[toIndex].lifeTime = FIRE_LIFETIME;
				m_ToNotify.push_back(from);
				m_ToNotify.push_back(to);
			}
			return true;
		}
		return false;
	}

	bool Simulation::UpdateSmoke(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if (!(id == PARTICLE_AIR || id == PARTICLE_WATER || id == PARTICLE_ACID))
		{
			if (to.y > from.y + 1)
			{
				to.y -= 1;
				return UpdateSmoke(from, to, particle);
			}
		}
		else
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[fromIndex] = m_Particles[toIndex];
			m_Particles[toIndex] = particle;
			m_ToNotify.push_back(from);
			m_ToNotify.push_back(to);
			return true;
		}
		return false;
	}

	bool Simulation::UpdateOil(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if (id != PARTICLE_AIR && id != PARTICLE_WATER && id != PARTICLE_ACID)
		{
			if (to.y < from.y - 1)
			{
				to.y += 1;
				return UpdateOil(from, to, particle);
			}
			else
			{
				if (to.x > from.x + 1)
				{
					to.x -= 1;
					return UpdateOil(from, to, particle);
				}
				else if (to.x < from.x - 1)
				{
					to.x += 1;
					return UpdateOil(from, to, particle);
				}
			}
		}
		else
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[fromIndex] = m_Particles[toIndex];
			m_Particles[toIndex] = particle;
			m_ToNotify.push_back(to);
			m_ToNotify.push_back(from);
			return true;
		}
		return false;
	}

	bool Simulation::UpdateAcid(const vec2 from, vec2 to, const Particle particle)
	{
		if (!InBounds(to))
		{
			if (to.x == INVALID_POS) to.x = 0;
			else if (to.x < 0) to.x = 0;
			else if (to.x >= m_Width) to.x = float(m_Width - 1);

			if (to.y == INVALID_POS) to.y = 0;
			else if (to.y <= 0) to.y = 0;
			else if (to.y >= m_Height) to.y = float(m_Height - 1);
		}

		auto id = m_Particles[PositionToIndex(to)].id;

		if (id != PARTICLE_AIR && id != PARTICLE_FIRE && id != PARTICLE_WOOD && id != PARTICLE_SAND && id != PARTICLE_WATER)
		{
			if (to.y < from.y - 1)
			{
				to.y += 1;
				return UpdateAcid(from, to, particle);
			}
			else
			{
				if (to.x > from.x + 1)
				{
					to.x -= 1;
					return UpdateAcid(from, to, particle);
				}
				else if (to.x < from.x - 1)
				{
					to.x += 1;
					return UpdateAcid(from, to, particle);
				}
			}
		}
		else if (id == PARTICLE_WOOD || id == PARTICLE_SAND)
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			if (rand() % 100 < m_AcidCorrodeChance)
			{
				m_Particles[fromIndex] = m_ParticleList.smoke;
				m_Particles[toIndex] = particle;
			}
			m_ToNotify.push_back(to);
			m_ToNotify.push_back(from);
			return true;
		}
		else if (id == PARTICLE_FIRE)
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[toIndex] = m_ParticleList.smoke;
			m_Particles[fromIndex] = m_ParticleList.air;
			m_ToNotify.push_back(to);
			return true;
		}
		else 
		{
			size_t fromIndex = PositionToIndex(from);
			size_t toIndex = PositionToIndex(to);
			m_Particles[fromIndex] = m_Particles[toIndex];
			m_Particles[toIndex] = particle;
			m_ToNotify.push_back(to);
			m_ToNotify.push_back(from);
			return true;
		}
		return false;
	}
}