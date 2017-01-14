#pragma once
#include <cstdint>

namespace GameEngineLibrary
{
	class SList
	{
	public:
		SList::SList(std::int32_t pData);
		~SList();

		std::int32_t GetData() const;
	private:
		std::int32_t mData;
	};
}
