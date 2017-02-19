#pragma once
#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "RTTI.h"

namespace GameEngineLibrary
{
	class Scope : public RTTI
	{
		typedef std::pair<std::string, Datum> PairType;

		typedef Hashmap<std::string, Datum> Hashmap;

		typedef Hashmap::Iterator HashmapIterator;

	public:
		Scope(const std::uint32_t size = 1);

		Scope(const Scope& rhs);

		Scope& operator=(const Scope& rhs);

		virtual ~Scope();

		Datum* Find(const std::string& name);

		const Datum* Find(const std::string& name) const;

		Datum* Find(const std::string& name, const Scope* scope);

		const Datum* Find(const std::string& name, const Scope* scope) const;

		Datum* Search(const std::string& name, Scope** scope = nullptr);

		const Datum* Search(const std::string& name, Scope** scope = nullptr) const;

		Datum& Append(const std::string& name);

		Scope& AppendScope(const std::string& name);

		void Adopt(Scope& childScope, const std::string& name);

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

		virtual std::string ToString() const override;

	private:
		//Helper Functions
		Scope& AppendScope(const string& name, Scope* scope);

		void Orphan(Scope& childScope, const std::string& name);

		virtual bool Equals(const RTTI* rhs) const override;

		Vector<PairType*> mVectorArray;

		Hashmap mHashmap;

		mutable HashmapIterator mHashmapIterator;

		Scope* mParentScope;		

		RTTI_DECLARATIONS(Scope, RTTI);
	};
}