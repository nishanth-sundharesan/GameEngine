#pragma once
namespace GameEngineLibrary
{
	template<class TKey>
	class DefaultHashFunctor
	{
	public:
		//Perform a Bernstein's Hash
		std::uint32_t DefaultHashFunctor::operator()(const TKey& key) const
		{
			uint32_t hash = 0;
			uint8_t* byteArray = reinterpret_cast<uint8_t*>(const_cast<TKey*>(&key));
			for (uint32_t i = 0; i < sizeof(key); ++i)
			{
				hash += 65 * byteArray[i];
			}
			return hash;
		}
	};

	template<>
	class DefaultHashFunctor<char*>
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
}