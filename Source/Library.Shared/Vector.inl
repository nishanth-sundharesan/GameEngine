#include "Pch.h"	
#ifndef VectorInitialized
#include "Vector.h"
#endif

using namespace std;

namespace GameEngineLibrary
{
	/************************************************************************/
	/* BEGIN	-Vector class-		Constructors & Functions.				*/
	/************************************************************************/
	template<class T>
	Vector<T>::Vector() :
		mSize(0), mStartingAddress(nullptr), mCapacity(0)
	{
	}

	template<class T>
	Vector<T>::Vector(uint32_t capacity) :
		mSize(0), mStartingAddress(nullptr), mCapacity(0)
	{
		Reserve(capacity);
	}

	template<class T>
	Vector<T>::Vector(const Vector<T>& rhs) :
		mSize(0), mStartingAddress(nullptr), mCapacity(0)
	{
		operator=(rhs);
	}

	template<class T>
	Vector<T>::~Vector()
	{
		Clear();
		ShrinkToFit();
	}

	template<class T>
	inline bool Vector<T>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template<class T>
	inline uint32_t Vector<T>::Size() const
	{
		return mSize;
	}

	template<class T>
	inline uint32_t Vector<T>::Capacity() const
	{
		return mCapacity;
	}

	template<class T>
	inline Vector<T>& Vector<T>::operator=(const Vector<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mCapacity = rhs.mCapacity;

			for (const auto& value : rhs)
			{
				PushBack(value);
			}
		}

