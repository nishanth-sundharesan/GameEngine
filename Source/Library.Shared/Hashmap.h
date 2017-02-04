#pragma once
#include <cstdint>
#include "Vector.h"
#include "SList.h"
#define	HashmapInitialized

namespace GameEngineLibrary
{
	template<class TKey>
	class DefaultHashFunctor
	{
	public:
		DefaultHashFunctor() = default;
		std::uint32_t DefaultHashFunctor::operator()(const TKey& key) const
		{
			uint32_t hash = 0;
			char* byteArray = reinterpret_cast<char*>(const_cast<TKey*>(&key));
			for (uint32_t i = 0; i < sizeof(key); ++i)
			{
				hash += byteArray[i];
			}
			return hash;
		}
	};

	template<>
	class DefaultHashFunctor<char*>
	{
	public:
		DefaultHashFunctor() = default;
		std::uint32_t DefaultHashFunctor::operator()(const char* key) const
		{
			uint32_t hash = 0;

			while (*key != '\0')
			{
				hash += static_cast<uint8_t>(*key);
				++key;
			}
			return hash;
		}
	};

	template<>
	class DefaultHashFunctor<std::string>
	{
	public:
		DefaultHashFunctor() = default;
		std::uint32_t DefaultHashFunctor::operator()(const std::string key) const
		{
			uint32_t hash = 0;

			for (uint32_t i = 0; i < key.length(); ++i)
			{
				hash += static_cast<uint8_t>(key[i]);
			}
			return hash;
		}
	};

	template<class TKey>
	class DefaultHashFunctor<TKey*>
	{
	public:
		DefaultHashFunctor() = default;
		inline std::uint32_t DefaultHashFunctor::operator()(const TKey* key) const
		{
			uint32_t hash = 0;
			/*uint8_t* byteArray = reinterpret_cast<uint8_t*>(const_cast<TKey*>(&key));
			for (uint32_t i = 0; i < sizeof(key); ++i)
			{
				hash += byteArray[i];
			}*/
			return hash;
		}
	};
}

namespace GameEngineLibrary
{
	/** Templated Hashmap class which takes in a Hash functor.
	*/
	template <class TKey, class TValue, typename HashFunctor = DefaultHashFunctor<TKey>>
	class Hashmap
	{
		typedef std::pair<TKey, TValue> PairType;
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;
	public:
		/** An Iterator class for the Hashmap class.
		*/
		class Iterator final
		{
			/** Hashmap class is marked as a friend class so that the Hashmap class can access the private members of the Iterator class.
			*/
			friend class Hashmap;
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
			*	@returns Returns true when both the Iterators point to the same data, false otherwise.
			*	@param rhs The right hand side Iterator to be compared with.
			*/
			bool operator==(const Iterator& rhs) const;

			/** Overloaded inequality operator.
			*	Checks if both the Iterators are not equal or both the Iterators do not point to the same data.
			*	Note: The function will return false when two uninitialized Iterators are compared.
			*	@returns Returns true when both the Iterators do not point to the same data, false otherwise.
			*	@param rhs The right hand side Iterator to be compared with.
			*/
			bool operator!=(const Iterator& rhs) const;

			/** Overloaded ++ - Prefix Increment operator.
			*	Increments the iterator and points to the next consequent data.
			*	@returns Returns the current Iterator reference after incrementing.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data.
			*/
			Iterator& operator++();

			/** Overloaded ++ - Postfix Increment operator.
			*	Makes a copy of the called iterator and returns it. It then later increments the called iterator and points to the next consequent data.
			*	For gaining performance, use the ++ - Prefix Increment operator
			*	@returns Returns a copy of the called iterator. It then later increments the called iterator.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data.
			*/
			Iterator operator++(int);

			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data
			*/
			PairType& operator*();

			/** Overloaded * (content of) operator.
			*	Returns the content of which the iterator was pointing to.
			*	@returns Returns the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data
			*/
			const PairType& operator*() const;

			/** Overloaded -> arrow operator.
			*	Returns the pointer to the content of which the iterator was pointing to.
			*	@returns Returns the pointer to the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data
			*/
			PairType* operator->();

