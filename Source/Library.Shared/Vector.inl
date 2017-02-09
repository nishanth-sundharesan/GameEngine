#include "Pch.h"
#include <assert.h>
#include <algorithm>
#ifndef VectorInitialized
#include "Vector.h"
#endif

using namespace std;

namespace GameEngineLibrary
{
	/************************************************************************/
	/* BEGIN	-Vector class-		Constructors & Functions.				*/
	/************************************************************************/	
	template<class T, typename F>
	Vector<T, F>::Vector(uint32_t capacity) :
		mSize(0), mStartingAddress(nullptr), mCapacity(0)
	{		
		Reserve(capacity);
	}

	template<class T, typename F>
	Vector<T, F>::Vector(const Vector<T, F>& rhs) :
		mSize(0), mStartingAddress(nullptr), mCapacity(0)
	{
		Reserve(rhs.mCapacity);
		for (uint32_t i = 0; i < rhs.mSize; ++i)
		{
			PushBack(rhs[i]);
		}
	}

	template<class T, typename F>
	inline Vector<T, F>& Vector<T, F>::operator=(const Vector<T, F>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mCapacity = 0;
			Reserve(rhs.mCapacity);

			for (uint32_t i = 0; i < rhs.mSize; ++i)
			{
				PushBack(rhs[i]);
			}
		}