		return *this;
	}

	template<class T>
	inline typename Vector<T>::Iterator Vector<T>::PushBack(const T& value)
	{
		if (mSize == mCapacity)
		{
			Reserve(mCapacity + mCapacity);
		}

		new(mStartingAddress + mSize)T(data);

		++mSize;
		return Iterator(this, mStartingAddress + mSize - 1);
	}

	template<class T>
	inline T& Vector<T>::operator[](const uint32_t index)
	{
		if (index >= mSize)
		{
			throw out_of_range("T& operator[](const uint32_t index): Trying to access an index out of range of the Vector.");
		}

		return *(mStartingAddress + index);
	}

	template<class T>
	inline const T& Vector<T>::operator[](const uint32_t index) const
	{
		if (index >= mSize)
		{
			throw out_of_range("T& operator[](const uint32_t index): Trying to access an index out of range of the Vector.");
		}

		return *(mStartingAddress + index);
	}

	template<class T>
	inline T& Vector<T>::At(const std::uint32_t index)
	{
		return operator[](index);
	}

	template<class T>
	inline const T& Vector<T>::At(const uint32_t index) const
	{
		return operator[](index);
	}

	template<class T>
	inline T& Vector<T>::Front()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T>::Front(): Vector is empty!");
		}

		return *mStartingAddress;
	}

	template<class T>
	inline const T& Vector<T>::Front() const
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T>::Front(): Vector is empty!");
		}

		return *mStartingAddress;
	}

	template<class T>
	inline T& Vector<T>::Back()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Vector<T>::Back(): Vector is empty!");
		}

		return *(mStartingAddress + mSize - 1);
	}

	template<class T>
	inline const T& Vector<T>::Back() const
	{
		if (IsEmpty())
		{
			throw out_of_range("const T& Vector<T>::Back() const: Vector is empty!");
		}

		return *(mStartingAddress + mSize - 1);
	}

	template<class T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value) const
	{
		Iterator iterator = begin();
		for (; iterator != end(); ++iterator)
		{
			if ((*iterator) == value)
			{
				break;
			}
		}
		return iterator;
	}

	template<class T>
	inline bool Vector<T>::Remove(const T& value)
	{
		Iterator iterator = Find(value);
		if (iterator != end())
		{
			(*iterator).~();

			//Checking if the removed item was not the item that was at the end of the list
			if (iterator.mCurrentNode + 1 != mStartingAddress + mSize)
			{
				memmove(iterator.mCurrentNode, iterator.mCurrentNode + 1, (mStartingAddress + mSize) - (iterator.mCurrentNode + 1));
			}
			--mSize;
		}
	}

	template<class T>
	inline bool Vector<T>::Remove(const Iterator& start, const Iterator& end)
	{
		if (start.mOwner != this || end.mOwner == this)
		{
			throw exception("bool Vector<T>::Remove(const Iterator& start, const Iterator& end): One or more of the Iterators belong to a different Vector!");
		}
		if (start.mCurrentNode > end.mCurrentNode)
		{
			throw exception("bool Vector<T>::Remove(const Iterator& start, const Iterator& end): start Iterator is greater than the end Iterator!");
		}

		Iterator temp = start;
		for (; temp != end; ++temp)
		{
			(*temp).~();
		}

		//Checking if there are any data left after the Iterator "end"
		if (end.mCurrentNode + 1 != mStartingAddress + mSize)
		{
			memmove(start.mCurrentNode, end.mCurrentNode + 1, (mStartingAddress + mSize) - (end.mCurrentNode));
		}
	}

	template<class T>
	inline bool Vector<T>::PopBack()
	{
		if (IsEmpty())
		{
			return false;
		}

		*(mStartingAddress + size - 1).~();
		--mSize;

		return true;
	}

	template<class T>
	inline bool Vector<T>::PopBack(const T& value)
	{
		if (IsEmpty())
		{
			return false;
		}

		data = *(mStartingAddress + size - 1);
		*(mStartingAddress + size - 1).~();
		--mSize;

		return true;
	}

	template<class T>
	inline typename Vector<T>::Iterator Vector<T>::begin() const
	{
		return Iterator(this, mStartingAddress);
	}

	template<class T>
	inline typename Vector<T>::Iterator Vector<T>::end() const
	{
		return Iterator(this, nullptr);
	}

	template<class T>
	inline void Vector<T>::Clear()
	{
		while (!IsEmpty())
		{
			PopBack();
		}
	}

	template<class T>
	inline void Vector<T>::ShrinkToFit()
	{
		if (mSize != mCapacity)
		{
			//We are not checking if mStartingAddress == nullptr as mSize can be 0
			mStartingAddress = (T*)realloc(mStartingAddress, sizeof(T) * mSize);
			mCapacity = mSize;
		}
	}

	template<class T>
	inline void Vector<T>::Reserve(uint32_t capacity)
	{
		if (capacity > mCapacity)
		{
			assert(capacity != 0);
			mStartingAddress = (T*)realloc(mStartingAddress, sizeof(T) * capacity);									//Realloc returns nullptr if the capacity is 0 or if it failed to allocate memory.
			if (mStartingAddress == nullptr)
			{
				throw exception("void Vector<T>::Reserve(std::uint32_t capacity): Failed to allocate memory!");
			}
			mCapacity = capacity;
		}
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/


	/************************************************************************/
	/* BEGIN	-Iterator class-		Constructors & Functions.			*/
	/************************************************************************/
	template<class T>
	inline Vector<T>::Iterator::Iterator() :
		mOwner(nullptr), mCurrentNode(nullptr)
	{

	}

	template<class T>
	inline Vector<T>::Iterator::Iterator(const Vector* owner, T* currentNode) :
		mOwner(owner), mCurrentNode(currentNode)
	{

	}

	template<class T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mOwner == rhs.mOwner) && (mCurrentNode == rhs.mCurrentNode));
	}

	template<class T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<class T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mCurrentNode == nullptr)
		{
			throw out_of_range("Vector<T>::Iterator& Vector<T>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		++mCurrentNode;

		if (mCurrentNode > mStartingAddress + mSize - 1)
		{
			//Iterator is now pointing to the end of the list.
			mCurrentNode = nullptr;
		}

		return (*this);
	}

	template<class T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		//Make a copy of the current Iterator
		Iterator temp(*this);

		//Increment the current Iterator
		operator++();

		//Return the copy as a VALUE and NOT by REFERENCE.
		return temp;
	}

	template<class T>
	inline T& Vector<T>::Iterator::operator*()
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("T& Vector<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *mCurrentNode;
	}

	template<class T>
	inline const T& Vector<T>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("T& Vector<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		return *mCurrentNode;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
}