#include "Pch.h"
#include "AttributedFoo.h"

using namespace GameEngineLibrary;
using namespace std;

RTTI_DEFINITIONS(UnitTestSupportClasses::AttributedFoo);

namespace UnitTestSupportClasses
{
	AttributedFoo::AttributedFoo()
	{
		InstantiateArrays(mArraySize);
		AssignValuesToMembers();
		InitializeSignatures();
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& rhs)
		:Attributed(rhs)
	{
		InstantiateArrays(rhs.mArraySize);
		PerformDeepCopy(rhs);
		InitializeExternalSignatures();
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& rhs)
		:Attributed(move(rhs))
	{
		PerformShallowCopyAndDefaultValues(rhs);
		InitializeExternalSignatures();
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(rhs);
			Clear();

			InstantiateArrays(rhs.mArraySize);
			PerformDeepCopy(rhs);
			InitializeExternalSignatures();
		}
		return *this;
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& rhs)
	{
		if (this != &rhs)
		{
			Attributed::operator=(move(rhs));
			Clear();

			PerformShallowCopyAndDefaultValues(rhs);
			InitializeExternalSignatures();
		}
		return *this;
	}

	AttributedFoo::~AttributedFoo()
	{
		Clear();
	}

	void AttributedFoo::InitializeSignatures()
	{
		InitializeInternalSignatures();
		InitializeExternalSignatures();
	}

	void AttributedFoo::Clear()
	{
		delete[] mInternalIntegerArray;
		delete[] mInternalFloatArray;
		delete[] mInternalStringArray;
		delete[] mInternalVector4Array;
		delete[] mInternalMatrix4x4Array;

		delete[] mExternalIntegerArray;
		delete[] mExternalFloatArray;
		delete[] mExternalStringArray;
		delete[] mExternalVector4Array;
		delete[] mExternalMatrix4x4Array;
	}

	void AttributedFoo::InitializeInternalSignatures()
	{
		AddInternalAttribute("mInternalInteger", mInternalInteger);
		AddInternalAttribute("mInternalFloat", mInternalFloat);
		AddInternalAttribute("mInternalString", mInternalString);
		AddInternalAttribute("mInternalVector4", mInternalVector4);
		AddInternalAttribute("mInternalMatrix4x4", mInternalMatrix4x4);

		AddInternalArrayAttribute("mInternalIntegerArray", mInternalIntegerArray, mArraySize);
		AddInternalArrayAttribute("mInternalFloatArray", mInternalFloatArray, mArraySize);
		AddInternalArrayAttribute("mInternalStringArray", mInternalStringArray, mArraySize);
		AddInternalArrayAttribute("mInternalVector4Array", mInternalVector4Array, mArraySize);
		AddInternalArrayAttribute("mInternalMatrix4x4Array", mInternalMatrix4x4Array, mArraySize);

		Scope& scope = AddNestedScopeAttribute("NestedScope");
		string nameOne = "Hello";
		string nameTwo = "World";
		string nameThree = "Now";

		int32_t firstPushedInt = 10;
		string firstPushedString = "Hello";
		Scope *firstPushedScope = new Scope();

		Datum& firstAppend = scope.Append(nameOne);
		firstAppend.SetType(DatumType::INT32_T);
		firstAppend.PushBack(firstPushedInt);

		Datum& secondAppend = scope.Append(nameTwo);
		secondAppend.SetType(DatumType::STRING);
		secondAppend.PushBack(firstPushedString);

		Datum& thirdAppend = scope.Append(nameThree);
		thirdAppend.SetType(DatumType::TABLE);
		thirdAppend.PushBack(firstPushedScope);

		scopeToAdopt = new Scope();
		AddNestedScopeAttribute(*scopeToAdopt, "AdoptedScope");
	}

	void AttributedFoo::InitializeExternalSignatures()
	{
		AddExternalAttribute("mExternalInteger", &mExternalInteger, 1);
		AddExternalAttribute("mExternalFloat", &mExternalFloat, 1);
		AddExternalAttribute("mExternalString", &mExternalString, 1);
		AddExternalAttribute("mExternalVector4", &mExternalVector4, 1);
		AddExternalAttribute("mExternalMatrix4x4", &mExternalMatrix4x4, 1);

		const RTTI* tempthis = this;
		AddExternalAttribute("AttributedFoo", &tempthis, 1U);

		AddExternalAttribute("mExternalIntegerArray", mExternalIntegerArray, mArraySize);
		AddExternalAttribute("mExternalFloatArray", mExternalFloatArray, mArraySize);
		AddExternalAttribute("mExternalStringArray", mExternalStringArray, mArraySize);
		AddExternalAttribute("mExternalVector4Array", mExternalVector4Array, mArraySize);
		AddExternalAttribute("mExternalMatrix4x4Array", mExternalMatrix4x4Array, mArraySize);
	}

	void AttributedFoo::PerformShallowCopyAndDefaultValues(AttributedFoo& rhs)
	{
		mInternalIntegerArray = rhs.mInternalIntegerArray;
		mInternalFloatArray = rhs.mInternalFloatArray;
		mInternalStringArray = rhs.mInternalStringArray;
		mInternalVector4Array = rhs.mInternalVector4Array;
		mInternalMatrix4x4Array = rhs.mInternalMatrix4x4Array;

		mExternalIntegerArray = rhs.mExternalIntegerArray;
		mExternalFloatArray = rhs.mExternalFloatArray;
		mExternalStringArray = rhs.mExternalStringArray;
		mExternalVector4Array = rhs.mExternalVector4Array;
		mExternalMatrix4x4Array = rhs.mExternalMatrix4x4Array;

		scopeToAdopt = rhs.scopeToAdopt;

		rhs.mInternalIntegerArray = nullptr;
		rhs.mInternalFloatArray = nullptr;
		rhs.mInternalStringArray = nullptr;
		rhs.mInternalVector4Array = nullptr;
		rhs.mInternalMatrix4x4Array = nullptr;

		rhs.mExternalIntegerArray = nullptr;
		rhs.mExternalFloatArray = nullptr;
		rhs.mExternalStringArray = nullptr;
		rhs.mExternalVector4Array = nullptr;
		rhs.mExternalMatrix4x4Array = nullptr;

		rhs.scopeToAdopt = nullptr;

		//Internal Variables
		mInternalInteger = rhs.mInternalInteger;
		mInternalFloat = rhs.mInternalFloat;
		mInternalString = rhs.mInternalString;
		mInternalVector4 = rhs.mInternalVector4;
		mInternalMatrix4x4 = rhs.mInternalMatrix4x4;

		//External Variables
		mExternalInteger = rhs.mExternalInteger;
		mExternalFloat = rhs.mExternalFloat;
		mExternalString = rhs.mExternalString;
		mExternalVector4 = rhs.mExternalVector4;
		mExternalMatrix4x4 = rhs.mExternalMatrix4x4;

		//Internal Variables
		rhs.mInternalInteger = 0;
		rhs.mInternalFloat = 0;
		rhs.mInternalString = string();
		rhs.mInternalVector4 = glm::vec4();
		rhs.mInternalMatrix4x4 = glm::mat4x4();

		//External Variables
		rhs.mExternalInteger = 0;
		rhs.mExternalFloat = 0;
		rhs.mExternalString = string();
		rhs.mExternalVector4 = glm::vec4();
		rhs.mExternalMatrix4x4 = glm::mat4x4();
	}

	void AttributedFoo::AssignValuesToMembers()
	{
		//Internal Variables
		mInternalInteger = 10;
		mInternalFloat = 20.0f;
		mInternalString = "Hello";
		mInternalVector4 = glm::vec4(10, 20, 30, 40);
		glm::vec4 firstVec4(10, 20, 30, 40);
		glm::vec4 secondVec4(50, 60, 70, 80);
		glm::vec4 thirdPVec4(90, 100, 110, 120);
		glm::vec4 fourthVec4(90, 100, 110, 120);
		mInternalMatrix4x4 = glm::mat4x4(firstVec4, secondVec4, thirdPVec4, fourthVec4);

		//External Variables
		mExternalInteger = mInternalInteger;
		mExternalFloat = mInternalFloat;
		mExternalString = mInternalString;
		mExternalVector4 = mInternalVector4;
		mExternalMatrix4x4 = mInternalMatrix4x4;

		//Internal Arrays
		mInternalIntegerArray[0] = 10;
		mInternalIntegerArray[1] = 20;

		mInternalFloatArray[0] = 30.0f;
		mInternalFloatArray[1] = 40.0f;

		mInternalStringArray[0] = "Hello";
		mInternalStringArray[1] = "World";

		mInternalVector4Array[0] = firstVec4;
		mInternalVector4Array[1] = secondVec4;

		mInternalMatrix4x4Array[0] = mInternalMatrix4x4;
		mInternalMatrix4x4Array[1] = mInternalMatrix4x4;

		//External Arrays
		mExternalIntegerArray[0] = mInternalIntegerArray[0];
		mExternalIntegerArray[1] = mInternalIntegerArray[1];

		mExternalFloatArray[0] = mInternalFloatArray[0];
		mExternalFloatArray[1] = mInternalFloatArray[1];

		mExternalStringArray[0] = mInternalStringArray[0];
		mExternalStringArray[1] = mInternalStringArray[1];

		mExternalVector4Array[0] = mInternalVector4Array[0];
		mExternalVector4Array[1] = mInternalVector4Array[1];

		mExternalMatrix4x4Array[0] = mInternalMatrix4x4Array[0];
		mExternalMatrix4x4Array[1] = mInternalMatrix4x4Array[1];
	}

	void AttributedFoo::InstantiateArrays(const uint32_t size)
	{
		mInternalIntegerArray = new int32_t[size];
		mInternalFloatArray = new float_t[size];
		mInternalStringArray = new string[size];
		mInternalVector4Array = new glm::vec4[size];
		mInternalMatrix4x4Array = new glm::mat4x4[size];

		mExternalIntegerArray = new int32_t[size];
		mExternalFloatArray = new float_t[size];
		mExternalStringArray = new string[size];
		mExternalVector4Array = new glm::vec4[size];
		mExternalMatrix4x4Array = new glm::mat4x4[size];
	}

	void AttributedFoo::PerformDeepCopy(const AttributedFoo& rhs)
	{
		//Internal Variables
		mInternalInteger = rhs.mInternalInteger;
		mInternalFloat = rhs.mInternalFloat;
		mInternalString = rhs.mInternalString;
		mInternalVector4 = rhs.mInternalVector4;
		mInternalMatrix4x4 = rhs.mInternalMatrix4x4;

		//External Variables
		mExternalInteger = rhs.mExternalInteger;
		mExternalFloat = rhs.mExternalFloat;
		mExternalString = rhs.mExternalString;
		mExternalVector4 = rhs.mExternalVector4;
		mExternalMatrix4x4 = rhs.mExternalMatrix4x4;

		for (uint32_t i = 0; i < mArraySize; ++i)
		{
			//Internal Arrays
			mInternalIntegerArray[i] = rhs.mInternalIntegerArray[i];
			mInternalFloatArray[i] = rhs.mInternalFloatArray[i];
			mInternalStringArray[i] = rhs.mInternalStringArray[i];
			mInternalVector4Array[i] = rhs.mInternalVector4Array[i];
			mInternalMatrix4x4Array[i] = rhs.mInternalMatrix4x4Array[i];

			//External Arrays
			mExternalIntegerArray[i] = rhs.mInternalIntegerArray[i];
			mExternalFloatArray[i] = rhs.mInternalFloatArray[i];
			mExternalStringArray[i] = rhs.mInternalStringArray[i];
			mExternalVector4Array[i] = rhs.mInternalVector4Array[i];
			mExternalMatrix4x4Array[i] = rhs.mInternalMatrix4x4Array[i];
		}

		scopeToAdopt = rhs.scopeToAdopt;
	}
}