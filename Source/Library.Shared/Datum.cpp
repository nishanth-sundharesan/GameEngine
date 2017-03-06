#include "Pch.h"
#include "Datum.h"
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
		: mMemoryType(rhs.mMemoryType), mSize(rhs.mSize), mCapacity(0), mDatumType(rhs.mDatumType)
	{
		AssignFunctionalityForEachType();
		if (rhs.mMemoryType == DatumMemoryType::INTERNAL)
		{
			mDatumValues.voidPointer = nullptr;
			Reserve(rhs.mCapacity);

			(this->*mPerformDeepCopy[static_cast<uint32_t>(mDatumType)])(rhs.mDatumValues, mSize);
		}
		else
		{
			mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
		}
	}

	Datum::Datum(Datum&& rhs)
		:mMemoryType(rhs.mMemoryType), mSize(rhs.mSize), mCapacity(rhs.mCapacity), mDatumType(rhs.mDatumType)
	{
		AssignFunctionalityForEachType();
		mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;

		rhs.mMemoryType = DatumMemoryType::UNASSIGNED;
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mDatumType = DatumType::UNASSIGNED;
		rhs.mDatumValues.voidPointer = nullptr;
	}

#pragma region Overloaded Assignment Operator Implementations
	Datum& Datum::operator=(const Datum& rhs)
	{
		if (this != &rhs)
		{
			if (mMemoryType == DatumMemoryType::INTERNAL)
			{
				this->~Datum();
			}

			mSize = rhs.mSize;
			mDatumType = rhs.mDatumType;
			mMemoryType = rhs.mMemoryType;
			if (rhs.mMemoryType == DatumMemoryType::INTERNAL)
			{
				mCapacity = 0;				
				mDatumValues.voidPointer = nullptr;

				Reserve(rhs.mCapacity);
				(this->*mPerformDeepCopy[static_cast<uint32_t>(mDatumType)])(rhs.mDatumValues, mSize);
			}
			else
			{				
				mCapacity = rhs.mCapacity;				
				mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;
			}
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs)
	{
		if (this != &rhs)
		{
			if (mMemoryType == DatumMemoryType::INTERNAL)
			{
				this->~Datum();
			}

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mDatumType = rhs.mDatumType;
			mMemoryType = rhs.mMemoryType;
			mDatumValues.voidPointer = rhs.mDatumValues.voidPointer;

			rhs.mMemoryType = DatumMemoryType::UNASSIGNED;
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mDatumType = DatumType::UNASSIGNED;
			rhs.mDatumValues.voidPointer = nullptr;
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

	Datum& Datum::operator=(const RTTI* const rhs)
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

	Datum& Datum::operator=(const Scope* const rhs)
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			mDatumType = DatumType::TABLE;
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

	void Datum::SetType(const DatumType datumType)
	{
		if (datumType == DatumType::UNASSIGNED)
		{
			throw exception("void Datum::SetType(DatumType datumType): Cannot assign an UNASSIGNED datum");
		}
		if (mDatumType != datumType && mDatumType != DatumType::UNASSIGNED)
		{
			throw exception("void Datum::SetType(DatumType datumType): Trying to assign a different datatype type to an already assigned datum.");
		}
		mDatumType = datumType;
	}

	void Datum::SetSize(const uint32_t size)
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
	void Datum::SetStorage(const int32_t* const pointerToData, const uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::INT32_T);

		mDatumValues.intPointer = const_cast<int32_t*>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(const std::float_t* const pointerToData, const uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::FLOAT);

		mDatumValues.floatPointer = const_cast<std::float_t*>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(const string* const pointerToData, const uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::STRING);

		mDatumValues.stringPointer = const_cast<string*>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(const vec4* const pointerToData, const uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::GLM_VECTOR4);

		mDatumValues.vector4Pointer = const_cast<vec4*>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(const mat4x4* const pointerToData, const uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::GLM_MATRIX4X4);

		mDatumValues.mat4x4Pointer = const_cast<mat4x4*>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

	void Datum::SetStorage(const RTTI** const pointerToData, const std::uint32_t size)
	{
		CheckForTheMemoryType(DatumMemoryType::INTERNAL);
		CheckForDatumType(DatumType::POINTER);

		mDatumValues.rttiPointer = const_cast<RTTI**>(pointerToData);
		mSize = size;
		mMemoryType = DatumMemoryType::EXTERNAL;
		assert(mCapacity == 0);
	}

#pragma endregion

#pragma region PushBack Implementations
	void Datum::PushBack(const int32_t& value)
	{
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::INT32_T);
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
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::FLOAT);
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
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::STRING);
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
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::GLM_VECTOR4);
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
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::GLM_MATRIX4X4);
		if (mSize == mCapacity)
		{
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		new(mDatumValues.mat4x4Pointer + mSize)mat4x4(value);
		mSize++;
	}

	void Datum::PushBack(const RTTI* const value)
	{
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::POINTER);
		if (mSize == mCapacity)
		{
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		mDatumValues.rttiPointer[mSize] = const_cast<RTTI*>(value);
		mSize++;
	}

	void Datum::PushBack(const Scope* const value)
	{
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::TABLE);
		if (mSize == mCapacity)
		{
			uint32_t capacity = ReserveStrategy(mCapacity);
			Reserve(capacity);
		}

		mMemoryType = DatumMemoryType::INTERNAL;
		mDatumValues.tablePointer[mSize] = const_cast<Scope*>(value);
		mSize++;
	}

