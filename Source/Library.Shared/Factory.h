#pragma once
#include "Hashmap.h"
													
namespace GameEngineLibrary
{
	template<class T>
	class Factory
	{
	public:
		virtual std::string ClassName() = 0;

		static Factory<T>* Find(const std::string& className);

		static T* Create(const std::string& className);

	protected:
		static void Add(Factory<T>& factory);

		static void Remove(Factory<T>& factory);

	private:
		virtual T* Create() = 0;

		static Hashmap<std::string, Factory<T>*> factoryHashmap;
	};	
}

#define ConcreteFactory(AbstractProduct, ConcreteProduct)								\
class ConcreteProduct ## Factory :public GameEngineLibrary::Factory<AbstractProduct>	\
{																						\
	public:																				\
		ConcreteProduct ## Factory()													\
		{																				\
			Add(*this);																	\
		}																				\
		virtual ~ConcreteProduct ## Factory()											\
		{																				\
			Remove(*this);																\
		}																				\
		virtual std::string ClassName() override										\
		{																				\
			return # ConcreteProduct;													\
		}																				\
	private:																			\
		virtual AbstractProduct* Create() override										\
		{																				\
			return new ConcreteProduct();												\
																						\
		}																				\
};																						\

#include "Factory.inl"