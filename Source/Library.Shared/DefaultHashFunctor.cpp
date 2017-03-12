#include "Pch.h"
#include "DefaultHashFunctor.h"

using namespace std;

namespace GameEngineLibrary
{
	uint32_t DefaultHashFunctor<char*>::operator()(const char* key) const
	{
		uint32_t hash = 0;

		while (*key != '\0')
		{			
			hash += (mPrimeNumber * hash) + static_cast<uint8_t>(*key);
			++key;
		}
		return hash;
	}

	uint32_t DefaultHashFunctor<const char*>::operator()(const char* key) const
	{
		uint32_t hash = 0;

		while (*key != '\0')
		{
			hash += (mPrimeNumber * hash) + static_cast<uint8_t>(*key);
			++key;
		}
		return hash;
	}

	uint32_t DefaultHashFunctor<string>::operator()(const string key) const
	{
		uint32_t hash = 0;

		for (uint32_t i = 0; i < key.length(); ++i)
		{
			hash += (mPrimeNumber * hash) + static_cast<uint8_t>(key[i]);
		}
		return hash;
	}

	uint32_t DefaultHashFunctor<const string>::operator()(const string key) const
	{
		uint32_t hash = 0;

		for (uint32_t i = 0; i < key.length(); ++i)
		{
			hash += (mPrimeNumber * hash) + static_cast<uint8_t>(key[i]);
		}
		return hash;
	}
}