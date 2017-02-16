#pragma once
#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"

namespace GameEngineLibrary
{
	class Scope
	{
		typedef std::pair<std::string, Datum> PairType;

	public:
		Scope(const std::uint32_t size = 1);

		Scope(const Scope& rhs);

		Scope& operator=(const Scope& rhs);

		~Scope();

		Datum* Find(const std::string& name);

		const Datum* Find(const std::string& name) const;

		Datum* Search(const std::string& name, Scope** scope = nullptr);

		const Datum* Search(const std::string& name, Scope** scope = nullptr) const;

		Datum& Append(const std::string& name);

		Scope& AppendScope(const std::string& name);

		void Adopt(Scope& childScope, const std::string& name, const std::uint32_t index);

		Scope* GetParent(); 

		const Scope* GetParent() const;

		Datum& operator[](const std::string& name);

		const Datum& operator[](const std::string& name) const;

		Datum& operator[](const std::uint32_t index);

		const Datum& operator[](const std::uint32_t index) const;

		bool operator==(const Scope& rhsScope) const;

		bool operator!=(const Scope& rhsScope) const;

		std::string FindName(const Scope* scope) const;

		void Clear();

	private:
		Vector<PairType*> vectorArray;

		Hashmap<std::string, Datum> hashmap;

		Hashmap<std::string, Datum>::Iterator hashmapIterator;

		Scope* parentScope;

		void Orphan();

		//TODO - remove later
		Datum tempDatum;
	};
}