			/** Overloaded -> arrow operator.
			*	Returns the pointer to the content of which the iterator was pointing to.
			*	@returns Returns the pointer to the content of which the iterator was pointing to.
			*	@throws Throws an exception if the Iterator is uninitialized or is pointing to the end of the Hashmap or is pointing to an invalid data
			*/
			const PairType* operator->() const;
		private:
			/** A private constructor which is called only from within the Hashmap class.
			*/
			Iterator(const Hashmap* owner, std::uint32_t index, typename ChainType::Iterator iterator);

			/** Index to the bucket of the Hashmap.
			*/
			std::uint32_t mIndex;

			/** The iterator to iterate the chain of an individual bucket.
			*/
			typename ChainType::Iterator mIterator;

			/** The Hashmap to which the Iterator belongs to.
			*/
			const Hashmap* mOwner;
		};

		/** Parameterized constructor.
		*	@param size The size for the Hashmap.
		*/
		explicit Hashmap(std::uint32_t size = 20);

		/** Use the default copy constructor to perform member wise copy.
		*/
		Hashmap(const Hashmap&) = default;

		/** Use the default assignment operator to perform member wise copy.
		*/
		Hashmap& operator=(const Hashmap&) = default;

		/** Clears the entire Hashmap.		
		*/
		virtual ~Hashmap() = default;

		/** Inserts the pair in the Hashmap based on the key of the pair.
		*	@param pair The pair to be inserted in the Hashmap.
		*	@returns Returns the Iterator pointing to the the newly inserted pair.
		*/
		Iterator Insert(const PairType& pair);

		/** Finds the iterator for the specified key in the Hashmap.
		*	@param key The key to the pair for which the Iterator should be found.
		*	@throws Throws an exception when specified key is not present in the Hashmap.
		*	@returns Returns an iterator for the matched key. If no matches were found then the function throws an exception.
		*/
		Iterator Find(const TKey& key) const;

		/** Overloaded subscript operator.
		*	This function returns the value for the specified key.
		*	Note: If the value for the specified key was not found, then a default copy of the TValue is created and inserted into the Hashmap. The same is returned.
		*	@param key The key to the pair for which the TValue should be returned.		
		*	@returns Returns the value for the specified key. If the value for the specified key was not found, then a default copy of the TValue is created and inserted into the Hashmap. The same is returned.
		*/
		TValue& operator[](const TKey& key);

		/** Overloaded subscript operator.
		*	This function returns the value for the specified key.
		*	Note: If the value for the specified key was not found, then a default copy of the TValue is created and inserted into the Hashmap. The same is returned.
		*	@param key The key to the pair for which the TValue should be returned.
		*	@returns Returns the value for the specified key. If the value for the specified key was not found, then a default copy of the TValue is created and inserted into the Hashmap. The same is returned.
		*/
		const TValue& operator[](const TKey& key) const;

		/** Removes the pair for the specified key in the Hashmap.
		*	@param key The key to the pair which should be removed in the Hashmap.
		*	@returns Returns true if the pair was successfully removed, false otherwise.
		*/
		bool Remove(const TKey& key);

		/** Returns the size of the Hashmap.
		*	@return Returns the size of the Hashmap.
		*/
		std::uint32_t Size() const;

		/** Checks if the specified key is present in the Hashmap.
		*	@param key The key to be searched for in the Hashmap.
		*	@returns Returns true if the key was successfully found, false otherwise.
		*/
		bool ContainsKey(const TKey& key) const;

		/** Returns an iterator that points to the beginning of the Hashmap.
		*	@returns Returns an iterator that points to the beginning of the Hashmap.
		*/
		Iterator begin() const;

		/** Returns an iterator that points one past the end of Hashmap.
		*	@returns Returns an iterator that points one past the end of Hashmap.
		*/
		Iterator end() const;

		/** Clears the entire Hashmap.
		*	Note: This function doesn't reset the Bucket size of the Hashmap.
		*/
		void Clear();
	private:		

		Iterator SearchBasedOnIndex(const uint32_t index, const TKey& key) const;
		
		/** The total population of the data/pairs present in the Hashmap.
		*	Note: This variable doesn't indicate the Bucket size of the Hashmap.
		*/
		std::uint32_t mSize;

		/** The Buckets of the Hashmap which is a Vector array of Chains(which are SLists).
		*/
		BucketType mBuckets;

		/** The Hash functor of the Hashmap.
		*/
		HashFunctor hashFunctor;
	};
}

#include "Hashmap.inl"