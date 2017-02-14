#include "Pch.h"
#include "Datum.h"
#include <algorithm>
#include <cassert>
#pragma warning ( push )
#pragma warning ( disable: 4201 )							//Suppressing the warning message "nonstandard extension used : nameless struct / union" in the file"
#include "../../External/Glm/Glm/gtx/string_cast.hpp"
#pragma warning ( pop )

using namespace std;
using namespace glm;

namespace GameEngineLibrary
{
	Datum::Datum()
		:mMemoryType(DatumMemoryType::UNASSIGNED), mSize(0), mCapacity(0), mDatumType(DatumType::UNASSIGNED)
	{
		mDatumValues.voidPointer = nullptr;
		AssignFunctionalityForEachType();
	}

	Datum::Datum(const Datum& rhs)
		: mMemoryType(rhs.mMemoryType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mDatumType(rhs.mDatumType)
	{
		mDatumValues.voidPointer = nullptr;
		AssignFunctionalityForEachType();
		Reserve(mCapacity);
		memmove(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)]);
	}

#pragma region Overloaded Assignment Operator Implementations
	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			// TODO: Optimizations by calling destructor here and assigning size, capacity and datumType
			if (mMemoryType == DatumMemoryType::INTERNAL)
			{
				if (rhs.mMemoryType == DatumMemoryType::INTERNAL)
				{
					this->~Datum();

					mCapacity = 0;
					mSize = rhs.mSize;
					mDatumType = rhs.mDatumType;					
					mDatumValues.voidPointer = nullptr;

					Reserve(rhs.mCapacity);
					memmove(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)]);
				}
				else if (rhs.mMemoryType == DatumMemoryType::EXTERNAL)
				{
					this->~Datum();

					mSize = rhs.mSize;
					mCapacity = rhs.mCapacity;
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::EXTERNAL;
					mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
				}
				else																								//For the case where rhs.mMemoryType == DatumMemoryType::UNASSIGNED
				{
					this->~Datum();

					mSize = rhs.mSize;
					mCapacity = rhs.mCapacity;
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::UNASSIGNED;
					mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
				}
			}
			else if (mMemoryType == DatumMemoryType::EXTERNAL)
			{
				if (rhs.mMemoryType == DatumMemoryType::INTERNAL)
				{
					mCapacity = 0;					
					mSize = rhs.mSize;	
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::INTERNAL;
					mDatumValues.voidPointer = nullptr;

					Reserve(rhs.mCapacity);
					memmove(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * mSize);
				}
				else if (rhs.mMemoryType == DatumMemoryType::EXTERNAL)
				{
					mSize = rhs.mSize;
					mCapacity = rhs.mCapacity;
					mDatumType = rhs.mDatumType;
					mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
				}
				else																								//For the case where rhs.mMemoryType == DatumMemoryType::UNASSIGNED
				{
					mSize = rhs.mSize;
					mCapacity = rhs.mCapacity;
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::UNASSIGNED;
					mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
				}
			}
			else																									//For the case where mMemoryType == DatumMemoryType::UNASSIGNED
			{
				if (rhs.mMemoryType == DatumMemoryType::INTERNAL)
				{
					mCapacity = 0;
					mSize = rhs.mSize;					
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::INTERNAL;
					mDatumValues.voidPointer = nullptr;

					Reserve(rhs.mCapacity);
					memmove(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)]);
				}
				else if (rhs.mMemoryType == DatumMemoryType::EXTERNAL)
				{
					mSize = rhs.mSize;
					mCapacity = rhs.mCapacity;
					mDatumType = rhs.mDatumType;
					mMemoryType = DatumMemoryType::EXTERNAL;
					mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
				}
			}
		}
		return *this;
	}

	Datum& Datum::operator=(const int32_t& rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::INT32_T;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const std::float_t& rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::FLOAT;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const string& rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::STRING;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const vec4& rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::GLM_VECTOR4;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const mat4x4& rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::GLM_MATRIX4X4;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

	Datum& Datum::operator=(const RTTI* rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::POINTER;
			PushBack(rhs);
		}
		else
		{
			Set(rhs);
		}
		return *this;
	}

