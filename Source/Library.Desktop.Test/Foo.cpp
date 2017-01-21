#include "Pch.h"
#include "Foo.h"

namespace UnitTestSupportClasses
{
	Foo::Foo() :mIntPointer(nullptr)
	{
	}

	Foo::Foo(int32_t data) :
		mIntData(data), mIntPointer(new int32_t(mIntData))
	{
	}

	Foo::Foo(const Foo& rhs)
	{
		mIntData = rhs.mIntData;
		mIntPointer = new int32_t(*rhs.mIntPointer);
	}

	Foo::~Foo()
	{
		Clear();
	}

	int32_t Foo::GetIntegerData() const
	{
		return mIntData;
	}

	int32_t* Foo::GetIntegerPointer() const
	{
		return mIntPointer;
	}

	void Foo::SetIntegerDataAndPointer(const int32_t data)
	{
		Clear();
		mIntData = data;
		mIntPointer = new int32_t(mIntData);
	}

	Foo& Foo::operator=(const Foo& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mIntData = rhs.mIntData;
			mIntPointer = new int32_t(*rhs.mIntPointer);
		}

		return *this;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		return (mIntData == rhs.mIntData && *mIntPointer == *rhs.mIntPointer);
	}

	void Foo::Clear()
	{
		delete mIntPointer;
	}
}