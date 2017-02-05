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
		if (rhs.mIntPointer != nullptr)
		{
			mIntPointer = new int32_t(rhs.mIntData);
		}
		else
		{
			mIntPointer = rhs.mIntPointer;
		}
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
			if (rhs.mIntPointer != nullptr)
			{
				mIntPointer = new int32_t(rhs.mIntData);
			}
			else
			{
				mIntPointer = rhs.mIntPointer;
			}
		}

		return *this;
	}

	bool Foo::operator==(const Foo& rhs) const
	{
		if (mIntData == rhs.mIntData)
		{
			if (mIntPointer == nullptr && rhs.mIntPointer == nullptr)
			{
				return true;
			}
			if (mIntPointer == nullptr || rhs.mIntPointer == nullptr)
			{
				return false;
			}
			if (*mIntPointer == *rhs.mIntPointer)
			{
				return true;
			}			
		}
		return false;
	}

	void Foo::Clear()
	{
		delete mIntPointer;
		mIntPointer = nullptr;
	}
}