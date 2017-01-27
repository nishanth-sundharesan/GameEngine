#pragma once
#include <cstdint>
#define	VectorInitialized

namespace GameEngineLibrary
{
	/** Templated Vector class
	*/
	template<class T>
	class Vector
	{
	public:
		class Iterator
		{
		public:
			Iterator();

			Iterator(const Iterator& rhs) = default;

			Iterator& operator=(const Iterator& rhs) = default;

			bool operator==(const Iterator& rhs) const;

			bool operator!=(const Iterator& rhs) const;

			Iterator& operator++();

			Iterator operator++(int);

			T& operator*();

			const T& operator*() const;
		private:
			Iterator(const Vector* owner, T* currentNode);

			const Vector* mOwner;
			T* mCurrentNode;
		};

		Vector();

		Vector(std::uint32_t capacity);

		Vector(const Vector<T>& rhs);

		~Vector();

		bool IsEmpty() const;

		std::uint32_t Size() const;

		std::uint32_t Capacity() const;

		Vector<T>& operator=(const Vector<T>& rhs);

		T& operator[](const std::uint32_t index);

		const T& operator[](const std::uint32_t index) const;

		T& At(const std::uint32_t index);

		const T& At(const std::uint32_t index) const;

		T& Front();

		const T& Front() const;

		T& Back();

		Iterator Find(const T& value) const;

		bool Remove(const T& value);

		bool Remove(const Iterator& start, const Iterator& end);

		const T& Back() const;

		Iterator PushBack(const T& value);

		bool PopBack();

		bool PopBack(const T& value);

		Iterator begin() const;

		Iterator end() const;

		void Reserve(std::uint32_t capacity);

		void Clear();

		void ShrinkToFit();
	private:
		T* mStartingAddress;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
	};

}

#include "Vector.inl"
