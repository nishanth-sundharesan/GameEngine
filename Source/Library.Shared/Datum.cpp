#include "Pch.h"
#include "Datum.h"
#include <algorithm>
#include <cassert>

using namespace std;
using namespace glm;

namespace GameEngineLibrary
{
	Datum::Datum()
		:mMemoryType(DatumMemoryType::UNASSIGNED), mSize(0), mCapacity(0), mDatumType(DatumType::UNASSIGNED)
	{
		mDataTypeSizes[static_cast<uint32_t>(DatumType::UNASSIGNED)] = 0;
		mDataTypeSizes[static_cast<uint32_t>(DatumType::INT32_T)] = sizeof(int32_t);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::FLOAT)] = sizeof(float);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::STRING)] = sizeof(string);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = sizeof(vec4);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = sizeof(mat4x4);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::POINTER)] = 0;

		mDestructors[static_cast<uint32_t>(DatumType::UNASSIGNED)] = nullptr;
		mDestructors[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::DestructIntData;
		mDestructors[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::DestructFloatData;
		mDestructors[static_cast<uint32_t>(DatumType::STRING)] = &Datum::DestructStringData;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::DestructVec4Data;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::DestructMat4x4Data;
		mDestructors[static_cast<uint32_t>(DatumType::POINTER)] = nullptr;
	}

	DatumType Datum::Type() const
	{
		return mDatumType;
	}

	void Datum::SetType(DatumType datumType)
	{
		if (mDatumType != DatumType::UNASSIGNED || mDatumType != datumType)
		{
			throw exception("void Datum::SetType(DatumType datumType): Trying to assign a different datatype type to an already assigned datum.");
		}
		mDatumType = datumType;
	}

	uint32_t Datum::Size() const
	{
		return mSize;
	}

	void Datum::SetSize(uint32_t size)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			throw exception("void Datum::SetSize(uint32_t size): The Datum type is unassigned.");
		}
		if (mMemoryType != DatumMemoryType::INTERNAL)
		{
			throw exception("void Datum::SetSize(uint32_t size): Cannot set size of a datum holding external data or no data.");
		}

		if (size > mCapacity)
		{
			Reserve(size);
		}
		else if (size < mSize)
		{
			(this->*mDestructors[static_cast<uint32_t>(mDatumType)])(size, mSize);
			mSize = size;
		}
	}

	void Datum::Clear()
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
		{
			(this->*mDestructors[static_cast<uint32_t>(mDatumType)])(0, mSize);			
			mSize = 0;
		}		
	}

#pragma region Destructors for each data type
	void Datum::DestructIntData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.intPointer + i)->~int32_t();
		}
	}

	void Datum::DestructFloatData(uint32_t startIndex, uint32_t endIndex)
	{
		startIndex;
		endIndex;
		//for (uint32_t i = startIndex; i < endIndex; ++i)
		//{
		//	//Calling destructor
		//	(mDatumValues.floatPointer + i)->~float();
		//}
	}

	void Datum::DestructStringData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.stringPointer + i)->~string();
		}
	}

	void Datum::DestructVec4Data(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.vector4Pointer + i)->~vec4();
		}
	}

	void Datum::DestructMat4x4Data(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.mat4x4Pointer + i)->~mat4x4();
		}
	}
#pragma endregion

#pragma region SetStorage Implementations
	void Datum::SetStorage(int32_t* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL || mSize == 0)
		{
			throw exception("void Datum::SetStorage(int32_t* pointerToData, uint32_t size): The datum you are trying to assign contains internal data.");
		}
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("void Datum::SetStorage(int32_t* pointerToData, uint32_t size): Datum type is not set to std::int32_t.");
		}

		mDatumValues.intPointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(float* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL || mSize == 0)
		{
			throw exception("void Datum::SetStorage(float* pointerToData, uint32_t size): The datum you are trying to assign contains internal data. Please call Clear() before assigning external data.");
		}
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::SetStorage(float* pointerToData, uint32_t size): Datum type is not set to float.");
		}

		mDatumValues.floatPointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(string* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL || mSize == 0)
		{
			throw exception("void Datum::SetStorage(string* pointerToData, uint32_t size): The datum you are trying to assign contains internal data. Please call Clear() before assigning external data.");
		}
		if (mDatumType != DatumType::STRING)
		{
			throw exception("void Datum::SetStorage(string* pointerToData, uint32_t size): Datum type is not set to std::string.");
		}

		mDatumValues.stringPointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(vec4* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL || mSize == 0)
		{
			throw exception("void Datum::SetStorage(vec4* pointerToData, uint32_t size): The datum you are trying to assign contains internal data. Please call Clear() before assigning external data.");
		}
		if (mDatumType != DatumType::GLM_VECTOR4)
		{
			throw exception("void Datum::SetStorage(vec4* pointerToData, uint32_t size): Datum type is not set to glm::vec4.");
		}

		mDatumValues.vector4Pointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(mat4x4* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL || mSize == 0)
		{
			throw exception("void Datum::SetStorage(mat4x4* pointerToData, uint32_t size): The datum you are trying to assign contains internal data.");
		}
		if (mDatumType != DatumType::GLM_MATRIX4X4)
		{
			throw exception("void Datum::SetStorage(mat4x4* pointerToData, uint32_t size): Datum type is not set to glm::mat4x4.");
		}

		mDatumValues.mat4x4Pointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity != 0);
	}

