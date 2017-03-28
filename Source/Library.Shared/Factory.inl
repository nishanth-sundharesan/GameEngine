#include "Pch.h"
#include "Factory.h"

using namespace std;

namespace GameEngineLibrary
{
	template<class T>
	Hashmap<string, Factory<T>*> Factory<T>::factoryHashmap;

	template<class T>
	typename Hashmap<string, Factory<T>*>::Iterator Factory<T>::begin()
	{
		return factoryHashmap.begin();
	}

	template<class T>
	typename Hashmap<string, Factory<T>*>::Iterator Factory<T>::end()
	{
		return factoryHashmap.end();
	}

	template<class T>
	Factory<T>* Factory<T>::Find(const string& className)
	{
		Hashmap<std::string, Factory<T>*>::Iterator iterator = factoryHashmap.Find(className);
		if (iterator == factoryHashmap.end())
		{
			return nullptr;
		}
		return (*iterator).second;
	}

	template<class T>
	T* Factory<T>::Create(const string& className)
	{
		Hashmap<std::string, Factory<T>*>::Iterator iterator = factoryHashmap.Find(className);
		if (iterator == factoryHashmap.end())
		{
			return nullptr;
		}
		return (*iterator).second->Create();
	}

	template<class T>
	void Factory<T>::Add(Factory<T>& factory)
	{
		factoryHashmap.Insert(make_pair(factory.ClassName(), &factory));
	}

	template<class T>
	bool Factory<T>::Remove(const Factory<T>& factory)
	{
		return factoryHashmap.Remove(factory.ClassName());
	}
}