#pragma endregion

	Datum::~Datum()
	{
		Clear();
		ShrinkToFit();
	}

#pragma region Public Methods

	void Datum::SetType(DatumType datumType)
	{
		if (mDatumType != datumType && mDatumType != DatumType::UNASSIGNED)
		{
			throw exception("void Datum::SetType(DatumType datumType): Trying to assign a different datatype type to an already assigned datum.");
		}
		mDatumType = datumType;
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

		(this->*mPushBacks[static_cast<uint32_t>(mDatumType)])(mSize, size);

		if (size < mSize)
		{
			(this->*mDestructors[static_cast<uint32_t>(mDatumType)])(size, mSize);		
		}
		mSize = size;
	}

	DatumType Datum::Type() const
	{
		return mDatumType;
	}

	uint32_t Datum::Size() const
	{
		return mSize;
	}

	void Datum::Clear()
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
		{
			(this->*mDestructors[static_cast<uint32_t>(mDatumType)])(0, mSize);
			mSize = 0;
		}
	}		

#pragma region SetStorage Implementations
	void Datum::SetStorage(int32_t* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
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
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(std::float_t* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
		{
			throw exception("void Datum::SetStorage(std::float_t* pointerToData, uint32_t size): The datum you are trying to assign contains internal data. Please call Clear() before assigning external data.");
		}
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::SetStorage(std::float_t* pointerToData, uint32_t size): Datum type is not set to std:float.");
		}

		mDatumValues.floatPointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(string* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
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
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(vec4* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
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
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(mat4x4* pointerToData, uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
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
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(RTTI** pointerToData, std::uint32_t size)
	{
		if (mMemoryType == DatumMemoryType::INTERNAL)
		{
			throw exception("void Datum::SetStorage(RTTI ** pointerToData, std::uint32_t size): The datum you are trying to assign contains internal data.");
		}
		if (mDatumType != DatumType::POINTER)
		{
			throw exception("void Datum::SetStorage(RTTI ** pointerToData, std::uint32_t size): Datum type is not set to the type POINTER.");
		}

		mDatumValues.rttiPointer = pointerToData;
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

#pragma endregion

#pragma region PushBack Implementations
	void Datum::PushBack(const int32_t& value)
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
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.intPointer + mSize)int32_t(value);
		mSize++;
	}

	void Datum::PushBack(const std::float_t& value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(std::float& value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::PushBack(std::float& value): Datum type is not set to std:float.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.floatPointer + mSize)std::float_t(value);
		mSize++;
	}

	void Datum::PushBack(const string& value)
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
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.stringPointer + mSize)string(value);
		mSize++;
	}

	void Datum::PushBack(const vec4& value)
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
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.vector4Pointer + mSize)vec4(value);
		mSize++;
	}

	void Datum::PushBack(const mat4x4& value)
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
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.mat4x4Pointer + mSize)mat4x4(value);
		mSize++;
	}

	void Datum::PushBack(const RTTI* value)
	{
		if (mMemoryType == DatumMemoryType::EXTERNAL)
		{
			throw exception("void Datum::PushBack(const RTTI** value): The datum you are trying to assign contains external data.");
		}
		if (mDatumType != DatumType::POINTER)
		{
			throw exception("void Datum::PushBack(const RTTI** value): Datum type is not set to the type POINTER.");
		}
		if (mSize == mCapacity)
		{
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		mDatumValues.rttiPointer[mSize] = const_cast<RTTI*>(value);
		mSize++;
	}
#pragma endregion

#pragma region Overloaded Equality Operator Implementations
	bool Datum::operator==(const Datum& rhs) const
	{
		if (mDatumType != rhs.mDatumType || mSize != rhs.mSize)
		{
			return false;
		}

		return (this->*mPerformSearch[static_cast<uint32_t>(mDatumType)])(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mSize);

		/*switch (mDatumType)
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
		}*/
	}

	bool Datum::operator==(const int32_t& intValue) const
	{
		return (mDatumType == DatumType::INT32_T && mSize == 1 && *(mDatumValues.intPointer) == intValue);
	}

	bool Datum::operator==(const std::float_t& floatValue) const
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

	bool Datum::operator==(const RTTI* rttiPointer) const
	{
		return (mDatumType == DatumType::POINTER && mSize == 1 && (**mDatumValues.rttiPointer).Equals(rttiPointer));
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

	bool Datum::operator!=(const std::float_t& floatValue) const
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

	bool Datum::operator!=(const RTTI* rttiPointer) const
	{
		return !(operator==(rttiPointer));
	}
#pragma endregion

#pragma region Set Method Definitions
	void Datum::Set(const int32_t& value, uint32_t index)
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

	void Datum::Set(const std::float_t& value, uint32_t index)
	{
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("void Datum::Set(std::float_t value, uint32_t index): Datum type is not set to std::float.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(std::float_t value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.floatPointer + index) = value;
	}

	void Datum::Set(const string& value, uint32_t index)
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

	void Datum::Set(const vec4& value, uint32_t index)
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

	void Datum::Set(const mat4x4& value, uint32_t index)
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

	void Datum::Set(const RTTI* value, uint32_t index)
	{
		if (mDatumType != DatumType::POINTER)
		{
			throw exception("void Datum::Set(const RTTI* value, uint32_t index): Datum type is not set to POINTER.");
		}
		if (index >= mSize)
		{
			throw out_of_range("void Datum::Set(const RTTI* value, uint32_t index): The specified 'index' is out of range.");
		}
		*(mDatumValues.rttiPointer + index) = const_cast<RTTI*>(value);
	}

#pragma endregion
	
#pragma region Get Method Implementations

#pragma region Non Const Get Method Implementations
	template<>
	int32_t& Datum::Get<int32_t>(uint32_t index)
	{
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("const int32_t& Datum::Get<int32_t>(uint32_t index) const: Datum type is not set to std::int32_t.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const int32_t& Datum::Get<int32_t>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.intPointer + index);
	}

	template<>
	std::float_t& Datum::Get<std::float_t>(uint32_t index)
	{
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("const std::float& Datum::Get<std::float>(uint32_t index) const: Datum type is not set to std::float.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const std::float& Datum::Get<std::float>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.floatPointer + index);
	}

	template<>
	string& Datum::Get<string>(uint32_t index)
	{
		if (mDatumType != DatumType::STRING)
		{
			throw exception("const string& Datum::Get<string>(uint32_t index) const: Datum type is not set to std::string.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const string& Datum::Get<string>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.stringPointer + index);
	}

	template<>
	vec4& Datum::Get<vec4>(uint32_t index)
	{
		if (mDatumType != DatumType::GLM_VECTOR4)
		{
			throw exception("const vec4& Datum::Get<vec4>(uint32_t index) const: Datum type is not set to glm::vec4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const vec4& Datum::Get<vec4>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.vector4Pointer + index);
	}

	template<>
	mat4x4& Datum::Get<mat4x4>(uint32_t index)
	{
		if (mDatumType != DatumType::GLM_MATRIX4X4)
		{
			throw exception("const mat4x4& Datum::Get<mat4x4>(uint32_t index) const: Datum type is not set to glm::mat4x4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const mat4x4& Datum::Get<mat4x4>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.mat4x4Pointer + index);
	}

	/*template<>
	RTTI*& Datum::Get<RTTI*>(uint32_t index)
	{
		if (mDatumType != DatumType::POINTER)
		{
			throw exception("RTTI* Datum::Get<RTTI>(uint32_t index): Datum type is not set to POINTER.");
		}
		if (index >= mSize)
		{
			throw out_of_range("RTTI* Datum::Get<RTTI>(uint32_t index): The specified 'index' is out of range.");
		}

		return *(mDatumValues.rttiPointer + index);
	}*/
#pragma endregion

#pragma region Const Get Method Implementations.
	template<>
	const int32_t& Datum::Get<int32_t>(uint32_t index) const
	{
		if (mDatumType != DatumType::INT32_T)
		{
			throw exception("const int32_t& Datum::Get<int32_t>(uint32_t index) const: Datum type is not set to std::int32_t.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const int32_t& Datum::Get<int32_t>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.intPointer + index);
	}

	template<>
	const std::float_t& Datum::Get<std::float_t>(uint32_t index) const
	{
		if (mDatumType != DatumType::FLOAT)
		{
			throw exception("const std::float& Datum::Get<std::float>(uint32_t index) const: Datum type is not set to std::float.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const std::float& Datum::Get<std::float>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.floatPointer + index);
	}

	template<>
	const string& Datum::Get<string>(uint32_t index) const
	{
		if (mDatumType != DatumType::STRING)
		{
			throw exception("const string& Datum::Get<string>(uint32_t index) const: Datum type is not set to std::string.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const string& Datum::Get<string>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.stringPointer + index);
	}

	template<>
	const vec4& Datum::Get<vec4>(uint32_t index) const
	{
		if (mDatumType != DatumType::GLM_VECTOR4)
		{
			throw exception("const vec4& Datum::Get<vec4>(uint32_t index) const: Datum type is not set to glm::vec4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const vec4& Datum::Get<vec4>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.vector4Pointer + index);
	}

	template<>
	const mat4x4& Datum::Get<mat4x4>(uint32_t index) const
	{
		if (mDatumType != DatumType::GLM_MATRIX4X4)
		{
			throw exception("const mat4x4& Datum::Get<mat4x4>(uint32_t index) const: Datum type is not set to glm::mat4x4.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const mat4x4& Datum::Get<mat4x4>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.mat4x4Pointer + index);
	}

	/*template<>
	const RTTI*& Datum::Get<RTTI*>(uint32_t index) const
	{
		if (mDatumType != DatumType::POINTER)
		{
			throw exception("const RTTI* Datum::Get<RTTI>(uint32_t index) const: Datum type is not set to POINTER.");
		}
		if (index >= mSize)
		{
			throw out_of_range("const RTTI* Datum::Get<RTTI>(uint32_t index) const: The specified 'index' is out of range.");
		}

		return *(mDatumValues.rttiPointer + index);
	}*/

#pragma endregion

#pragma endregion
	
	string Datum::ToString(uint32_t index) const
	{
		if (index >= mSize)
		{
			throw out_of_range("string Datum::ToString(uint32_t index) const: The specified 'index' is out of range.");
		}

		return (this->*mToString[static_cast<uint32_t>(mDatumType)])(index);

		//switch (mDatumType)
		//{
		//case GameEngineLibrary::DatumType::UNASSIGNED:
		//	throw exception("string Datum::ToString(uint32_t index) const: Cannot convert unassigned Datum to std::string.");
		//	break;
		//case GameEngineLibrary::DatumType::INT32_T:
		//	return std::to_string(*(mDatumValues.intPointer + index));
		//	break;
		//case GameEngineLibrary::DatumType::FLOAT:
		//	return std::to_string(*(mDatumValues.floatPointer + index));
		//	break;
		//case GameEngineLibrary::DatumType::STRING:
		//	return *(mDatumValues.stringPointer + index);
		//	break;
		//case GameEngineLibrary::DatumType::GLM_VECTOR4:
		//	return glm::to_string(*(mDatumValues.vector4Pointer + index));
		//	break;
		//case GameEngineLibrary::DatumType::GLM_MATRIX4X4:
		//	return glm::to_string(*(mDatumValues.mat4x4Pointer + index));
		//	break;
		//case GameEngineLibrary::DatumType::POINTER:
		//	return (*(*(mDatumValues.rttiPointer + index))).ToString();
		//	break;
		//default:
		//	break;
		//}

		//// TODO remove the empty return statement
		//return "";
	}

	void Datum::SetFromString(string& inputString, uint32_t index)
	{
		/*if (index >= mSize)
		{
			throw out_of_range("void Datum::SetFromString(string& inputString, uint32_t index): The specified 'index' is out of range.");
		}*/

		switch (mDatumType)
		{
		case GameEngineLibrary::DatumType::UNASSIGNED:
			throw exception("exception");
			break;
		case GameEngineLibrary::DatumType::INT32_T:
			int32_t readIntValue;
			sscanf_s(inputString.c_str(), "%d", &readIntValue);
			Set(readIntValue, index);
			break;
		case GameEngineLibrary::DatumType::FLOAT:
			float readFloatValue;
			sscanf_s(inputString.c_str(), "%f", &readFloatValue);
			Set(readFloatValue, index);
			break;
		case GameEngineLibrary::DatumType::STRING:
			Set(inputString, index);
			break;
		case GameEngineLibrary::DatumType::GLM_VECTOR4:

			break;
		case GameEngineLibrary::DatumType::GLM_MATRIX4X4:
			break;
		case GameEngineLibrary::DatumType::POINTER:
			throw exception("exception");
			break;
		default:
			break;
		}
		inputString;
		index;
	}

	void Datum::ShrinkToFit()
	{
		if (mMemoryType == DatumMemoryType::INTERNAL && mDatumType != DatumType::UNASSIGNED && mSize != mCapacity)
		{
			mDatumValues.voidPointer = realloc(mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * mSize);
			mCapacity = mSize;
		}
	}

#pragma endregion

#pragma region Private Methods
	void Datum::Reserve(uint32_t capacity)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			mDatumValues.voidPointer = realloc(mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * capacity);
			mCapacity = capacity;
		}
	}

	void Datum::AssignFunctionalityForEachType()
	{
		mDataTypeSizes[static_cast<uint32_t>(DatumType::UNASSIGNED)] = 0;
		mDataTypeSizes[static_cast<uint32_t>(DatumType::INT32_T)] = sizeof(int32_t);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::FLOAT)] = sizeof(std::float_t);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::STRING)] = sizeof(string);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = sizeof(vec4);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = sizeof(mat4x4);
		mDataTypeSizes[static_cast<uint32_t>(DatumType::POINTER)] = sizeof(RTTI**);

		mDestructors[static_cast<uint32_t>(DatumType::UNASSIGNED)] = nullptr;
		mDestructors[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::DestructIntData;
		mDestructors[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::DestructFloatData;
		mDestructors[static_cast<uint32_t>(DatumType::STRING)] = &Datum::DestructStringData;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::DestructVec4Data;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::DestructMat4x4Data;
		mDestructors[static_cast<uint32_t>(DatumType::POINTER)] = nullptr;

		mPushBacks[static_cast<uint32_t>(DatumType::UNASSIGNED)] = nullptr;
		mPushBacks[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::PushBackIntData;
		mPushBacks[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::PushBackFloatData;
		mPushBacks[static_cast<uint32_t>(DatumType::STRING)] = &Datum::PushBackStringData;
		mPushBacks[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::PushBackVec4Data;
		mPushBacks[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::PushBackMat4x4Data;
		mPushBacks[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::PushBackRTTIPointer;

		mPerformSearch[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::PerformVoidSearch;
		mPerformSearch[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::PerformIntSearch;
		mPerformSearch[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::PerformFloatSearch;
		mPerformSearch[static_cast<uint32_t>(DatumType::STRING)] = &Datum::PerformStringSearch;
		mPerformSearch[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::PerformVec4Search;
		mPerformSearch[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::PerformMat4x4Search;
		mPerformSearch[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::PerformRTTIPointerSearch;

		mToString[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::ToStringUnassigned;
		mToString[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::ToStringInt;
		mToString[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::ToStringFloat;
		mToString[static_cast<uint32_t>(DatumType::STRING)] = &Datum::ToStringString;
		mToString[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::ToStringVec4;
		mToString[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::ToStringMat4x4;
		mToString[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::ToStringRTTIPointer;
	}

	uint32_t Datum::ReserveStrategy(uint32_t capacity)
	{
		return std::max<uint32_t>(1U, capacity + capacity);
	}

#pragma region Search based on each data type(Declarations)
	bool Datum::PerformVoidSearch(void * lhs, void * rhs, std::uint32_t size) const
	{
		return true;
		lhs;
		rhs;
		size;
	}

	bool Datum::PerformIntSearch(void* lhs, void* rhs, uint32_t size) const
	{
		return PerformDeepSearch(static_cast<int32_t*>(lhs), static_cast<int32_t*>(rhs), size);
	}

	bool Datum::PerformFloatSearch(void* lhs, void* rhs, uint32_t size) const
	{
		return PerformDeepSearch(static_cast<std::float_t*>(lhs), static_cast<std::float_t*>(rhs), size);
	}

	bool Datum::PerformStringSearch(void* lhs, void* rhs, uint32_t size) const
	{
		return PerformDeepSearch(static_cast<string*>(lhs), static_cast<string*>(rhs), size);
	}

	bool Datum::PerformVec4Search(void* lhs, void* rhs, uint32_t size) const
	{
		return PerformDeepSearch(static_cast<vec4*>(lhs), static_cast<vec4*>(rhs), size);
	}

	bool Datum::PerformMat4x4Search(void* lhs, void* rhs, uint32_t size) const
	{
		return PerformDeepSearch(static_cast<mat4x4*>(lhs), static_cast<mat4x4*>(rhs), size);
	}

	bool Datum::PerformRTTIPointerSearch(void* lhs, void* rhs, uint32_t size) const
	{
		RTTI** lhsRTTI = static_cast<RTTI**>(lhs);
		RTTI** rhsRTTI = static_cast<RTTI**>(rhs);

		for (uint32_t i = 0; i < size; ++i)
		{
			if (!((**(lhsRTTI + i)).Equals(*(rhsRTTI + i))))
			{
				return false;
			}
		}
		return true;
	}
#pragma endregion

#pragma region Pushbacking based on each data type	
	void Datum::PushBackIntData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(int32_t());
		}
	}

	void Datum::PushBackFloatData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(std::float_t());
		}
	}

	void Datum::PushBackStringData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(string());
		}
	}

	void Datum::PushBackVec4Data(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(vec4());
		}
	}

	void Datum::PushBackMat4x4Data(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(mat4x4());
		}
	}

	void Datum::PushBackRTTIPointer(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(nullptr);
		}
	}
#pragma endregion

#pragma region Destructing for each data type
	void Datum::DestructIntData(uint32_t startIndex, uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.intPointer + i)->~int32_t();
		}
	}

	void Datum::DestructFloatData(uint32_t startIndex, uint32_t endIndex)
	{
		typedef std::float_t float_std;
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.floatPointer + i)->~float_std();
		}
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

#pragma region Converting each data type to std::string Implementations
	std::string Datum::ToStringUnassigned(uint32_t index) const
	{
		throw exception("std::string Datum::ToStringUnassigned() const: Cannot convert unassigned Datum to std::string.");
		index;
	}

	std::string Datum::ToStringInt(uint32_t index) const
	{
		return std::to_string(*(mDatumValues.intPointer + index));
	}

	std::string Datum::ToStringFloat(uint32_t index) const
	{
		return std::to_string(*(mDatumValues.floatPointer + index));
	}

	std::string Datum::ToStringString(uint32_t index) const
	{
		return *(mDatumValues.stringPointer + index);
	}

	std::string Datum::ToStringVec4(uint32_t index) const
	{
		return glm::to_string(*(mDatumValues.vector4Pointer + index));
	}

	std::string Datum::ToStringMat4x4(uint32_t index) const
	{
		return glm::to_string(*(mDatumValues.mat4x4Pointer + index));
	}

	std::string Datum::ToStringRTTIPointer(uint32_t index) const
	{
		return (*(*(mDatumValues.rttiPointer + index))).ToString();
	}
#pragma endregion

#pragma endregion
}