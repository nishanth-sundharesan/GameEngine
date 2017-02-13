#pragma once
#include <cstdint>

namespace GameEngineLibrary
{
	class DefaultReserveStrategy
	{
	public:
		std::uint32_t DefaultReserveStrategy::operator()(std::uint32_t size, uint32_t capacity) const
		{
			size;
			return capacity * 2;
		}
	};
}