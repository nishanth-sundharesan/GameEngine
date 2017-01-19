#pragma once
#include <cstdint>
#define SListInitialized

namespace GameEngineLibrary
{
	/** Templated Singly Linked List Class.
	*/
	template <class T>
	class SList
	{
	public:
		/** Zero parameter constructor.
		*	Initializes the private members of the class.
		*/
		SList();

		/** Copy constructor.
		*	Performs a deep copy of an SList object.
		*	@param rhs The object of which the deep copy will be made.
		*/
		SList(const SList<T>& rhs);

		/** Overloaded assignment operator.
		*	Performs a deep copy of the right hand side object.
		*	@param rhs It is the right hand side object which will be deep copied to the left hand side object.
		*	@return Returns the deep copied SList
		*/
		SList<T>& operator=(const SList<T>& rhs);

		/** Clears the entire SList.
		*/
		~SList();

		/** Pushes/Adds the data at the front of the list.
		*	@param pData The data to be pushed at the front of the list.
		*/
		void PushFront(const T& pData);

		/** Pushes/Adds the data at the back of the list.
		*	@param pData The data to be pushed at the back of the list.
		*/
		void PushBack(const T& pData);

		/** Pops/Deletes the data at the front of the list.
		*	@throws Throws an exception if SList is empty.
		*	@see PopFront(T& pData);
		*/
		void PopFront();

		/** Assigns the data at the front of the list to the pData and deletes the data at the front of the list.
		*	@param pData The popped data from the front of the list.
		*	@throws Throws an exception if SList is empty.
		*	@see PopFront();
		*/
		void PopFront(T& pData);

		/** Checks if SList is empty or not.
		*	@return Returns true if SList is empty, false otherwise.
		*/
		bool IsEmpty();

		/** Checks if SList is empty or not.
		*	@return Returns true if SList is empty, false otherwise.
		*/
		const bool IsEmpty() const;

		/** Returns the data at the front of SList.
		*	@return Returns the data at the front of SList.
		*	@throws Throws an exception if SList is empty.
		*/
		T& Front();

		/** Returns the data at the front of SList.
		*	@return Returns the data at the front of SList.
		*	@throws Throws an exception if SList is empty.
		*/
		const T& Front() const;

		/** Returns the data at the back of SList.
		*	@return Returns the data at the back of SList.
		*	@throws Throws an exception if SList is empty.
		*/
		T& Back();

		/** Returns the data at the back of SList.
		*	@return Returns the data at the back of SList.
		*	@throws Throws an exception if SList is empty.
		*/
		const T& Back() const;

		/** Returns the size of SList.
		*	@return Returns the size of SList.
		*/
		int Size();

		/** Returns the size of SList.
		*	@return Returns the size of SList.
		*/
		const int Size() const;

		/** Clears the entire SList.
		*/
		void Clear();

	private:
		struct Node
		{
			T data;
			Node* next;
		};

		/** Node pointer to the back of the list.
		*/
		Node* mBack;

		/** Node pointer to the back of the list.
		*/
		Node* mFront;

		/** An integer variable to hold the size of the list.
		*/
		std::int32_t mSize;
	};
}

#include "SList.inl"