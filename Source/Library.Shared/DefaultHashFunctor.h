#pragma once
namespace GameEngineLibrary
{
	const std::uint32_t mPrimeNumber = 65;

	//Default compare hash functor
	// TODO - This will be called from inside of Hashmap. A functor could be overloaded for char*
	//bool DefaultCompare<TKey>::operator()(const TKey& lhs, const TKey& rhs) const
	//{
	//	return lhs == rhs;
	//}

	template<class TKey>
	class DefaultHashFunctor
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const TKey& key) const;
	};

	template<>
	class DefaultHashFunctor<char*>
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const char* key) const;
	};

	template<>
	class DefaultHashFunctor<const char*>
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const char* key) const;
	};

	template<>
	class DefaultHashFunctor<std::string>
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const std::string key) const;
	};

	template<>
	class DefaultHashFunctor<const std::string>
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const std::string key) const;
	};
}

#include "DefaultHashFunctor.inl"