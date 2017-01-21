#include "Pch.h"
#ifndef SListInitialized
#include "SList.h"
#endif

using namespace std;

namespace GameEngineLibrary
{
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

		return mFront->data;
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

		return mBack->data;
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
		}

		mBack = nullptr;
		mSize = 0;
	}

	template<class T>
	inline SList<T>::Node::Node(const T& data, Node * next)
	{
		mData = data;
		mNext = next;
	}
}
