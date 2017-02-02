#include "Pch.h"
#include <assert.h>
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
	Vector<T, F>::Vector() :
		mSize(0), mStartingAddress(nullptr), mCapacity(0), mDefaultCapacity(10)
	{

	}

	template<class T, typename F>
	Vector<T, F>::Vector(uint32_t capacity) :
		mSize(0), mStartingAddress(nullptr), mCapacity(0), mDefaultCapacity(capacity)
	{
		Reserve(capacity);
	}

	template<class T, typename F>
	Vector<T, F>::Vector(const Vector<T, F>& rhs) :
		mSize(0), mStartingAddress(nullptr)
	{
		operator=(rhs);

		/*for (uint32_t i = 0; i < rhs.mSize; ++i)
		{
			PushBack(rhs[i]);
		}*/
	}


	/*Vector<T,F>::Vector()
	{
	}*/

	template<class T, typename F>
	inline Vector<T, F>& Vector<T, F>::operator=(const Vector<T, F>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mCapacity = 0;
			mDefaultCapacity = rhs.mDefaultCapacity;
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
		//static IncrementFunctor incrementFunctor;

		if (mSize == mCapacity)
		{
			//triples the capacity
			//std:uint32_t capacity = mCapacity + max<uint32_t>(1U,incrementFunctor(mSize,mCapacity));
			Reserve(mCapacity == 0 ? mDefaultCapacity : (mCapacity + mCapacity));
		}

		//Placement new - Calls only the constructor. Doesn't allocate memory
		new(mStartingAddress + mSize)T(value);


		//return Iterator(this, mStartingAddress + mSize++);
		++mSize;
		return Iterator(this, mStartingAddress + mSize - 1);
	}

	template<class T, typename F>
	inline bool Vector<T, F>::PopBack()
	{
		if (IsEmpty())
		{
			return false;
		}

		//Calls the destructor of the last element in the array
		//(mStartingAddress + (--mSize))->~T();
		(mStartingAddress + mSize - 1)->~T();
		--mSize;

		return true;
	}

	template<class T, typename F>
	inline bool Vector<T, F>::PopBack(T& value)
	{
		if (IsEmpty())
		{
			return false;
		}

		//Calls the destructor of the last element in the array
		value = *(mStartingAddress + mSize - 1);
		(mStartingAddress + mSize - 1)->~T();
		--mSize;

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
		//return operator[](0);
	}

	template<class T, typename F>
	inline T& Vector<T, F>::Back()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T,F>::Back(): Vector is empty!");
		}

		return *(mStartingAddress + mSize - 1);
		//return operator[](size -1);
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
		return Iterator(this, mStartingAddress);
		//return Iterator(this, 0);
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator Vector<T, F>::end() const
	{
		return Iterator(this, nullptr);
		//return Iterator(this, size);
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

		//auto size = (mSize - it.mIndex-1)*sizeof(T);
		//if(size > 0)
		//Checking if the removed item was not the last item of the Vector array.
		if (iterator.mCurrentNode + 1 != mStartingAddress + mSize)
		{
			memmove(iterator.mCurrentNode, iterator.mCurrentNode + 1, sizeof(T) * (mSize - 1));
			//If we use the above one SDL life cycle will throw warning
			//memmove_s(iterator.mCurrentNode, iterator.mCurrentNode + 1, sizeof(T) * (mSize - 1));
		}
		--mSize;

		return true;
	}

	template<class T, typename F>
	inline bool Vector<T, F>::Remove(const Iterator& startIterator, const Iterator& endIterator)
	{
		if (startIterator.mOwner != this || endIterator.mOwner != this)
		{
			throw exception("bool Vector<T,F>::Remove(const Iterator& startIterator, const Iterator& endIterator): One or more of the Iterators belong to a different Vector!");
		}
		if ((startIterator == end()) || (endIterator != end() && startIterator.mCurrentNode >= endIterator.mCurrentNode))
		{
			throw exception("bool Vector<T,F>::Remove(const Iterator& startIterator, const Iterator& endIterator): start Iterator is greater than or equal to the end Iterator!");
		}

		Iterator temp = startIterator;
		uint32_t tempCount = 0;
		for (; temp != endIterator; ++temp)
		{
			(*temp).~T();
			tempCount++;
		}

		//Checking if there are any data left after the endIterator
		if (endIterator != end() && endIterator.mCurrentNode + 1 != mStartingAddress + mSize)
		{
			memmove(startIterator.mCurrentNode, endIterator.mCurrentNode, sizeof(T) * ((mStartingAddress + mSize) - endIterator.mCurrentNode));
		}

		mSize = mSize - tempCount;
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
		mOwner(nullptr), mCurrentNode(nullptr)
	{

	}

	template<class T, typename F>
	inline Vector<T, F>::Iterator::Iterator(const Vector* owner, T* currentNode) :
		mOwner(owner), mCurrentNode(currentNode)
	{

	}

	template<class T, typename F>
	inline bool Vector<T, F>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mOwner == rhs.mOwner) && (mCurrentNode == rhs.mCurrentNode));
	}

	template<class T, typename F>
	inline bool Vector<T, F>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<class T, typename F>
	inline typename Vector<T, F>::Iterator& Vector<T, F>::Iterator::operator++()
	{
		if (mCurrentNode == nullptr)
		{
			throw out_of_range("Vector<T,F>::Iterator& Vector<T,F>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		if (mCurrentNode + 1 == mOwner->mStartingAddress + mOwner->mSize)
		{
			//Iterator is now pointing to the end of the list.
			mCurrentNode = nullptr;
		}
		else
		{
			++mCurrentNode;
		}

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
		if (mCurrentNode == nullptr)
		{
			throw exception("T& Vector<T,F>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *mCurrentNode;
	}

	template<class T, typename F>
	inline const T& Vector<T, F>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("T& Vector<T,F>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *mCurrentNode;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
}