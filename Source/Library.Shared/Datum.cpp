#include "Pch.h"
#include "Datum.h"
#include <assert.h>

namespace GameEngineLibrary
{
	Datum::Datum()
		:isInternalStorage(true), mSize(0), mCapacity(0), mDatumType(DatumType::UNASSIGNED)
	{
	}

	DatumType Datum::Type() const
	{
		return mDatumType;
	}

	void Datum::SetType(DatumType datumType)
	{
		if (mDatumType == DatumType::UNASSIGNED || mDatumType == datumType)
		{
			mDatumType = datumType;
			//return;
		}
		//TODO-> throw exception here
	}

	std::uint32_t Datum::Size() const
	{
		return mSize;
	}

	void Datum::SetSize(uint32_t size)
	{
		//This code could come here
		//if (mDatumType == DatumType::UNASSIGNED)
		//{
			//TODO-> throw exception here
		//}

		if (isInternalStorage)
		{

			if (size > mCapacity)
			{
				Reserve(mCapacity);
			}
			else if (size >= 0 && size < mSize)
			{
				for (uint32_t i = size; i < mSize; ++i)
				{
					//Call Destructor here
					//(mDatumValues.voidPointer + i)->
				}
				mSize = size;
			}
		}
	}

	void Datum::Clear()
	{
		if (isInternalStorage)
		{
			for (uint32_t i = 0; i < mSize; ++i)
			{
				//Call Destructor here
				//(mDatumValues.voidPointer + i)->
			}
		}
		mSize = 0;

	}

	void Datum::SetStorage(int32_t* pointerToData, uint32_t size)
	{
		if (isInternalStorage)
		{
			//throw exception here
		}
		if (mDatumType != DatumType::INT32_T)
		{
			//throw exception here
		}
		mDatumValues.intPointer = pointerToData;
		mSize = size;
		isInternalStorage = false;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(float* pointerToData, uint32_t size)
	{
		if (isInternalStorage)
		{
			//throw exception here
		}
		if (mDatumType != DatumType::FLOAT)
		{
			//throw exception here
		}
		mDatumValues.floatPointer = pointerToData;
		mSize = size;
		isInternalStorage = false;
		assert(mCapacity != 0);
	}

	void Datum::SetStorage(std::string* pointerToData, uint32_t size)
	{
		if (isInternalStorage)
		{
			//throw exception here
		}
		if (mDatumType != DatumType::STRING)
		{
			//throw exception here
		}
		mDatumValues.stringPointer = pointerToData;
		mSize = size;
		isInternalStorage = false;
		assert(mCapacity != 0);
	}

	bool Datum::operator==(const Datum& rhs) const
	{
		if (isInternalStorage != rhs.isInternalStorage || mDatumType != rhs.mDatumType || mSize != rhs.mSize || mCapacity != rhs.mCapacity)
		{
			return false;
		}
		for (uint32_t i = 0; i < mSize; ++i)
		{
			switch (mDatumType)
			{
				//return PerformDeepSearch(mDatumValues.voidPointer, rhs.mDatumValues.voidPointer, mSize);

			case DatumType::INT32_T:
				return PerformDeepSearch(mDatumValues.intPointer, rhs.mDatumValues.intPointer, mSize);
				break;
			case DatumType::FLOAT:
				return PerformDeepSearch(mDatumValues.floatPointer, rhs.mDatumValues.floatPointer, mSize);
				break;
			case DatumType::STRING:
				break;
			}
		}
		//temporary
		return false;
	}

	/*template<>
	bool Datum::PerformDeepSearch<int32_t>(int32_t* lhs, int32_t *rhs, uint32_t size) const
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			if (*(lhs + i) != *(rhs + i))
			{
				return false;
			}
		}
		return true;
	}

	template<>
	bool Datum::PerformDeepSearch<float>(float* lhs, float *rhs, uint32_t size) const
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			if (*(lhs + i) != *(rhs + i))
			{
				return false;
			}
		}
		return true;
	}*/

	bool Datum::operator==(const int32_t& intValue) const
	{
		//return (mDatumType == DatumType::INT32_T && mSize == 1 && *(mDatumValues.intPointer) == intValue);
		if (mDatumType == DatumType::INT32_T && mSize == 1 && *(mDatumValues.intPointer) == intValue)
		{
			return true;
		}
		return false;
	}

	bool Datum::operator==(const float& floatValue) const
	{
		if (mDatumType == DatumType::FLOAT && mSize == 1 && *(mDatumValues.floatPointer) == floatValue)
		{
			return true;
		}
		return false;
	}

	bool Datum::operator==(const std::string& stringValue) const
	{
		if (mDatumType == DatumType::STRING && mSize == 1 && *(mDatumValues.stringPointer) == stringValue)
		{
			return true;
		}
		return false;
	}

	void Datum::Set(int32_t value, uint32_t index)
	{
		if (mDatumType != DatumType::INT32_T)
		{
			//throw exception here
		}
		if (index < mSize)
		{
			*(mDatumValues.intPointer + index) = value;
		}
		//throw out of range exception
	}

	void Datum::Set(float value, uint32_t index)
	{
		if (mDatumType != DatumType::FLOAT)
		{
			//throw exception here
		}
		if (index < mSize)
		{
			*(mDatumValues.floatPointer + index) = value;
		}
	}

	void Datum::Set(std::string value, uint32_t index)
	{
		if (mDatumType != DatumType::STRING)
		{
			//throw exception here
		}
		if (index < mSize)
		{
			*(mDatumValues.stringPointer + index) = value;
		}
	}

	template<>
	int32_t Datum::Get<int32_t>(uint32_t index) const
	{
		if (mDatumType == DatumType::UNASSIGNED)
		{
			//throw exception here
		}

		if (index >= mSize)
		{
			//throw exception here
		}

		return *(mDatumValues.intPointer);
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

		return *(mDatumValues.floatPointer);
	}

	void Datum::Reserve(std::uint32_t capacity)
	{
		assert(capacity != 0);
		//mDatumValues.voidPointer = realloc(mDatumValues.voidPointer,)
		//mCapacity = capacity;
	}
}