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
	private:
		struct Node
		{
			Node(const T& data, Node* next = nullptr);

			T mData;
			Node* mNext;
		};

	public:
		/** An Iterator class for the SList class.
		*/
		class Iterator
		{
			/** SList is marked as a friend class so that the SList class can access the private members of the Iterator class.
			*/
			friend class SList;
		public:
			/** Zero parameterized constructor.
			*	Initializes the private members of the class.
			*/
			Iterator();

			/** Use the default copy constructor to perform member wise copy.			
			*/
			Iterator(const Iterator&) = default;

			/** Use the default assignment operator to perform member wise copy.
			*/
			Iterator& operator=(const Iterator&) = default;

			/** Overloaded equality operator.
			*	Checks if both the Iterators point to the same data.
			*	Note: The function will return true when two uninitialized Iterators are compared.
			*	@returns Returns true when both the Iterators point to the same node, false otherwise.
			*	@param rhs The right hand side Iterator to be compared with.
			*/
			bool operator==(const Iterator& rhs) const;

			/** Overloaded inequality operator.
			*	Checks if both the Iterators are not equal or both the Iterators do not point to the same data.
			*	Note: The function will return false when two uninitialized Iterators are compared.
			*	@returns Returns true when both the Iterators do not point to the same node, false otherwise.
			*	@param rhs The right hand side Iterator to be compared with.
			*/
			bool operator!=(const Iterator& rhs) const;

			/** Overloaded ++ - Prefix Increment operator.
			*	Increments the iterator and points to the next consequent data.
			*	@returns Returns the current Iterator reference after incrementing.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data.
			*/
			Iterator& operator++();

			/** Overloaded ++ - Postfix Increment operator.
			*	Makes a copy of the called iterator and returns it. It then later increments the called iterator and points to the next consequent data.
			*	For gaining performance, use the ++ - Prefix Increment operator
			*	@returns Returns a copy of the called iterator. It then later increments the called iterator.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data.
			*/
			Iterator operator++(int);
			
			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data
			*/
			T& operator*();

			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data
			*/
			const T& operator*() const;

		private:
			/** A private constructor which is called only by the SList.
			*/
			Iterator(const SList<T>* owner, Node* currentNode);

			/** The SList of which the Iterator belongs to.
			*/
			const SList* mOwner;

			/** To node to which the Iterator is currently pointing to.
			*/
			Node* mCurrentNode;
		};

		/** Zero parameterized constructor.
		*	Initializes the private members of the class SList.
		*/
		SList();

		/** Copy constructor.
		*	Performs a deep copy of an SList object.
		*	@param rhs The object of which the deep copy will be made.
		*/
		SList(const SList<T>& rhs);

		/** Overloaded assignment operator.
		*	Performs a deep copy of the right hand side object: SList.
		*	@param rhs It is the right hand side object which will be deep copied to the left hand side object.
		*	@return Returns the deep copied SList
		*/
		SList<T>& operator=(const SList<T>& rhs);

		/** Clears the entire SList.
		*/
		~SList();

		/** Pushes/Adds the data at the front of the list.
		*	@param pData The data to be pushed at the front of the list.
		*	@returns Returns the Iterator of the newly pushed data
		*/
		Iterator PushFront(const T& value);

		/** Pushes/Adds the data at the back of the list.
		*	@param pData The data to be pushed at the back of the list.
		*	@returns Returns the Iterator pointing to the newly pushed data
		*/
		Iterator PushBack(const T& value);

		/** Pops/Deletes the data at the front of the list.
		*	@throws Throws an exception if SList is empty.
		*	@see PopFront(T& pData);
		*/
		void PopFront();

		/** Assigns the data at the front of the list to the param value and deletes the data at the front of the list.
		*	@param value The popped data from the front of the list.
		*	@throws Throws an exception if SList is empty.
		*	@see PopFront();
		*/
		void PopFront(T& value);

		/** Checks if SList is empty or not.
		*	@return Returns true if SList is empty, false otherwise.
		*/
		bool IsEmpty() const;

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
		std::uint32_t Size() const;

		/** Clears the entire SList.
		*/
		void Clear();

		/** Returns an iterator that points to the beginning of SList.
		*	@returns Returns an iterator that points to the beginning of SList.
		*/
		Iterator begin() const;

		/** Returns an iterator that points one past the end of SList.
		*	@returns Returns an iterator that points one past the end of SList.
		*/
		Iterator end() const;

		/** This function inserts an item after the specified iterator.
		*	If the iterator points one past the end of the list, then it inserts the data at the end of the list.
		*	@param data The data to be inserted in the SList.
		*	@param iterator	The Iterator after which the data will be inserted.
		*	@returns Returns an Iterator that points to the newly inserted data.
		*/
		Iterator InsertAfter(const T& value, const Iterator& iterator);

		/** Finds the iterator for the first matched specified value.
		*	@param value The value for which the Iterator should be found.
		*	@returns Returns an iterator for the first matched value. If no matches were found then the function returns an iterator that points past the end of SList.
		*/
		Iterator Find(const T& value) const;

		/** Removes the first matched value in SList.
		*	@param value The value to be removed in SList.
		*	@returns Returns true if the data was successfully removed, false otherwise.
		*/
		bool Remove(const T& value);

		bool Remove(const Iterator& iterator);

	private:
		/** Node pointer pointing to the back of SList.
		*/
		Node* mBack;

		/** Node pointer pointing to the front of SList.
		*/
		Node* mFront;

		/** An integer variable to hold the size of SList.
		*/
		std::uint32_t mSize;
	};
}

#include "SList.inl"