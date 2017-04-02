#pragma once
#include "Hashmap.h"

namespace GameEngineLibrary
{
	/** Templated Factory class
	*/
	template<class T>
	class Factory
	{
	public:
		typedef typename Hashmap<std::string, Factory<T>*>::Iterator HashmapIterator;

		/** Defaulted constructor.
		*/
		Factory() = default;

		/** Defaulted copy constructor.
		*/
		Factory(const Factory&) = default;

		/** Defaulted assignment operator.
		*/
		Factory& operator=(const Factory&) = default;
		
		/** Pure virtual function which should return the class name as a std::string.
		*	@return Returns the class name as a std::string.
		*/
		virtual const std::string& ClassName() const = 0;

		/** Returns the Hashmap's begin iterator.
		*	@returns Returns the Hashmap's begin iterator.
		*/
		static HashmapIterator begin();

		/** Returns the Hashmap's end iterator.
		*	@returns Returns the Hashmap's end iterator.
		*/
		static HashmapIterator end();

		/** Static function which takes in the name of the class as a string and returns a base class pointer to that instance.
		*	The function returns nullptr if the className was not found.
		*	@return Returns a base class pointer to the instance mapped to the className string. Returns nullptr if the className was not found.
		*/
		static Factory<T>* Find(const std::string& className);

		/** Static function which takes in the name of the class as a string and creates an instance of that class.
		*	@return Returns a pointer to the newly created instance, returns nullptr if there isn't any class which matches the passed string className.
		*/
		static T* Create(const std::string& className);
	protected:
		/** Static function which takes in a reference to the Factory<T> object and adds its pointer along with its key(class's name) to the static Hashmap.
		*	@param factory Reference to the factory object which needs to be added to the static Hashmap.
		*/
		static void Add(Factory<T>& factory);

		/** Static function which takes in a reference to the Factory<T> object and removes its pointer from the static Hashmap.
		*	@param factory Reference to the factory object which needs to be removed from the static Hashmap.
		*/
		static bool Remove(const Factory<T>& factory);
	private:
		/** Pure virtual function which is intended for the derived class to override and create instances of itself and return it.
		*	@returns Returns the pointer to the newly created instance of the derived class.
		*/
		virtual T* Create() const = 0;

		/** Static Hashmap which stores in the pointers to Factory<T> objects along with its key(class's name).
		*/
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
		virtual const std::string& ClassName() const override							\
		{																				\
			return className;															\
		}																				\
	private:																			\
		virtual AbstractProduct* Create() const override								\
		{																				\
			return new ConcreteProduct();												\
																						\
		}																				\
		std::string className = # ConcreteProduct;										\
};																						\

#include "Factory.inl"