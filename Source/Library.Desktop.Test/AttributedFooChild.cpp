#include "Pch.h"
#include "AttributedFooChild.h"

using namespace GameEngineLibrary;
using namespace std;

RTTI_DEFINITIONS(UnitTestSupportClasses::AttributedFooChild);

namespace UnitTestSupportClasses
{
	AttributedFooChild::AttributedFooChild()
	{
		InstantiateArrays(mArraySize);
		AssignValuesToMembers();
		InitializeSignatures();
	}

	AttributedFooChild::AttributedFooChild(const AttributedFooChild& rhs)
		:AttributedFoo(rhs)
	{
		InstantiateArrays(rhs.mArraySize);
		PerformDeepCopy(rhs);
		InitializeExternalSignatures();
	}

	AttributedFooChild::AttributedFooChild(AttributedFooChild&& rhs)
		:AttributedFoo(move(rhs))
	{
		PerformShallowCopyAndDefaultValues(rhs);
		InitializeExternalSignatures();
	}

	AttributedFooChild& AttributedFooChild::operator=(const AttributedFooChild& rhs)
	{
		if (this != &rhs)
		{
			AttributedFoo::operator=(rhs);
			Clear();

			InstantiateArrays(rhs.mArraySize);
			PerformDeepCopy(rhs);
			InitializeExternalSignatures();
		}
		return *this;
	}

	AttributedFooChild& AttributedFooChild::operator=(AttributedFooChild&& rhs)
	{
		if (this != &rhs)
		{
			AttributedFoo::operator=(move(rhs));
			Clear();

			PerformShallowCopyAndDefaultValues(rhs);
			InitializeExternalSignatures();
		}
		return *this;
	}

	AttributedFooChild::~AttributedFooChild()
	{
		Clear();
	}

	void AttributedFooChild::InstantiateArrays(const std::uint32_t size)
	{
		mInternalIntegerArrayChild = new int32_t[size];
		mInternalFloatArrayChild = new float_t[size];

		mExternalIntegerArrayChild = new int32_t[size];
		mExternalFloatArrayChild = new float_t[size];
	}

	void AttributedFooChild::AssignValuesToMembers()
	{
		mInternalIntegerChild = 10;
		mInternalFloatChild = 20.0f;

		mExternalIntegerChild = 30;
		mExternalFloatChild = 40.0f;

		//Internal Arrays
		mInternalIntegerArrayChild[0] = 10;
		mInternalIntegerArrayChild[1] = 20;

		mInternalFloatArrayChild[0] = 30.0f;
		mInternalFloatArrayChild[1] = 40.0f;

		//External Arrays
		mExternalIntegerArrayChild[0] = mInternalIntegerArrayChild[0];
		mExternalIntegerArrayChild[1] = mInternalIntegerArrayChild[1];

		mExternalFloatArrayChild[0] = mInternalFloatArrayChild[0];
		mExternalFloatArrayChild[1] = mInternalFloatArrayChild[1];
	}

	void AttributedFooChild::InitializeSignatures()
	{
		AttributedFoo::InitializeSignatures();

		InitializeInternalSignatures();
		InitializeExternalSignatures();
	}

	void AttributedFooChild::InitializeInternalSignatures()
	{
		AddInternalAttribute("mInternalIntegerChild", mInternalIntegerChild);
		AddInternalAttribute("mInternalFloatChild", mInternalFloatChild);
	}

	void AttributedFooChild::InitializeExternalSignatures()
	{
		AddExternalAttribute("mExternalIntegerChild", &mExternalIntegerChild, 1);
		AddExternalAttribute("mExternalFloatChild", &mExternalFloatChild, 1);
	}

	void AttributedFooChild::PerformDeepCopy(const AttributedFooChild& rhs)
	{
		mInternalIntegerChild = rhs.mInternalIntegerChild;
		mInternalFloatChild = rhs.mInternalFloatChild;

		mExternalIntegerChild = rhs.mExternalIntegerChild;
		mExternalFloatChild = rhs.mExternalFloatChild;

		for (uint32_t i = 0; i < mArraySize; ++i)
		{
			//Internal Arrays
			mInternalIntegerArrayChild[i] = rhs.mInternalIntegerArrayChild[i];
			mInternalFloatArrayChild[i] = rhs.mInternalFloatArrayChild[i];

			//External Arrays
			mExternalIntegerArrayChild[i] = rhs.mInternalIntegerArrayChild[i];
			mExternalFloatArrayChild[i] = rhs.mInternalFloatArrayChild[i];
		}
	}

	void AttributedFooChild::Clear()
	{
		delete[] mInternalIntegerArrayChild;
		delete[] mInternalFloatArrayChild;

		delete[] mExternalIntegerArrayChild;
		delete[] mExternalFloatArrayChild;
	}

	void AttributedFooChild::PerformShallowCopyAndDefaultValues(AttributedFooChild& rhs)
	{
		mInternalIntegerArrayChild = rhs.mInternalIntegerArrayChild;
		mInternalFloatArrayChild = rhs.mInternalFloatArrayChild;

		mExternalIntegerArrayChild = rhs.mExternalIntegerArrayChild;
		mExternalFloatArrayChild = rhs.mExternalFloatArrayChild;

		rhs.mInternalIntegerArrayChild = nullptr;
		rhs.mInternalFloatArrayChild = nullptr;

		rhs.mExternalIntegerArrayChild = nullptr;
		rhs.mExternalFloatArrayChild = nullptr;

		//Internal Variables
		mInternalIntegerChild = rhs.mInternalIntegerChild;
		mInternalFloatChild = rhs.mInternalFloatChild;

		//External Variables
		mExternalIntegerChild = rhs.mExternalIntegerChild;
		mExternalFloatChild = rhs.mExternalFloatChild;

		//Internal Variables
		rhs.mInternalIntegerChild = 0;
		rhs.mInternalFloatChild = 0;

		rhs.mExternalIntegerChild = 0;
		rhs.mExternalFloatChild = 0;
	}
}