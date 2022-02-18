#pragma once
#include <memory>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <array>

#include <glm/glm.hpp>

typedef std::uint8_t  u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::int8_t	 s8;
typedef std::int16_t s16;
typedef std::int32_t s32;
typedef std::int64_t s64;

typedef float float32;
typedef u8 BYTE;

typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;

namespace Fidelia
{
	//@note: probably not gonna use them lol, remove if not
	template <typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... ARG>
	constexpr Scope<T> CreateScope(ARG&& ... args)
	{
		return std::make_unique<T>(std::forward<ARG>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... ARG>
	constexpr Ref<T> CreateRef(ARG&& ... args)
	{
		return std::make_shared<T>(std::forward<ARG>(args)...);
	}
}