#pragma endregion

#pragma region Remove Implementations
	bool Datum::Remove(const Scope* const value)
	{
		CheckForTheMemoryType(DatumMemoryType::EXTERNAL);
		CheckForDatumType(DatumType::TABLE);
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mDatumValues.tablePointer[i] == value)
			{
				mDatumValues.tablePointer[i] = nullptr;
				--mSize;
				if (i != mSize)
				{
					memmove(mDatumValues.tablePointer[i], mDatumValues.tablePointer[i + 1], mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * (mSize - i));
				}
				return true;
			}
		}
		return false;
	}
#pragma endregion

#pragma region Overloaded Equality Operator Implementations
	bool Datum::operator==(const Datum& rhs) const
	{
		if (mDatumType != rhs.mDatumType || mSize != rhs.mSize)
		{
			return false;
		}

		return (this->*mPerformDeepCompare[static_cast<uint32_t>(mDatumType)])(rhs.mDatumValues);
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

	bool Datum::operator==(const RTTI* const rttiPointer) const
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

	bool Datum::operator!=(const RTTI* const rttiPointer) const
	{
		return !(operator==(rttiPointer));
	}
#pragma endregion

#pragma region Set Method Definitions

	void Datum::Set(const int32_t& value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::INT32_T, index);
		*(mDatumValues.intPointer + index) = value;
	}

	void Datum::Set(const std::float_t& value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::FLOAT, index);
		*(mDatumValues.floatPointer + index) = value;
	}

	void Datum::Set(const string& value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::STRING, index);
		*(mDatumValues.stringPointer + index) = value;
	}

	void Datum::Set(const vec4& value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::GLM_VECTOR4, index);
		*(mDatumValues.vector4Pointer + index) = value;
	}

	void Datum::Set(const mat4x4& value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::GLM_MATRIX4X4, index);
		*(mDatumValues.mat4x4Pointer + index) = value;
	}

	void Datum::Set(const RTTI* const value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::POINTER, index);
		*(mDatumValues.rttiPointer + index) = const_cast<RTTI*>(value);
	}

	void Datum::Set(const Scope* const value, const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::TABLE, index);
		*(mDatumValues.tablePointer + index) = const_cast<Scope*>(value);
	}

#pragma endregion

#pragma region Get Method Implementations

