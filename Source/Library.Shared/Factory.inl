#include "Pch.h"
#include "Factory.h"

using namespace std;

namespace GameEngineLibrary
{
	template<class T>
	Hashmap<std::string, Factory<T>*> Factory<T>::factoryHashmap;

	template<class T>
	Factory<T>* Factory<T>::Find(const string& className)
	{
		Hashmap::Iterator iterator = factoryHashmap.Find(className);
		if (iterator == factoryHashmap.end())
		{
			return nullptr;
		}
		(*iterator).second;
	}

	template<class T>
	T* Factory<T>::Create(const string& className)
	{
		Hashmap<std::string, Factory<T>*>::Iterator iterator = factoryHashmap.Find(className);
		if (iterator == factoryHashmap.end())
		{
			throw new exception("T* Factory<T>::Create(const string& className): ConcreteFactory macro is not declared for the mentioned string.");
		}

		return (*iterator).second->Create();
	}

	template<class T>
	void Factory<T>::Add(Factory<T>& factory)
	{
		factoryHashmap.Insert(make_pair(factory.ClassName(), &factory));
	}

	template<class T>
	void Factory<T>::Remove(Factory<T>& factory)
	{
		factoryHashmap.Remove(factory.ClassName());
	}
}