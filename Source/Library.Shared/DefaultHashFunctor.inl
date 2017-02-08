namespace GameEngineLibrary
{
	template <typename TKey>
	uint32_t DefaultHashFunctor<TKey>::operator()(const TKey& key) const
	{
		uint32_t hash = 0;
		uint8_t* byteArray = reinterpret_cast<uint8_t*>(const_cast<TKey*>(&key));
		for (uint32_t i = 0; i < sizeof(key); ++i)
		{
			hash += 65 * byteArray[i];
		}
		return hash;
	}
}