#pragma region Non Const Get Method Implementations
	template<>
	int32_t& Datum::Get<int32_t>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::INT32_T, index);
		return *(mDatumValues.intPointer + index);
	}

	template<>
	std::float_t& Datum::Get<std::float_t>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::FLOAT, index);
		return *(mDatumValues.floatPointer + index);
	}

	template<>
	string& Datum::Get<string>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::STRING, index);
		return *(mDatumValues.stringPointer + index);
	}

	template<>
	vec4& Datum::Get<vec4>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::GLM_VECTOR4, index);
		return *(mDatumValues.vector4Pointer + index);
	}

	template<>
	mat4x4& Datum::Get<mat4x4>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::GLM_MATRIX4X4, index);
		return *(mDatumValues.mat4x4Pointer + index);
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::POINTER, index);
		return *(mDatumValues.rttiPointer + index);
	}

	template<>
	Scope*& Datum::Get<Scope*>(const uint32_t index)
	{
		CheckForBoundsAndDatumType(DatumType::TABLE, index);
		return *(mDatumValues.tablePointer + index);
	}
#pragma endregion

#pragma region Const Get Method Implementations.
	template<>
	const int32_t& Datum::Get<int32_t>(const uint32_t index) const
	{
		return const_cast<const int32_t&>(const_cast<Datum*>(this)->Get<int32_t>(index));
	}

	template<>
	const std::float_t& Datum::Get<std::float_t>(const uint32_t index) const
	{
		return const_cast<const std::float_t&>(const_cast<Datum*>(this)->Get<std::float_t>(index));
	}

	template<>
	const string& Datum::Get<string>(const uint32_t index) const
	{
		return const_cast<const string&>(const_cast<Datum*>(this)->Get<string>(index));
	}

	template<>
	const vec4& Datum::Get<vec4>(const uint32_t index) const
	{
		return const_cast<const vec4&>(const_cast<Datum*>(this)->Get<vec4>(index));
	}

	template<>
	const mat4x4& Datum::Get<mat4x4>(const uint32_t index) const
	{
		return const_cast<const mat4x4&>(const_cast<Datum*>(this)->Get<mat4x4>(index));
	}

	template<>
	RTTI*const& Datum::Get<RTTI*>(const uint32_t index) const
	{
		return const_cast<RTTI*const&>(const_cast<Datum*>(this)->Get<RTTI*>(index));
	}

	template<>
	Scope*const& Datum::Get<Scope*>(const uint32_t index) const
	{
		return const_cast<Scope*const&>(const_cast<Datum*>(this)->Get<Scope*>(index));
	}
#pragma endregion

#pragma endregion

	string Datum::ToString(const uint32_t index) const
	{
		CheckForBounds(index);
		string convertedString;
		(this->*mToString[static_cast<uint32_t>(mDatumType)])(index, convertedString);
		return convertedString;
	}

	void Datum::SetFromString(const string& inputString, const uint32_t index)
	{
		CheckForBounds(index);
		(this->*mSetFromString[static_cast<uint32_t>(mDatumType)])(inputString, index);
	}

	void Datum::ShrinkToFit()
	{
		if (mMemoryType == DatumMemoryType::INTERNAL && mDatumType != DatumType::UNASSIGNED && mSize != mCapacity)
		{
			mDatumValues.voidPointer = realloc(mDatumValues.voidPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * mSize);
			mCapacity = mSize;
		}
	}

	Scope& Datum::operator[](const std::uint32_t index)
	{
		return const_cast<Scope&>(const_cast<const Datum*>(this)->operator[](index));
	}

	const Scope& Datum::operator[](const std::uint32_t index) const
	{
		return *Get<Scope*>(index);
	}

#pragma endregion

#pragma region Private Methods

