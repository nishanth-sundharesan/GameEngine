#include "Pch.h"
#include "AttributedFoo.h"

using namespace GameEngineLibrary;
using namespace std;

RTTI_DEFINITIONS(UnitTestSupportClasses::AttributedFoo);

namespace UnitTestSupportClasses
{
	AttributedFoo::AttributedFoo()
	{

		InitializeSignatures();
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs)
	{
		rhs;
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		if (this != &rhs)
		{

		}
		return *this;
	}

	AttributedFoo::~AttributedFoo()
	{
		delete[] mExternalIntegerArray;
		delete[] mExternalFloatArray;
		delete[] mExternalStringArray;
		delete[] mExternalVector4Array;
		delete[] mExternalMatrix4x4Array;
	}

	void AttributedFoo::InitializeSignatures()
	{

	}
}