#pragma endregion

#pragma region PushBack Implementations
	void Datum::PushBack(int32_t& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(int32_t& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("void Datum::PushBack(int32_t& value): Datum type is not set to std::int32_t.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.intPointer + mSize)int32_t(value);
		mSize++;
	}

	void Datum::PushBack(float& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(float& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::PushBack(float& value): Datum type is not set to float.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.floatPointer + mSize)float(value);
		mSize++;
	}

	void Datum::PushBack(string& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(string& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::STRING)
		{
			throw exception("void Datum::PushBack(string& value): Datum type is not set to std::string.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.stringPointer + mSize)string(value);
		mSize++;
	}

	void Datum::PushBack(vec4& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(vec4& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::GLM_VECTOR4)
		{
			throw exception("void Datum::PushBack(vec4& value): Datum type is not set to glm::vec4.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.stringPointer + mSize)vec4(value);
		mSize++;
	}

	void Datum::PushBack(mat4x4& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(mat4x4& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::GLM_MATRIX4X4)
		{
			throw exception("void Datum::PushBack(mat4x4& value): Datum type is not set to glm::mat4x4.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.stringPointer + mSize)mat4x4(value);
		mSize++;
	}
#pragma endregion

#pragma region Overloaded Equality Operator Implementations
	bool Datum::operator==(const Datum& rhs) const
	{
		if (mMemoryType != rhs.mMemoryType || mDatumType != rhs.mDatumType || mSize != rhs.mSize || mCapacity != rhs.mCapacity)
		{
			return false;
		}

		switch (mDatumType)
		{
		case DatumType::INT32_T:
			return PerformDeepSearch(mDatumValues.intPointer, rhs.mDatumValues.intPointer, mSize);
			break;
		case DatumType::FLOAT:
			return PerformDeepSearch(mDatumValues.floatPointer, rhs.mDatumValues.floatPointer, mSize);
			break;
		case DatumType::STRING:
			return PerformDeepSearch(mDatumValues.stringPointer, rhs.mDatumValues.stringPointer, mSize);
			break;
		case DatumType::GLM_VECTOR4:
			return PerformDeepSearch(mDatumValues.vector4Pointer, rhs.mDatumValues.vector4Pointer, mSize);
			break;
		case DatumType::GLM_MATRIX4X4:
			return PerformDeepSearch(mDatumValues.mat4x4Pointer, rhs.mDatumValues.mat4x4Pointer, mSize);
			break;
		default:
			return false;
		}
	}

	bool Datum::operator==(const int32_t& intValue) const
	{
		return (mDatumType == DatumType::INT32_T && mSize == 1 && *(mDatumValues.intPointer) == intValue);
	}

	bool Datum::operator==(const float& floatValue) const
	{
		return (mDatumType == DatumType::FLOAT && mSize == 1 && *(mDatumValues.floatPointer) == floatValue);
	}

	bool Datum::operator==(const string& stringValue) const
	{
		return (mDatumType == DatumType::STRING && mSize == 1 && *(mDatumValues.stringPointer) == stringValue);
	}

	bool Datum::operator==(const vec4& vector4Value) const
	{
		return (mDatumType == DatumType::GLM_VECTOR4 && mSize == 1 && *(mDatumValues.vector4Pointer) == vector4Value);
	}

	bool Datum::operator==(const glm::mat4x4& matrix4x4Value) const
	{
		return (mDatumType == DatumType::GLM_MATRIX4X4 && mSize == 1 && *(mDatumValues.mat4x4Pointer) == matrix4x4Value);
	}

#pragma endregion

#pragma region Overloaded InEquality Operator Implementations
	bool Datum::operator!=(const Datum& rhs) const
	{
		return !(operator==(rhs));
	}

	bool Datum::operator!=(const int32_t& intValue) const
	{
		return !(operator==(intValue));
	}

	bool Datum::operator!=(const float& floatValue) const
	{
		return !(operator==(floatValue));
	}

	bool Datum::operator!=(const string& stringValue) const
	{
		return !(operator==(stringValue));
	}

	bool Datum::operator!=(const vec4& vector4Value) const
	{
		return !(operator==(vector4Value));
	}

	bool Datum::operator!=(const glm::mat4x4& matrix4x4Value) const
	{
		return !(operator==(matrix4x4Value));
	}
#pragma endregion

#pragma region Set Method Definitions
	void Datum::Set(int32_t& value, uint32_t index)
	{
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("void Datum::Set(int32_t value, uint32_t index): Datum type is not set to std::int32_t.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(int32_t value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.intPointer + index) = value;
	}

	void Datum::Set(float& value, uint32_t index)
	{
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::Set(float value, uint32_t index): Datum type is not set to float.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(float value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.floatPointer + index) = value;
	}

	void Datum::Set(string& value, uint32_t index)
	{
		if (mDatumType != DatumType::STRING)
		{
			throw exception("void Datum::Set(string value, uint32_t index): Datum type is not set to std::string.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(string value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.stringPointer + index) = value;
	}

	void Datum::Set(vec4& value, uint32_t index)
	{
		if (mDatumType != DatumType::GLM_VECTOR4)
		{
			throw exception("void Datum::Set(vec4& value, uint32_t index): Datum type is not set to glm::vec4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(vec4& value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.vector4Pointer + index) = value;
	}

	void Datum::Set(mat4x4& value, uint32_t index)
	{
		if (mDatumType != DatumType::GLM_MATRIX4X4)
		{
			throw exception("void Datum::Set(mat4x4& value, uint32_t index): Datum type is not set to glm::mat4x4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(mat4x4& value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.mat4x4Pointer + index) = value;
	}

#pragma endregion

	void Datum::ShrinkToFit()
	{
		if (mMemoryType == DatumMemoryType::INTERNAL && mDatumType != DatumType::UNASSIGNED && mSize != mCapacity)
		{
			mDatumValues.voidPointer = realloc(mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * mSize);
			mCapacity = mSize;
		}
	}

	template<>
	int32_t Datum::Get<int32_t>(uint32_t index) const
	{
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("int32_t Datum::Get<int32_t>(uint32_t index) const: Datum type is not set to std::int32_t.");
		}

		if (index >= mSize)
		{
			throw out_of_range("int32_t Datum::Get<int32_t>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.intPointer + index);
	}

	template<>
	float Datum::Get<float>(uint32_t index) const
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			//throw exception here
		}

		if (index >= mSize)
		{
			//throw exception here
		}

		return *(mDatumValues.floatPointer + index);
	}

#pragma region Reserve Implementations
	void Datum::Reserve(uint32_t capacity)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			mDatumValues.voidPointer = realloc(mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * capacity);
			mCapacity = capacity;
		}
	}
	/*void Datum::Reserve(uint32_t capacity, int32_t* intPointer)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			intPointer = static_cast<int32_t*>(realloc(intPointer, sizeof(int32_t) * capacity));
			mCapacity = capacity;
		}
	}

	void Datum::Reserve(uint32_t capacity, float* floatPointer)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			floatPointer = static_cast<float*>(realloc(floatPointer, sizeof(float) * capacity));
			mCapacity = capacity;
		}
	}

	void Datum::Reserve(uint32_t capacity, string* stringPointer)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			stringPointer = static_cast<string*>(realloc(stringPointer, sizeof(string) * capacity));
			mCapacity = capacity;
		}
	}

	void Datum::Reserve(uint32_t capacity, vec4* vector4Pointer)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			vector4Pointer = static_cast<vec4*>(realloc(vector4Pointer, sizeof(vec4) * capacity));
			mCapacity = capacity;
		}
	}

	void Datum::Reserve(uint32_t capacity, mat4x4* mat4x4Pointer)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			mat4x4Pointer = static_cast<mat4x4*>(realloc(mat4x4Pointer, sizeof(mat4x4) * capacity));
			mCapacity = capacity;
		}
	}*/
#pragma endregion
}