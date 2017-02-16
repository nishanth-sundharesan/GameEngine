#include "Pch.h"
#include "Scope.h"

using namespace std;

namespace GameEngineLibrary
{
	Scope::Scope(const uint32_t size) :
		vectorArray(1U), hashmap(1U), parentScope(nullptr)
	{
		size;
	}

	Scope::Scope(const Scope& rhs)
	{
		rhs;
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		rhs;
		return *this;
	}

	Scope::~Scope()
	{

	}
	Datum* Scope::Find(const std::string& name)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Find(name));
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		return &tempDatum;
		name;
	}

	Datum* Scope::Search(const std::string& name, Scope** scope)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Search(name, scope));
	}

	const Datum* Scope::Search(const std::string& name, Scope** scope) const
	{
		return &tempDatum;
		name;
		scope;
	}

	Datum& Scope::Append(const std::string& name)
	{
		bool isNewValueInserted = false;
		PairType pair = make_pair(name, Datum());
		hashmapIterator = hashmap.Insert(pair, isNewValueInserted);

		if (isNewValueInserted)
		{
			vectorArray.PushBack(&pair);
		}
		return (hashmapIterator->second);
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		return *this;
		name;
	}
	void Scope::Adopt(Scope& childScope, const std::string& name, const std::uint32_t index)
	{
		//Adopt calls oprhan();
		childScope;
		name;
		index;
	}

	Scope* Scope::GetParent()
	{
		return nullptr;
	}

	const Scope* Scope::GetParent() const
	{
		return nullptr;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return tempDatum;
		name;
	}

	const Datum& Scope::operator[](const std::string& name) const
	{
		//This function throws exception if it doesn't find the Datum
		return tempDatum;
		name;
	}

	Datum& Scope::operator[](const std::uint32_t index)
	{
		return tempDatum;
		index;
	}

	const Datum& Scope::operator[](const std::uint32_t index) const
	{
		return tempDatum;
		index;
	}

	bool Scope::operator==(const Scope& rhsScope) const
	{
		return false;
		rhsScope;
	}

	bool Scope::operator!=(const Scope & rhsScope) const
	{
		return false;
		rhsScope;
	}

	std::string Scope::FindName(const Scope* scope) const
	{
		return std::string();
		scope;
	}

	void Scope::Clear()
	{

	}

	void Scope::Orphan()
	{
		//detach the scope from it's parent scope.
	}
}