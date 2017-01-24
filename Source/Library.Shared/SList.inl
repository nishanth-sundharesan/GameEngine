#include "Pch.h"
#ifndef SListInitialized
#include "SList.h"
#endif

using namespace std;

namespace GameEngineLibrary
{
	/************************************************************************/
	/* BEGIN	-SList class-		Constructors & Functions.				*/
	/************************************************************************/
	template <class T>
	SList<T>::SList() :
		mBack(nullptr), mFront(nullptr), mSize(0)
	{

	}

	template <class T>
	SList<T>::SList(const SList<T>& rhs) :
		mBack(nullptr), mFront(nullptr), mSize(0)
	{
		Node* tempFrontNode = rhs.mFront;
		while (tempFrontNode != nullptr)
		{
			PushBack(tempFrontNode->mData);
			tempFrontNode = tempFrontNode->mNext;
		}
	}

	template <class T>
	inline SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();													//Clear the entire list before assigning new data
			Node* tempFrontNode = rhs.mFront;
			while (tempFrontNode != nullptr)
			{
				PushBack(tempFrontNode->mData);
				tempFrontNode = tempFrontNode->mNext;
			}
		}

		return *this;
	}

	template <class T>
	SList<T>::~SList()
	{
		Clear();
	}

	template <class T>
	inline void SList<T>::PushFront(const T& data)
	{
		Node* newNode = new Node(data, mFront);
		mFront = newNode;															//Always make the next pointer to point to the front of the list

		if (IsEmpty())
		{
			mBack = newNode;
		}

		++mSize;
	}

	template <class T>
	inline void SList<T>::PushBack(const T& data)
	{
		Node* newNode = new Node(data, nullptr);

		if (IsEmpty())
		{
			mFront = newNode;
			mBack = newNode;
		}
		else
		{
			mBack->mNext = newNode;
			mBack = mBack->mNext;
		}

		++mSize;
	}

	template <class T>
	inline void SList<T>::PopFront()
	{
		if (IsEmpty())
		{
			throw exception("void PopFront(): SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->mNext;

		delete tempNode;
		tempNode = nullptr;

		--mSize;
	}

	template <class T>
	inline void SList<T>::PopFront(T& data)
	{
		if (IsEmpty())
		{
			throw exception("void PopFront(T& ): SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->mNext;

		data = tempNode->mData;

		delete tempNode;
		tempNode = nullptr;

		--mSize;
	}

	template <class T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <class T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw exception("T& Front(): SList is empty!");
		}

		return mFront->mData;
	}

	template <class T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw exception("const T& Front(): SList is empty!");
		}

		return mFront->mData;
	}

	template <class T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw exception("T& Back(): SList is empty!");
		}

		return mBack->mData;
	}

	template <class T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw exception("const T& Back(): SList is empty!");
		}

		return mBack->mData;
	}

	template <class T>
	inline uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	inline void SList<T>::Clear()
	{
		Node *tempNode = nullptr;

		while (mFront != nullptr)								//Traverse through the entire list and delete all the allocated memory.
		{
			tempNode = mFront;
			mFront = mFront->mNext;
			delete tempNode;
			tempNode = nullptr;
		}

		mBack = nullptr;
		mSize = 0;
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(this, mFront);
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(this, nullptr);
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const T& data, const Iterator& iterator)
	{
		//Checks if the iterator belongs to "this" SList and it is a valid iterator(i.e Iterator is not pointing to the end of list, Iterator is initialized, Iterator is pointing to a valid data)
		if (iterator.mOwner == this && iterator.mCurrentNode != nullptr)
		{
			Node* newNode = new Node(data, iterator.mCurrentNode->mNext);
			iterator.mCurrentNode->mNext = newNode;

			//If the Node is inserted at the end of SList
			if (newNode->mNext == nullptr)
			{
				mBack = newNode;
			}

			return Iterator(this, newNode);
		}

		return end();
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value) const
	{
		Iterator endIterator = end();
		for (Iterator iterator = begin(); iterator != endIterator; ++iterator)
		{
			if ((*iterator) == value)
			{
				return iterator;
			}
		}

		return endIterator;
	}

	template<class T>
	inline bool SList<T>::Remove(const T& value)
	{
		bool isSuccessfullyRemoved = false;

		if (IsEmpty())
		{
			isSuccessfullyRemoved = false;
		}
		else
		{
			//If the list is not empty
			Node *currentNode = mFront;

			//Checking if the value is present in the first node
			if (currentNode->mData == value)
			{
				mFront = mFront->mNext;
				mBack = nullptr;

				delete currentNode;
				currentNode = nullptr;

				//Decrement the size of SList
				mSize--;

				isSuccessfullyRemoved = true;
			}
			else
			{
				//Checking if the value is present in the subsequent nodes
				Node *previousNode = currentNode;
				currentNode = currentNode->mNext;

				while (currentNode != nullptr)
				{
					if (currentNode->mData == value)
					{
						previousNode->mNext = currentNode->mNext;

						delete currentNode;
						currentNode = nullptr;

						//Decrement the size of SList
						mSize--;

						isSuccessfullyRemoved = true;						

						break;
					}

					previousNode = currentNode;
					currentNode = currentNode->mNext;
				}
			}
		}

		return isSuccessfullyRemoved;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/


	/************************************************************************/
	/* BEGIN	-Node struct-			Constructor.						*/
	/************************************************************************/
	template<class T>
	inline SList<T>::Node::Node(const T& data, Node* next)
	{
		mData = data;
		mNext = next;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/


	/************************************************************************/
	/* BEGIN	-Iterator class-		Constructors & Functions.			*/
	/************************************************************************/
	template<class T>
	inline SList<T>::Iterator::Iterator() :
		mCurrentNode(nullptr), mOwner(nullptr)
	{

	}

	template<class T>
	inline SList<T>::Iterator::Iterator(const SList* owner, Node* currentNode) :
		mOwner(owner), mCurrentNode(currentNode)
	{

	}

	template<class T>
	inline bool SList<T>::Iterator::operator==(const Iterator& rhs) const
	{
		bool areEqual = false;

		//Checking if both the Iterators belong to the same SList
		if (mOwner == rhs.mOwner)
		{
			//Checking if both the Iterators point to the same Node
			if (mCurrentNode == rhs.mCurrentNode)
			{
				areEqual = true;
			}
		}

		return areEqual;
	}

	template<class T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(*this == rhs);
	}

	template<class T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("Iterator& SList<T>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		mCurrentNode = mCurrentNode->mNext;
		return (*this);
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		//Make a copy of the current Iterator
		Iterator temp(*this);

		//Increment the current Iterator
		++(*this);

		//Return the copy as VALUE and NOT by REFERENCE.
		return temp;
	}

	template<class T>
	inline T& SList<T>::Iterator::operator*()
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("T& SList<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}
		return mCurrentNode->mData;
	}

	template<class T>
	inline const T& SList<T>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr)
		{
			throw exception("T& SList<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}
		return mCurrentNode->mData;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
}
