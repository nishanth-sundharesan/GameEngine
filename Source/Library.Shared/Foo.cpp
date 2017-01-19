#include "Pch.h"
#include "Foo.h"

Foo::Foo() :mIntPointer(nullptr)
{
}

Foo::Foo(int32_t pData) :
	mIntData(pData), mIntPointer(new int32_t(mIntData))
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

int32_t Foo::getIntegerData() const
{
	return mIntData;
}

int32_t* Foo::getIntegerPointer() const
{
	return mIntPointer;
}

void Foo::setIntegerDataAndPointer(const int32_t pData)
{
	mIntData = pData;
	delete mIntPointer;
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
