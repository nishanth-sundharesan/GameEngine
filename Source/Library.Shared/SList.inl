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
			PushBack(tempFrontNode->data);
			tempFrontNode = tempFrontNode->next;
		}
	}

	template <class T>
	inline SList<T>& SList<T>::operator=(const SList<T>& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			Node* tempFrontNode = rhs.mFront;
			while (tempFrontNode != nullptr)
			{
				PushBack(tempFrontNode->data);
				tempFrontNode = tempFrontNode->next;
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
	inline void SList<T>::PushFront(const T& pData)
	{
		Node* newNode = new Node();
		newNode->data = pData;
		newNode->next = mFront;

		if (IsEmpty())
		{
			mFront = newNode;
			mBack = newNode;
		}
		else
		{
			mFront = newNode;
		}

		++mSize;
	}

	template <class T>
	inline void SList<T>::PushBack(const T& pData)
	{
		Node* newNode = new Node();
		newNode->data = pData;
		newNode->next = nullptr;

		if (IsEmpty())
		{
			mFront = newNode;
			mBack = newNode;
		}
		else
		{
			mBack->next = newNode;
			mBack = mBack->next;
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
		mFront = mFront->next;

		delete tempNode;

		--mSize;
	}

	template <class T>
	inline void SList<T>::PopFront(T& pData)
	{
		if (IsEmpty())
		{
			throw exception("void PopFront(T& ): SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->next;

		pData = tempNode->data;

		delete tempNode;

		--mSize;
	}

	template <class T>
	inline bool SList<T>::IsEmpty()
	{
		return mSize == 0;
	}

	template <class T>
	inline const bool SList<T>::IsEmpty() const
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

		return mFront->data;
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

		return mBack->data;
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
	inline int SList<T>::Size()
	{
		return mSize;
	}

	template <class T>
	inline const int SList<T>::Size() const
	{
		return mSize;
	}

	template <class T>
	inline void SList<T>::Clear()
	{
		Node *tempNode = nullptr;

		while (mFront != nullptr)
		{
			tempNode = mFront;
			mFront = mFront->next;
			delete tempNode;
		}

		mBack = nullptr;
		mSize = 0;
	}
}
