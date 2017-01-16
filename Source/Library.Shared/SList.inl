#include "Pch.h"

using namespace std;

namespace GameEngineLibrary
{
	template <class T>
	SList<T>::SList() :
		mBack(nullptr), mFront(nullptr), mSize(0)
	{

	}

	template <class T>
	SList<T>::SList(const SList<T>& other) :
		mBack(nullptr), mFront(nullptr), mSize(0)
	{
		if (!IsEmpty())
		{
			Node* tempFrontNode = other.mFront;
			while (tempFrontNode != nullptr)
			{
				PushBack(*tempFrontNode->data);				
				tempFrontNode = tempFrontNode->next;
			}
		}
	}

	template <class T>
	inline SList<T>& SList<T>::operator=(const SList<T>& other)
	{
		Clear();
		this(other);		
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
		newNode->data = new T;
		*newNode->data = pData;
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
	inline T SList<T>::PopFront()
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->next;

		T tempData = &tempNode->data;

		delete tempNode->data;
		delete tempNode;

		--mSize;

		return tempData;
	}

	template <class T>
	inline void SList<T>::PopFront(T& pData)
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
		}

		Node* tempNode = mFront;
		mFront = mFront->next;

		pData = &tempNode->data;

		delete tempNode->data;
		delete tempNode;

		--mSize;
	}

	template <class T>
	inline void SList<T>::PushBack(const T& pData)
	{
		Node* newNode = new Node();
		newNode->data = new T;
		*newNode->data = pData;
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
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template <class T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
		}

		return mFront->data;
	}

	template <class T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
		}

		return mBack->data;
	}

	template <class T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
		}

		return mFront->data;
	}

	template <class T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw exception("SList is empty!");
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
			delete tempNode->data;
			delete tempNode;
		}

		mBack = nullptr;
	}
}
