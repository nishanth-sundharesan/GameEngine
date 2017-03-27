#include "Pch.h"
#include "FooProduct.h"

using namespace std;

namespace UnitTestSupportClasses
{
	RTTI_DEFINITIONS(FooProduct);

	FooProduct::FooProduct()
	{
		mIntegerData = 100;
	}

	void FooProduct::SetIntegerData(std::int32_t integerData)
	{
		mIntegerData = integerData;
	}

	std::int32_t FooProduct::GetIntegerData()
	{
		return mIntegerData;
	}
}