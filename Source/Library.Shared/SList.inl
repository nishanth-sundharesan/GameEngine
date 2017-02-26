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
		operator=(rhs);
	}

	template <class T>
	inline SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();													//Clearing the entire list before assigning new data
			for (const auto& value : rhs)
			{
				PushBack(value);
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
	inline typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		mFront = new Node(value, mFront);
		if (IsEmpty())
		{
			mBack = mFront;
		}
		++mSize;
		return Iterator(this, mFront);
	}

	template <class T>
	inline typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{
		Node* newNode = new Node(value, nullptr);

		if (IsEmpty())
		{
			mFront = newNode;
		}
		else
		{
			mBack->mNext = newNode;
		}

		mBack = newNode;
		++mSize;
		return Iterator(this, newNode);
	}

	template <class T>
	inline void SList<T>::PopFront()
	{
		if (IsEmpty())
		{
			throw out_of_range("void PopFront(): SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->mNext;

		delete tempNode;
		tempNode = nullptr;

		--mSize;
	}

	template <class T>
	inline void SList<T>::PopFront(T& value)
	{
		if (IsEmpty())
		{
			throw out_of_range("void PopFront(T& ): SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->mNext;

		value = tempNode->mData;

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
			throw out_of_range("T& SList<T>::Front(): SList is empty!");
		}

		return mFront->mData;
	}

	template <class T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw out_of_range("const T& SList<T>::Front() const: SList is empty!");
		}

		return mFront->mData;
	}

	template <class T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw out_of_range("T& Back(): SList is empty!");
		}

		return mBack->mData;
	}

	template <class T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw out_of_range("const T& Back(): SList is empty!");
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
	inline typename SList<T>::Iterator SList<T>::InsertAfter(const T& value, const Iterator& iterator)
	{
		if (iterator.mOwner != this)
		{
			throw exception("SList<T>::Iterator SList<T>::InsertAfter(): Iterator is uninitialized ot it belongs to a different SList!");
		}
		if (iterator == end())
		{
			return PushBack(value);
		}

		//If the iterator belongs to "this" SList and it is already pointing to a data)
		Node* newNode = new Node(value, iterator.mCurrentNode->mNext);
		iterator.mCurrentNode->mNext = newNode;

		//If the Node is inserted at the end of SList
		if (newNode->mNext == nullptr)
		{
			mBack = newNode;
		}

		return Iterator(this, newNode);
	}

	template<class T>
	inline typename SList<T>::Iterator SList<T>::Find(const T& value) const
	{
		if (IsEmpty())
		{
			return end();
		}

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

	template<class T>
	inline bool SList<T>::Remove(const Iterator& iterator)
	{
		bool isSuccessfullyRemoved = false;
		if (iterator.mCurrentNode == nullptr || iterator.mOwner == nullptr)
		{
			return isSuccessfullyRemoved;
		}
		if (iterator == end())
		{
			return isSuccessfullyRemoved;
		}

		if (iterator == begin())
		{
			PopFront();
			isSuccessfullyRemoved = true;
		}
		else
		{
			Iterator prevIterator = begin();
			while (prevIterator.mCurrentNode->mNext != iterator.mCurrentNode)
			{
				++prevIterator;
			}

			prevIterator.mCurrentNode->mNext = iterator.mCurrentNode->mNext;
			if (mBack == iterator.mCurrentNode)
			{
				mBack = prevIterator.mCurrentNode;
			}

			delete iterator.mCurrentNode;
			--mSize;

			isSuccessfullyRemoved = true;
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
	inline SList<T>::Node::Node(const T& value, Node* next)
	{
		mData = value;
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
		return ((mOwner == rhs.mOwner) && (mCurrentNode == rhs.mCurrentNode));
	}

	template<class T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template<class T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mCurrentNode == nullptr)
		{
			throw out_of_range("SList<T>::Iterator& SList<T>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
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
		operator++();

		//Return the copy as a VALUE and NOT by REFERENCE.
		return temp;
	}

	template<class T>
	inline T& SList<T>::Iterator::operator*()
	{
		if (mCurrentNode == nullptr)
		{
			throw out_of_range("T& SList<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}
		return mCurrentNode->mData;
	}

	template<class T>
	inline const T& SList<T>::Iterator::operator*() const
	{
		if (mCurrentNode == nullptr)
		{
			throw out_of_range("T& SList<T>::Iterator::operator*(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}
		return mCurrentNode->mData;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
}
