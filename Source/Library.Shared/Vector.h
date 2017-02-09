#pragma once
#include <cstdint>
#define	VectorInitialized
#include "DefaultHashFunctor.h"

namespace GameEngineLibrary
{
	class DefaultReserveStrategy
	{
	public:
		std::uint32_t DefaultReserveStrategy::operator()(std::uint32_t size, uint32_t capacity) const
		{
			size;
			return capacity * 2;
		}
	};
}

namespace GameEngineLibrary
{
	/** Templated Vector class
	*/
	template<class T, typename F = DefaultReserveStrategy>
	class Vector
	{
	public:
		/** An Iterator class for the Vector class.
		*/
		class Iterator final
		{
			/** Vector class is marked as a friend class so that the Vector class can access the private members of the Iterator class.
			*/
			friend class Vector;

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

			/** Use the default destructor
			*/
			~Iterator() = default;

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
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the vector array or is pointing to an invalid data.
			*/
			Iterator& operator++();

			/** Overloaded ++ - Postfix Increment operator.
			*	Makes a copy of the called iterator and returns it. It then later increments the called iterator and points to the next consequent data.
			*	For gaining performance, use the ++ - Prefix Increment operator
			*	@returns Returns a copy of the called iterator. It then later increments the called iterator.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the vector array or is pointing to an invalid data.
			*/
			Iterator operator++(int);

			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the vector array or is pointing to an invalid data
			*/
			T& operator*();

			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the vector array or is pointing to an invalid data
			*/
			const T& operator*() const;
		private:
			/** A private constructor which is called only from within the Vector class.
			*/
			Iterator(const Vector* owner, std::uint32_t mIndex);

			/** The Vector to which the Iterator belongs to.
			*/
			const Vector* mOwner;

			/** The index of the node in the Vector.
			*/
			std::uint32_t mIndex;
		};
		/** Parameterized constructor.
		*	Initializes the private members of the class Vector and Reserves the Vector with the specified capacity(memory).
		*	@param capacity The expected size of the Vector array.
		*/
		explicit Vector(std::uint32_t capacity = 29);

		/** Copy constructor.
		*	Performs a deep copy of the Vector array.
		*	@param rhs The object of which the deep copy will be made.
		*/
		Vector(const Vector& rhs);

		/** Overloaded assignment operator.
		*	Performs a deep copy of the right hand side object: Vector array.
		*	@param rhs It is the right hand side object which will be deep copied to the left hand side object.
		*	@return Returns the deep copied Vector array
		*/
		Vector& operator=(const Vector& rhs);

		/** Clears the entire Vector.
		*	Note: This function doesn't reset the capacity of the Vector array. Use ShrinkToFit() or Reserve().
		*/
		virtual ~Vector();

		/** Pushes/Adds the data at the back of the Vector array.
		*	@param pData The data to be pushed at the back of the array.
		*	@returns Returns the Iterator pointing to the the newly pushed data
		*/
		Iterator PushBack(const T& value);

		/** Pops/Deletes the data at the back of the Vector array.
		*	@returns Returns true if the data was successfully removed, false otherwise.
		*	@see PopBack(T& value);
		*/
		bool PopBack();

		/** Assigns the data at the back of the array to the param "value" and deletes the data at the back of the array.
		*	@param value The popped data from the back of the array.
		*	@returns Returns true if the data was successfully removed, false otherwise.
		*	@see PopFront();
		*/
		bool PopBack(T& value);

		/** Checks if Vector array is empty or not.
		*	Note. This doesn't check the capacity of the Vector array.
		*	@return Returns true if array is empty, false otherwise.
		*/
		bool IsEmpty() const;

		/** Returns the data at the front of the Vector array.
		*	@return Returns the data at the front of the Vector array.
		*	@throws Throws an exception if the Vector is empty.
		*/
		T& Front();

		/** Returns the data at the front of the Vector array.
		*	@return Returns the data at the front of the Vector array.
		*	@throws Throws an exception if the Vector is empty.
		*/
		const T& Front() const;

		/** Returns the data at the back of the Vector array.
		*	@return Returns the data at the back of the Vector array.
		*	@throws Throws an exception if the Vector is empty.
		*/
		T& Back();