		return *this;
	}

	template<class T, typename F>
	Vector<T, F>::~Vector()
	{
		Clear();
		ShrinkToFit();
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::PushBack(const T& value)
	{
		//Even this is being shared across all instances of the Vector class		
		static F reserveStrategy;
		if (mSize == mCapacity)
		{
			uint32_t capacity = max<uint32_t>(1U, reserveStrategy(mSize, mCapacity));
			Reserve(capacity);
		}

		//Placement new - Calls only the constructor. Doesn't allocate memory
		new(mStartingAddress + mSize)T(value);

		return Iterator(this, mSize++);
	}

	template<class T, typename F>
	inline bool Vector<T, F>::PopBack()
	{
		if (IsEmpty())
		{
			return false;
		}

		//Calls the destructor of the last element in the array
		(mStartingAddress + (--mSize))->~T();

		return true;
	}

	template<class T, typename F>
	inline bool Vector<T, F>::PopBack(T& value)
	{
		if (IsEmpty())
		{
			return false;
		}

		value = *(mStartingAddress + mSize - 1);
		//Calls the destructor of the last element in the array
		(mStartingAddress + (--mSize))->~T();

		return true;
	}

	template<class T, typename F>
	inline bool Vector<T, F>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template<class T, typename F>
	inline T& Vector<T, F>::Front()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T,F>::Front(): Vector is empty!");
		}

		return *mStartingAddress;
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::Front() const
	{
		if (IsEmpty())
		{
			throw out_of_range(" const T& Vector<T,F>::Front() const: Vector is empty!");
		}

		return *mStartingAddress;
	}

	template<class T, typename F>
	inline T& Vector<T, F>::Back()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T,F>::Back(): Vector is empty!");
		}

		return *(mStartingAddress + mSize - 1);
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::Back() const
	{
		if (IsEmpty())
		{
			throw out_of_range("const T& Vector<T,F>::Back() const: Vector is empty!");
		}

		return *(mStartingAddress + mSize - 1);
	}

	template<class T, typename F>
	inline uint32_t Vector<T, F>::Size() const
	{
		return mSize;
	}

	template<class T, typename F>
	inline uint32_t Vector<T, F>::Capacity() const
	{
		return mCapacity;
	}

	template<class T, typename F>
	inline void Vector<T, F>::Clear()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::begin() const
	{
		return Iterator(this, 0);
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::end() const
	{
		return Iterator(this, mSize);
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::Find(const T& value) const
	{
		Iterator iterator = begin();
		for (uint32_t i = 0; i < mSize; ++i, ++iterator)
		{
			if (operator[](i) == value)
			{
				return iterator;
			}
		}
		return end();
	}

	template<class T, typename F>
	inline bool Vector<T, F>::Remove(const T& value)
	{
		Iterator iterator = Find(value);
		if (iterator == end())
		{
			return false;
		}

		(*iterator).~T();


		auto size = (mSize - iterator.mIndex - 1) * sizeof(T);
		if (size > 0)																					//Checking if the removed item was not the last item of the Vector array.
		{
			memmove(mStartingAddress + iterator.mIndex, mStartingAddress + iterator.mIndex + 1, size);
		}

		--mSize;

		return true;
	}

	template<class T, typename F>
	inline bool Vector<T, F>::Remove(const Iterator& startIterator, const Iterator& endIterator)
	{
		if (startIterator.mOwner != this || endIterator.mOwner != this)
		{
			throw out_of_range("bool Vector<T,F>::Remove(const Iterator& startIterator, const Iterator& endIterator): One or more Iterators belong to a different Vector!");
		}
		if (startIterator.mIndex >= mSize || endIterator.mIndex > mSize)
		{
			throw out_of_range("bool Vector<T,F>::Remove(const Iterator& startIterator, const Iterator& endIterator): One or more iterators point to an invalid data!");
		}
		if (startIterator.mIndex >= endIterator.mIndex)
		{
			throw out_of_range("bool Vector<T,F>::Remove(const Iterator& startIterator, const Iterator& endIterator): start Iterator is greater than or equal to the end Iterator!");
		}

		Iterator temp = startIterator;
		uint32_t tempCount = 0;
		for (; temp != endIterator; ++temp)
		{
			(*temp).~T();
			tempCount++;
		}

		if ((endIterator.mIndex + 1) < mSize)
		{
			memmove(mStartingAddress + startIterator.mIndex, mStartingAddress + endIterator.mIndex, (mSize - endIterator.mIndex) * sizeof(T));
		}

		mSize -= tempCount;
		return true;
	}

	template<class T, typename F>
	inline T& Vector<T, F>::operator[](const uint32_t index)
	{
		if (index >= mSize)
		{
			throw out_of_range("T& operator[](const uint32_t index): Trying to access an index out of range of the Vector.");
		}

		return *(mStartingAddress + index);
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::operator[](const uint32_t index) const
	{
		if (index >= mSize)
		{
			throw out_of_range("const T& Vector<T,F>::operator[](const uint32_t index) const: Trying to access an index out of range of the Vector.");
		}

		return *(mStartingAddress + index);
	}

	template<class T, typename F>
	inline T& Vector<T, F>::At(const std::uint32_t index)
	{
		return operator[](index);
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::At(const uint32_t index) const
	{
		return operator[](index);
	}

	template<class T, typename F>
	inline void Vector<T, F>::Reserve(uint32_t capacity)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			mStartingAddress = static_cast<T*>(realloc(mStartingAddress, sizeof(T) * capacity));									//Realloc returns nullptr if the capacity is 0 or if it failed to allocate memory.
			if (mStartingAddress == nullptr)
			{
				throw exception("void Vector<T,F>::Reserve(std::uint32_t capacity): Failed to allocate memory!");
			}
			mCapacity = capacity;
		}
	}

	template<class T, typename F>
	inline void Vector<T, F>::ShrinkToFit()
	{
		if (mSize != mCapacity)
		{
			//We are not checking if mStartingAddress == nullptr as mSize can be 0
			mStartingAddress = static_cast<T*>(realloc(mStartingAddress, sizeof(T) * mSize));
			mCapacity = mSize;
		}
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/


	/************************************************************************/
	/* BEGIN	-Iterator class-		Constructors & Functions.			*/
	/************************************************************************/
	template<class T, typename F>
	inline Vector<T, F>::Iterator::Iterator() :
		mOwner(nullptr), mIndex(0)
	{

	}

	template<class T, typename F>
	inline Vector<T, F>::Iterator::Iterator(const Vector* owner, uint32_t index) :
		mOwner(owner), mIndex(index)
	{

	}

	template<class T, typename F>
	inline bool Vector<T, F>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mOwner == rhs.mOwner) && (mIndex == rhs.mIndex));
	}

	template<class T, typename F>
	inline bool Vector<T, F>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator& Vector<T, F>::Iterator::operator++()
	{
		if (mOwner == nullptr || mIndex >= mOwner->mSize)
		{
			throw out_of_range("Vector<T,F>::Iterator& Vector<T,F>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}
		++mIndex;

		return (*this);
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::Iterator::operator++(int)
	{
		//Make a copy of the current Iterator
		Iterator temp(*this);

		//Increment the current Iterator
		operator++();

		//Return the copy as a VALUE and NOT by REFERENCE.
		return temp;
	}

	template<class T, typename F>
	inline T& Vector<T, F>::Iterator::operator*()
	{
		if (mOwner == nullptr || mIndex >= mOwner->mSize)
		{
			throw out_of_range("T& Vector<T,F>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *(mOwner->mStartingAddress + mIndex);
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::Iterator::operator*() const
	{
		if (mOwner == nullptr || mIndex >= mOwner->mSize)
		{
			throw out_of_range("T& Vector<T,F>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *(mOwner->mStartingAddress + mIndex);
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
}