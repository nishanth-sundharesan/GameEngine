#pragma once
#include <cstdint>

namespace GameEngineLibrary
{	
	template <class T>
	class SList
	{
	public:
		SList();
		SList(const SList<T>& other);
		~SList();
		SList<T>& operator=(const SList<T>& other);		

		void PushFront(const T& pData);
		T PopFront();
		void PopFront(T& pData);
		void PushBack(const T& pData);
		bool IsEmpty() const;

		T& Front();
		T& Back();
		const T& Front() const;
		const T& Back() const;

		int Size();
		const int Size() const;
		void Clear();

	private:
		struct Node
		{
			T *data;
			Node* next;
		};

		Node* mBack;
		Node* mFront;

		std::int32_t mSize;
	};
}

#include "SList.inl"