		/** Returns the data at the back of the Vector array.
		*	@return Returns the data at the back of the Vector array.
		*	@throws Throws an exception if the Vector is empty.
		*/
		const T& Back() const;

		/** Returns the size of the Vector array.
		*	@return Returns the size of the Vector array.
		*/
		std::uint32_t Size() const;

		/** Returns the capacity of the Vector array.
		*	@return Returns the capacity of the Vector array.
		*/
		std::uint32_t Capacity() const;

		/** Clears the entire Vector array.
		*	Note. This function doesn't reset the capacity of the Vector array.
		*	@see ShrinkToFit();
		*/
		void Clear();

		/** Returns an iterator that points to the beginning of the Vector array.
		*	@returns Returns an iterator that points to the beginning of the Vector array.
		*/
		Iterator begin() const;

		/** Returns an iterator that points one past the end of Vector array.
		*	@returns Returns an iterator that points one past the end of Vector array.
		*/
		Iterator end() const;

		/** Finds the iterator for the first matched specified value.
		*	@param value The value for which the Iterator should be found.
		*	@returns Returns an iterator for the first matched value. If no matches were found then the function returns an iterator that points past the end of the Vector array.
		*/
		Iterator Find(const T& value) const;

		/** Removes the first matched value in the Vector array.
		*	@param value The data to be removed in the Vector array.
		*	@returns Returns true if the value was successfully removed, false otherwise.
		*/
		bool Remove(const T& value);

		/** Removes all the elements between the startIterator and endIterator.
		*	@param startIterator The starting Iterator(inclusive).
		*	@param endIterator The ending Iterator(exclusive).
		*	@throws Throws an exception if the Iterators belong to a different Vector or if the start Iterator is greater than or equal to the end Iterator.
		*	@returns Returns true if the values were successfully removed, false otherwise.
		*/
		bool Remove(const Iterator& startIterator, const Iterator& endIterator);

		/** Overloaded subscript operator.
		*	This function returns the value present at the specified index.
		*	@param index The index of the value in the Vector array to be returned.
		*	@throws Throws an exception if the index is greater than or equal to the size of the Vector array.
		*	@returns Returns the value present at the specified index.
		*/
		T& operator[](const std::uint32_t index);

		/** Overloaded subscript operator.
		*	This function returns the value present at the specified index.
		*	@param index The index of the value in the Vector array to be returned.
		*	@throws Throws an exception if the index is greater than or equal to the size of the Vector array.
		*	@returns Returns the value present at the specified index.
		*/
		const T& operator[](const std::uint32_t index) const;

		/** Returns the value present at the specified index.
		*	@param index The index of the value in the Vector array to be returned.
		*	@throws Throws an exception if the index is greater than or equal to the size of the Vector array.
		*	@returns Returns the value present at the specified index.
		*/
		T& At(const std::uint32_t index);

		/** Returns the value present at the specified index.
		*	@param index The index of the value in the Vector array to be returned.
		*	@throws Throws an exception if the index is greater than or equal to the size of the Vector array.
		*	@returns Returns the value present at the specified index.
		*/
		const T& At(const std::uint32_t index) const;

		/** Reserves/Allocates the memory for the Vector array.
		*	Reserves the memory for the param "capacity" number of objects in the Vector array.
		*	Reserves/Allocates the memory only if the specified capacity is greater than the current capacity.
		*	@param capacity The new capacity of the Vector array. This should be greater than the current capacity
		*	@throws Throws an exception if the function fails to allocate memory.
		*/
		void Reserve(std::uint32_t capacity);

		/** Shrinks the capacity of the Vector array to the Size of the Vector array.
		*	Note: This function doesn't affect the size of the Vector array.
		*/
		void ShrinkToFit();
	private:
		/** The starting address of the allocated memory chunk.
		*/
		T* mStartingAddress;

		/** The size of the Vector array.
		*/
		std::uint32_t mSize;

		/** The capacity of the Vector array.
		*/
		std::uint32_t mCapacity;
	};
}

#include "Vector.inl"
