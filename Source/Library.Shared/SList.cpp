#include "Pch.h"
#include "SList.h"

using namespace std;

namespace GameEngineLibrary
{
	SList::SList(int32_t pData) :mData(pData)
	{
	}

	SList::~SList()
	{
	}

	int32_t SList::GetData() const
	{
		return mData;
	}
}
