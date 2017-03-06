#include "Pch.h"
#ifndef HashmapInitialized
#include "Hashmap.h"
#endif

using namespace std;

namespace GameEngineLibrary
{
#pragma region Hashmap Implementation
	/************************************************************************/
	/* BEGIN	-Hashmap class-		Constructors & Functions.				*/
	/************************************************************************/
	template <class TKey, class TValue, class HashFunctor>
	inline Hashmap<TKey, TValue, HashFunctor>::Hashmap(const uint32_t size) :
		mSize(0)
	{
		if (size == 0)
		{
			throw exception("Hashmap<TKey, TValue, HashFunctor>::Hashmap(const uint32_t size): Cannot set hashmap size to 0");
		}

		ChainType defaultChain;
		for (uint32_t i = 0; i < size; ++i)
		{
			mBuckets.PushBack(defaultChain);															//Initializing all the buckets with the default chain.
		}
	}

	template<class TKey, class TValue, typename HashFunctor>
	inline Hashmap<TKey, TValue, HashFunctor>::Hashmap(Hashmap&& rhs)
		:mSize(rhs.mSize), mBuckets(move(rhs.mBuckets))
	{
		rhs.mSize = 0;
	}

	template<class TKey, class TValue, typename HashFunctor>
	inline Hashmap<TKey, TValue, HashFunctor>& Hashmap<TKey, TValue, HashFunctor>::operator=(Hashmap&& rhs)
	{
		if (this != &rhs)
		{
			mSize = rhs.mSize;
			mBuckets = move(rhs.mBuckets);

			rhs.mSize = 0;
		}
		return *this;
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::Insert(const PairType& pair)
	{
		uint32_t index = (hashFunctor(pair.first)) % (mBuckets.Size());

		Iterator iterator = SearchBasedOnIndex(index, pair.first);
		if (iterator == end())
		{
			++mSize;
			return Iterator(this, index, mBuckets[index].PushBack(pair));								//If the key of the pair is not present in the Hashmap, then insert the pair.
		}
		return iterator;																				//Or else, return an iterator that points to the already present pair.
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::Insert(const PairType& pair, bool& isInserted)
	{
		uint32_t index = (hashFunctor(pair.first)) % (mBuckets.Size());

		Iterator iterator = SearchBasedOnIndex(index, pair.first);
		if (iterator == end())
		{
			isInserted = true;
			++mSize;
			return Iterator(this, index, mBuckets[index].PushBack(pair));								//If the key of the pair is not present in the Hashmap, then insert the pair.
		}
		isInserted = false;
		return iterator;																				//Or else, return an iterator that points to the already present pair.
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::SearchBasedOnIndex(const uint32_t index, const TKey& key) const
	{
		for (auto chainIterator = mBuckets[index].begin(); chainIterator != mBuckets[index].end(); ++chainIterator)
		{
			if ((*chainIterator).first == key)
			{
				return Iterator(this, index, chainIterator);
			}
		}

		return end();
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::Find(const TKey& key) const
	{
		uint32_t index = (hashFunctor(key)) % (mBuckets.Size());
		return SearchBasedOnIndex(index, key);
	}

	template <class TKey, class TValue, class HashFunctor>
	TValue& Hashmap<TKey, TValue, HashFunctor>::operator[](const TKey& key)
	{
		return (*Insert(make_pair(key, TValue()))).second;
	}

	template <class TKey, class TValue, class HashFunctor>
	const TValue& Hashmap<TKey, TValue, HashFunctor>::operator[](const TKey& key) const
	{
		Iterator foundIterator = Find(key);
		if (foundIterator == end())
		{
			throw out_of_range("const TValue& Hashmap<TKey, TValue, HashFunctor>::operator[](const TKey& key) const: Trying to fetch the value whose key is not present.");
		}
		return (*foundIterator).second;
	}

	template <class TKey, class TValue, class HashFunctor>
	TValue& Hashmap<TKey, TValue, HashFunctor>::At(const TKey& key)
	{
		return const_cast<TValue&>(const_cast<const Hashmap*>(this)->At(key));
	}

	template <class TKey, class TValue, class HashFunctor>
	const TValue& Hashmap<TKey, TValue, HashFunctor>::At(const TKey& key) const
	{
		return operator[](key);
	}

	template <class TKey, class TValue, class HashFunctor>
	bool Hashmap<TKey, TValue, HashFunctor>::Remove(const TKey& key)
	{
		uint32_t index = (hashFunctor(key)) % (mBuckets.Size());

		for (auto chainIterator = mBuckets[index].begin(); chainIterator != mBuckets[index].end(); ++chainIterator)
		{
			if ((*chainIterator).first == key)
			{
				mBuckets[index].Remove(chainIterator);
				--mSize;
				return true;
			}
		}
		return false;
	}

	template <class TKey, class TValue, class HashFunctor>
	uint32_t Hashmap<TKey, TValue, HashFunctor>::Size() const
	{
		return mSize;
	}

	template <class TKey, class TValue, class HashFunctor>
	bool Hashmap<TKey, TValue, HashFunctor>::ContainsKey(const TKey& key) const
	{
		return (!(Find(key) == end()));
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::begin() const
	{
		for (uint32_t i = 0; i < mBuckets.Size(); ++i)											//Do a DFS for the first found element in the Hashmap.
		{
			if (mBuckets[i].begin() != mBuckets[i].end())
			{
				return Iterator(this, i, mBuckets[i].begin());
			}
		}
		return end();
	}

	template <class TKey, class TValue, class HashFunctor>
	inline typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::end() const
	{
		return Iterator(this, mBuckets.Size() - 1, mBuckets[mBuckets.Size() - 1].end());
	}

	template <class TKey, class TValue, class HashFunctor>
	inline void Hashmap<TKey, TValue, HashFunctor>::Clear()
	{
		for (uint32_t i = 0; i < mBuckets.Size(); ++i)
		{
			mBuckets[i].Clear();
		}
		mSize = 0;
	}
	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
#pragma endregion

#pragma region Iterator Implementation	
	/************************************************************************/
	/* BEGIN	-Iterator class-		Constructors & Functions.			*/
	/************************************************************************/
	template <class TKey, class TValue, class HashFunctor>
	Hashmap<TKey, TValue, HashFunctor>::Iterator::Iterator() :
		mOwner(nullptr), mIndex(0)
	{

	}

	template <class TKey, class TValue, class HashFunctor>
	Hashmap<TKey, TValue, HashFunctor>::Iterator::Iterator(const Hashmap* owner, uint32_t index, typename ChainIterator iterator) :
		mOwner(owner), mIndex(index), mIterator(iterator)
	{

	}

	template <class TKey, class TValue, class HashFunctor>
	bool Hashmap<TKey, TValue, HashFunctor>::Iterator::operator==(const Iterator& rhs) const
	{
		return ((mOwner == rhs.mOwner) && (mIndex == rhs.mIndex) && (mIterator == rhs.mIterator));
	}

	template <class TKey, class TValue, class HashFunctor>
	bool Hashmap<TKey, TValue, HashFunctor>::Iterator::operator!=(const Iterator& rhs) const
	{
		return !(operator==(rhs));
	}

	template <class TKey, class TValue, class HashFunctor>
	typename Hashmap<TKey, TValue, HashFunctor>::Iterator& Hashmap<TKey, TValue, HashFunctor>::Iterator::operator++()
	{
		if (mIndex == mOwner->mBuckets.Size() - 1 && mIterator == mOwner->mBuckets[mIndex].end())
		{
			throw out_of_range("Hashmap<TKey, TValue, HashFunctor>::Iterator& Hashmap<TKey, TValue, HashFunctor>::Iterator::operator++(): Iterator is uninitialized or is pointing to the end of the list or is pointing to an invalid data!");
		}

		++mIterator;
		while (true)
		{
			if (mIterator != mOwner->mBuckets[mIndex].end())
			{
				return (*this);
			}
			if ((mIndex + 1) == mOwner->mBuckets.Size())
			{
				break;
			}
			++mIndex;
			mIterator = mOwner->mBuckets[mIndex].begin();
		}
		return (*this);
	}

	template <class TKey, class TValue, class HashFunctor>
	typename Hashmap<TKey, TValue, HashFunctor>::Iterator Hashmap<TKey, TValue, HashFunctor>::Iterator::operator++(int)
	{
		// Make a copy of the current Iterator
		Iterator temp(*this);

		//Increment the current Iterator
		operator++();

		//Return the copy as a VALUE and NOT by REFERENCE.
		return temp;
	}

	template <class TKey, class TValue, class HashFunctor>
	typename Hashmap<TKey, TValue, HashFunctor>::PairType& Hashmap<TKey, TValue, HashFunctor>::Iterator::operator*()
	{
		//TODO Check if owner is nullptr
		return (*mIterator);
	}

	template <class TKey, class TValue, class HashFunctor>
	typename const Hashmap<TKey, TValue, HashFunctor>::PairType& Hashmap<TKey, TValue, HashFunctor>::Iterator::operator*() const
	{
		//TODO Check if owner is nullptr
		return (*mIterator);
	}

	template <class TKey, class TValue, class HashFunctor>
	typename Hashmap<TKey, TValue, HashFunctor>::PairType* Hashmap<TKey, TValue, HashFunctor>::Iterator::operator->()
	{
		//TODO Check if owner is nullptr
		return &(*mIterator);
	}

	template <class TKey, class TValue, class HashFunctor>
	const typename Hashmap<TKey, TValue, HashFunctor>::PairType* Hashmap<TKey, TValue, HashFunctor>::Iterator::operator->() const
	{
		//TODO Check if owner is nullptr
		return &(*mIterator);
	}

	/*----------------------------------------------------------------------*/
	/* END																	*/
	/*----------------------------------------------------------------------*/
#pragma endregion
}