#pragma region Helper Check Methods
	void Datum::CheckForTheMemoryType(const DatumMemoryType memoryType) const
	{
		if (mMemoryType == memoryType)
		{
			throw exception("The datum you are trying to assign contains invalid external data. You are either trying to assign External storage to an Internal storage type Datum or trying to Append data to an External storage type Datum.");
		}
	}

	void Datum::CheckForBounds(const uint32_t index) const
	{
		if (index >= mSize)
		{
			throw out_of_range("The specified 'index' is out of range.");
		}
	}

	void Datum::CheckForDatumType(const DatumType datumType) const
	{
		if (mDatumType != datumType)
		{
			throw exception("Datum type is invalid.");
		}
	}

	void Datum::CheckForBoundsAndDatumType(const DatumType datumType, const uint32_t index) const
	{
		CheckForDatumType(datumType);
		CheckForBounds(index);
	}
#pragma endregion

	void Datum::Reserve(const uint32_t capacity)
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
		mDataTypeSizes[static_cast<uint32_t>(DatumType::TABLE)] = sizeof(Scope**);

		mDestructors[static_cast<uint32_t>(DatumType::UNASSIGNED)] = nullptr;
		mDestructors[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::DestructPrimitiveTypes;
		mDestructors[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::DestructPrimitiveTypes;
		mDestructors[static_cast<uint32_t>(DatumType::STRING)] = &Datum::DestructStringData;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::DestructPrimitiveTypes;
		mDestructors[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::DestructPrimitiveTypes;
		mDestructors[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::DestructPrimitiveTypes;
		mDestructors[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::DestructPrimitiveTypes;

		mPushBacks[static_cast<uint32_t>(DatumType::UNASSIGNED)] = nullptr;
		mPushBacks[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::PushBackIntData;
		mPushBacks[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::PushBackFloatData;
		mPushBacks[static_cast<uint32_t>(DatumType::STRING)] = &Datum::PushBackStringData;
		mPushBacks[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::PushBackVec4Data;
		mPushBacks[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::PushBackMat4x4Data;
		mPushBacks[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::PushBackRTTIPointer;
		mPushBacks[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::PushBackScopePointer;

		mPerformDeepCompare[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::PerformVoidDeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::PerformIntDeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::PerformFloatDeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::STRING)] = &Datum::PerformStringDeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::PerformVec4DeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::PerformMat4x4DeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::PerformRTTIPointerDeepCompare;
		mPerformDeepCompare[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::PerformRTTIPointerDeepCompare;

		mToString[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::ToStringUnassigned;
		mToString[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::ToStringInt;
		mToString[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::ToStringFloat;
		mToString[static_cast<uint32_t>(DatumType::STRING)] = &Datum::ToStringString;
		mToString[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::ToStringVec4;
		mToString[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::ToStringMat4x4;
		mToString[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::ToStringRTTIPointer;
		mToString[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::ToStringRTTIPointer;

		mSetFromString[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::SetFromStringUnassigned;
		mSetFromString[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::SetFromStringInt;
		mSetFromString[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::SetFromStringFloat;
		mSetFromString[static_cast<uint32_t>(DatumType::STRING)] = &Datum::SetFromStringString;
		mSetFromString[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::SetFromStringVec4;
		mSetFromString[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::SetFromStringMat4x4;
		mSetFromString[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::SetFromStringRTTIPointer;
		mSetFromString[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::SetFromStringRTTIPointer;

		mPerformDeepCopy[static_cast<uint32_t>(DatumType::UNASSIGNED)] = &Datum::PerformDeepCopyUnassigned;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::INT32_T)] = &Datum::PerformDeepCopyInt;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::FLOAT)] = &Datum::PerformDeepCopyFloat;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::STRING)] = &Datum::PerformDeepCopyString;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = &Datum::PerformDeepCopyVec4;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = &Datum::PerformDeepCopyMat4x4;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::POINTER)] = &Datum::PerformDeepCopyRTTIPointer;
		mPerformDeepCopy[static_cast<uint32_t>(DatumType::TABLE)] = &Datum::PerformDeepCopyRTTIPointer;
	}

	uint32_t Datum::ReserveStrategy(const uint32_t capacity) const
	{
		return std::max<uint32_t>(1U, capacity + capacity);
	}

#pragma region Search based on each data type(Declarations)
	bool Datum::PerformVoidDeepCompare(const DatumValues& rhsDatumValues) const
	{
		return true;
		rhsDatumValues;
	}

	bool Datum::PerformIntDeepCompare(const DatumValues& rhsDatumValues) const
	{
		return PerformDeepCompare(mDatumValues.intPointer, rhsDatumValues.intPointer);
	}

	bool Datum::PerformFloatDeepCompare(const DatumValues& rhsDatumValues) const
	{
		return PerformDeepCompare(mDatumValues.floatPointer, rhsDatumValues.floatPointer);
	}

	bool Datum::PerformStringDeepCompare(const DatumValues& rhsDatumValues) const
	{
		return PerformDeepCompare(mDatumValues.stringPointer, rhsDatumValues.stringPointer);
	}

	bool Datum::PerformVec4DeepCompare(const DatumValues& rhsDatumValues) const
	{
		return PerformDeepCompare(mDatumValues.vector4Pointer, rhsDatumValues.vector4Pointer);
	}

	bool Datum::PerformMat4x4DeepCompare(const DatumValues& rhsDatumValues) const
	{
		return PerformDeepCompare(mDatumValues.mat4x4Pointer, rhsDatumValues.mat4x4Pointer);
	}

	bool Datum::PerformRTTIPointerDeepCompare(const DatumValues& rhsDatumValues) const
	{
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (!(mDatumValues.rttiPointer[i]->Equals(rhsDatumValues.rttiPointer[i])))
			{
				return false;
			}
		}
		return true;
	}
#pragma endregion

#pragma region Pushbacking based on each data type	
	void Datum::PushBackIntData(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(int32_t());
		}
	}

	void Datum::PushBackFloatData(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(std::float_t());
		}
	}

	void Datum::PushBackStringData(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(string());
		}
	}

	void Datum::PushBackVec4Data(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(vec4());
		}
	}

	void Datum::PushBackMat4x4Data(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(mat4x4());
		}
	}

	void Datum::PushBackRTTIPointer(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(static_cast<RTTI*>(nullptr));
		}
	}

	void Datum::PushBackScopePointer(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			PushBack(static_cast<Scope*>(nullptr));
		}
	}
#pragma endregion

#pragma region Destructing for each data type
	void Datum::DestructPrimitiveTypes(const uint32_t startIndex, const uint32_t endIndex)
	{
		startIndex;
		endIndex;
	}

	void Datum::DestructStringData(const uint32_t startIndex, const uint32_t endIndex)
	{
		for (uint32_t i = startIndex; i < endIndex; ++i)
		{
			(mDatumValues.stringPointer + i)->~string();
		}
	}

#pragma endregion

#pragma region Converting each data type to std::string Implementations
	void Datum::ToStringUnassigned(const uint32_t index, std::string& convertedString) const
	{
		throw exception("std::string Datum::ToStringUnassigned() const: Cannot convert UNASSIGNED Datum to std::string.");
		index;
		convertedString;
	}

	void Datum::ToStringInt(const uint32_t index, std::string& convertedString) const
	{
		convertedString = std::to_string(*(mDatumValues.intPointer + index));
	}

	void Datum::ToStringFloat(const uint32_t index, std::string& convertedString) const
	{
		convertedString = std::to_string(*(mDatumValues.floatPointer + index));
	}

	void Datum::ToStringString(const uint32_t index, std::string& convertedString) const
	{
		convertedString = *(mDatumValues.stringPointer + index);
	}

	void Datum::ToStringVec4(const uint32_t index, std::string& convertedString) const
	{
		convertedString = glm::to_string(*(mDatumValues.vector4Pointer + index));
	}

	void Datum::ToStringMat4x4(const uint32_t index, std::string& convertedString) const
	{
		convertedString = glm::to_string(*(mDatumValues.mat4x4Pointer + index));
	}

	void Datum::ToStringRTTIPointer(const uint32_t index, std::string& convertedString) const
	{
		convertedString = mDatumValues.rttiPointer[index] != nullptr ? mDatumValues.rttiPointer[index]->ToString() : string();
	}

#pragma endregion

#pragma region Set from std::string to specific data type Implementations
	void Datum::SetFromStringUnassigned(const std::string& inputString, const std::uint32_t index)
	{
		throw exception("void Datum::SetFromStringUnassigned(std::string & inputString, const std::uint32_t index): Cannot set data to an UNASSIGNED Datum object.");
		inputString;
		index;
	}

	void Datum::SetFromStringInt(const string& inputString, const uint32_t index)
	{
		int32_t intValue;
		sscanf_s(inputString.c_str(), "%d", &intValue);
		Set(intValue, index);
	}

	void Datum::SetFromStringFloat(const std::string& inputString, const uint32_t index)
	{
		float floatValue;
		sscanf_s(inputString.c_str(), "%f", &floatValue);
		Set(floatValue, index);
	}

	void Datum::SetFromStringString(const std::string& inputString, const uint32_t index)
	{
		Set(inputString, index);
	}

	void Datum::SetFromStringVec4(const std::string& inputString, const uint32_t index)
	{
		vec4 vec4Value;
		sscanf_s(inputString.c_str(), "vec4(%f, %f, %f, %f)", &vec4Value[0], &vec4Value[1], &vec4Value[2], &vec4Value[3]);
		Set(vec4Value, index);
	}

	void Datum::SetFromStringMat4x4(const std::string& inputString, const uint32_t index)
	{
		mat4x4 mat4x4Value;
		sscanf_s(inputString.c_str(), "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
			&mat4x4Value[0][0], &mat4x4Value[0][1], &mat4x4Value[0][2], &mat4x4Value[0][3],
			&mat4x4Value[1][0], &mat4x4Value[1][1], &mat4x4Value[1][2], &mat4x4Value[1][3],
			&mat4x4Value[2][0], &mat4x4Value[2][1], &mat4x4Value[2][2], &mat4x4Value[2][3],
			&mat4x4Value[3][0], &mat4x4Value[3][1], &mat4x4Value[3][2], &mat4x4Value[3][3]);
		Set(mat4x4Value, index);
	}

	void Datum::SetFromStringRTTIPointer(const std::string& inputString, const std::uint32_t index)
	{
		throw exception("void Datum::SetFromStringRTTIPointer(std::string & inputString, const std::uint32_t index): Cannot set a pointer from string.");
		inputString;
		index;
	}
#pragma endregion

#pragma region Perform deep copy for each type Implementations
	void Datum::PerformDeepCopyUnassigned(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		return;
		rhsDatumValues;
		size;
	}

	void Datum::PerformDeepCopyInt(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		memcpy(mDatumValues.intPointer, rhsDatumValues.intPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * size);
	}

	void Datum::PerformDeepCopyFloat(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		memcpy(mDatumValues.floatPointer, rhsDatumValues.floatPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * size);
	}

	void Datum::PerformDeepCopyString(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			new(mDatumValues.stringPointer + i)string(rhsDatumValues.stringPointer[i]);
		}
	}

	void Datum::PerformDeepCopyVec4(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		memcpy(mDatumValues.floatPointer, rhsDatumValues.floatPointer, mDataTypeSizes[static_cast<uint32_t>(mDatumType)] * size);
		for (uint32_t i = 0; i < size; ++i)
		{
			new(mDatumValues.vector4Pointer + i)vec4(rhsDatumValues.vector4Pointer[i]);
		}
	}

	void Datum::PerformDeepCopyMat4x4(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			new(mDatumValues.mat4x4Pointer + i)mat4x4(rhsDatumValues.mat4x4Pointer[i]);
		}
	}

	void Datum::PerformDeepCopyRTTIPointer(const DatumValues& rhsDatumValues, const uint32_t size)
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			mDatumValues.rttiPointer[i] = rhsDatumValues.rttiPointer[i];
		}
	}
#pragma endregion

#pragma endregion
}