#include "Pch.h"
#include "BarChildProduct.h"

using namespace std;

namespace UnitTestSupportClasses
{
	void BarChildProduct::SetIntegerData(int32_t integerData)
	{
		mIntegerData = integerData;
	}

	int32_t BarChildProduct::GetIntegerData()
	{
		return mIntegerData;
	}
}