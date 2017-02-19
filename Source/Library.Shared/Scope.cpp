#include "Pch.h"
#include "Scope.h"

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::Scope);

namespace GameEngineLibrary
{
	Scope::Scope(const uint32_t size) :
		mHashmap(max<uint32_t>(1U, size)), mVectorArray(max<uint32_t>(1U, size)), mParentScope(nullptr)
	{

	}

	Scope::Scope(const Scope& rhs)
	{
		mParentScope = rhs.mParentScope;
		PairType pair;

		for (uint32_t i = 0; i < rhs.mVectorArray.Size(); ++i)
		{
			pair = *(rhs.mVectorArray[i]);
			if (pair.second.Type() == DatumType::TABLE)
			{
				if (pair.second.Size() == 0)
				{
					Datum& datum = Append(pair.first);
					datum = pair.second;
				}
				else
				{
					for (uint32_t j = 0; j < pair.second.Size(); ++j)
					{
						Scope& tempScope = AppendScope(pair.first);
						tempScope.operator=(pair.second[j]);
					}
				}
			}
			else
			{
				Datum& datum = Append(pair.first);
				datum = pair.second;
			}			
		}
	}

	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			Clear();

			mParentScope = nullptr;
			PairType pair;

			for (uint32_t i = 0; i < rhs.mVectorArray.Size(); ++i)
			{
				pair = *(rhs.mVectorArray[i]);
				if (pair.second.Type() == DatumType::TABLE)
				{
					if (pair.second.Size() == 0)
					{
						Datum& datum = Append(pair.first);
						datum = pair.second;
					}
					else
					{
						for (uint32_t j = 0; j < pair.second.Size(); ++j)
						{
							Scope& tempScope = AppendScope(pair.first);
							tempScope = pair.second[j];
						}
					}
				}
				else
				{
					Datum& datum = Append(pair.first);
					datum = pair.second;
				}
			}
		}
		return *this;
	}

	Scope::~Scope()
	{
		Clear();
	}

	Datum* Scope::Find(const string& name)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Find(name));
	}

	const Datum* Scope::Find(const string& name) const
	{
		return Find(name, this);
	}

	Datum* Scope::Find(const string& name, const Scope* scope)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Find(name, scope));
	}

	const Datum* Scope::Find(const string& name, const Scope* scope) const
	{
		if (scope == nullptr)
		{
			return nullptr;
		}

		mHashmapIterator = (scope->mHashmap.Find(name));
		if (mHashmapIterator == scope->mHashmap.end())
		{
			return nullptr;
		}
		else
		{
			return &(mHashmapIterator->second);
		}
	}

	Datum* Scope::Search(const string& name, Scope** scope)
	{
		return const_cast<Datum*>(const_cast<const Scope*>(this)->Search(name, scope));
	}

	const Datum* Scope::Search(const string& name, Scope** scope) const
	{
		if (scope == nullptr)
		{
			const Scope *scopeToSearch = this;
			const Datum *foundDatum = Find(name, scopeToSearch);

			while (foundDatum == nullptr && scopeToSearch != nullptr)
			{
				scopeToSearch = scopeToSearch->GetParent();
				foundDatum = Find(name, scopeToSearch);
			}

			return foundDatum;
		}
		else
		{
			return (*scope)->Find(name);
		}
	}

	Datum& Scope::Append(const string& name)
	{
		bool isNewValueInserted = false;
		mHashmapIterator = mHashmap.Insert(make_pair(name, Datum()), isNewValueInserted);

		if (isNewValueInserted)
		{
			mVectorArray.PushBack(&(*mHashmapIterator));
		}
		return (mHashmapIterator->second);
	}

	Scope& Scope::AppendScope(const string& name)
	{
		return AppendScope(name, new Scope());
	}

	Scope& Scope::AppendScope(const string& name, Scope* newlyInsertedScope)
	{
		bool isNewValueInserted = false;

		mHashmapIterator = mHashmap.Insert(make_pair(name, Datum()), isNewValueInserted);
		if (isNewValueInserted)
		{
			mVectorArray.PushBack(&(*mHashmapIterator));
			mHashmapIterator->second.SetType(DatumType::TABLE);

			newlyInsertedScope->mParentScope = this;
			mHashmapIterator->second.PushBack(newlyInsertedScope);

			return *newlyInsertedScope;
		}
		else
		{
			if ((mHashmapIterator->second).Type() != DatumType::TABLE)
			{
				throw exception("Scope& Scope::AppendScope(const std::string& name): A Datum record already exists with the same name and is not of the type  DatumType::TABLE.");
			}
			else
			{
				newlyInsertedScope->mParentScope = this;
				mHashmapIterator->second.PushBack(newlyInsertedScope);

				return *newlyInsertedScope;
			}
		}
	}

	void Scope::Adopt(Scope& childScope, const string& name)
	{
		Orphan(childScope, name);
		childScope.mParentScope = this;
		AppendScope(name, &childScope);
	}

	Scope* Scope::GetParent()
	{
		return const_cast<Scope*>(const_cast<const Scope*>(this)->GetParent());
	}

	const Scope* Scope::GetParent() const
	{
		return mParentScope;
	}

	Datum& Scope::operator[](const string& name)
	{
		return Append(name);
	}

	const Datum& Scope::operator[](const string& name) const
	{
		return mHashmap[name];
	}

	Datum& Scope::operator[](const uint32_t index)
	{
		return const_cast<Datum&>(const_cast<const Scope*>(this)->operator[](index));
	}

	const Datum& Scope::operator[](const uint32_t index) const
	{
		if (index >= mVectorArray.Size())
		{
			throw out_of_range("const Datum& Scope::operator[](const std::uint32_t index) const: The specified index is out of range.");
		}
		return *(Find((*mVectorArray[index]).first));
	}

	bool Scope::operator==(const Scope& rhsScope) const
	{
		if (this == &rhsScope)
		{
			return true;
		}
		if (mVectorArray.Size() != rhsScope.mVectorArray.Size())
		{
			return false;
		}
		for (uint32_t i = 0; i < mVectorArray.Size(); ++i)
		{
			if (*mVectorArray[i] != *rhsScope.mVectorArray[i])
			{
				return false;
			}
		}
		return true;
	}

	bool Scope::operator!=(const Scope& rhsScope) const
	{
		return !(operator==(rhsScope));
	}

	string Scope::FindName(const Scope* scope) const
	{
		for (uint32_t i = 0; i < mVectorArray.Size(); ++i)
		{
			if ((*mVectorArray[i]).second.Type() == DatumType::TABLE)
			{
				Datum tableDatum = (*mVectorArray[i]).second;
				for (uint32_t j = 0; j < tableDatum.Size(); ++j)
				{
					if (&tableDatum[j] == scope)
					{
						return (*mVectorArray[i]).first;
					}
				}
			}
		}
		return string();
	}

	void Scope::Clear()
	{
		for (uint32_t i = 0; i < mVectorArray.Size(); ++i)
		{
			if ((*mVectorArray[i]).second.Type() == DatumType::TABLE)
			{
				Datum datumContainingScope = (*mVectorArray[i]).second;
				Scope *scopeToDelete;
				for (uint32_t j = 0; j < datumContainingScope.Size(); ++j)
				{
					scopeToDelete = datumContainingScope.Get<Scope*>(j);
					if (scopeToDelete != nullptr)
					{
						scopeToDelete->mParentScope = nullptr;
						delete scopeToDelete;
					}
				}
			}
			else
			{
				((*mVectorArray[i]).second).~Datum();
			}
		}
		mHashmap.Clear();
		mVectorArray.Clear();
	}

	std::string Scope::ToString() const
	{
		//DatumType type;
		string scopeString;
		for (uint32_t i = 0; i < mVectorArray.Size() - 1; ++i)
		{
			scopeString += std::to_string((*mVectorArray[i]).second.Size()) + ", ";
		}
		scopeString += std::to_string((*mVectorArray[mVectorArray.Size() - 1]).second.Size());

		return scopeString;
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr)
		{
			return false;
		}
		if (this == rhs)
		{
			return true;
		}
		Scope* rhsScope = rhs->As<Scope>();
		return rhsScope == nullptr ? false : *this == *rhsScope;
	}

	void Scope::Orphan(Scope& childScope, const string& name)
	{
		//detach the scope from it's parent scope.
		Scope* parentScope = childScope.mParentScope;
		if (parentScope == nullptr)
		{
			throw exception("void Scope::Orphan(Scope& childScope): Cannot adopt the root scope.");
		}

		mHashmapIterator = parentScope->mHashmap.Find(name);
		if (mHashmapIterator == parentScope->mHashmap.end())
		{
			throw exception("void Scope::Orphan(Scope& childScope, const std::string& name): Cannot find the passed childScope.");
		}
		parentScope->mVectorArray.Remove(&(*mHashmapIterator));
		parentScope->mHashmap.Remove(name);
	}
}