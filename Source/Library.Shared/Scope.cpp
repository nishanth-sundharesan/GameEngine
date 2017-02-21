#include "Pch.h"
#include "Scope.h"

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::Scope);

namespace GameEngineLibrary
{
	Scope::Scope(const uint32_t size) :
		mHashmap(max<uint32_t>(1U, size)), mVectorArray(max<uint32_t>(1U, size)), mParentScope(nullptr)
	{
		AssignEnumToString();
	}

	Scope::Scope(const Scope& rhsScope)
		:mParentScope(nullptr)
	{
		AssignEnumToString();
		PerformDeepCopy(rhsScope);
	}

	Scope& Scope::operator=(const Scope& rhsScope)
	{
		if (this != &rhsScope)
		{
			Clear();
			mParentScope = nullptr;
			PerformDeepCopy(rhsScope);
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
		const Scope *scopeToSearch = scope == nullptr ? this : *scope;
		const Datum *foundDatum = Find(name, scopeToSearch);

		while (foundDatum == nullptr && scopeToSearch != nullptr)
		{
			scopeToSearch = scopeToSearch->GetParent();
			foundDatum = Find(name, scopeToSearch);
		}

		if (foundDatum != nullptr && scope != nullptr)
		{
			*scope = const_cast<Scope*>(scopeToSearch);
		}

		return foundDatum;
	}

	Datum& Scope::Append(const string& name)
	{
		bool isNewValueInserted = false;
		mHashmapIterator = mHashmap.Insert(make_pair(name, Datum()), isNewValueInserted);

		if (isNewValueInserted)
		{
			//Push back to the order vector as we are creating a new record
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
			//Push back to the order vector as we are creating a new record
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
				throw exception("Scope& Scope::AppendScope(const string& name, Scope* newlyInsertedScope): A Datum record already exists with the same name and is not of the type DatumType::TABLE.");
			}
			else
			{
				//If a record of the same name is already present and it is of the type TABLE, then simply append a new Scope() inside the Datum. The order vector remains same.
				newlyInsertedScope->mParentScope = this;
				mHashmapIterator->second.PushBack(newlyInsertedScope);
				return *newlyInsertedScope;
			}
		}
	}

	void Scope::AssignEnumToString()
	{
		mEnumNames[static_cast<uint32_t>(DatumType::UNASSIGNED)] = "UNASSIGNED";
		mEnumNames[static_cast<uint32_t>(DatumType::INT32_T)] = "INT32_T";
		mEnumNames[static_cast<uint32_t>(DatumType::FLOAT)] = "FLOAT";
		mEnumNames[static_cast<uint32_t>(DatumType::STRING)] = "STRING";
		mEnumNames[static_cast<uint32_t>(DatumType::GLM_VECTOR4)] = "GLM_VECTOR4";
		mEnumNames[static_cast<uint32_t>(DatumType::GLM_MATRIX4X4)] = "GLM_MATRIX4X4";
		mEnumNames[static_cast<uint32_t>(DatumType::POINTER)] = "POINTER";
		mEnumNames[static_cast<uint32_t>(DatumType::TABLE)] = "TABLE";
	}

	void Scope::Adopt(Scope& childScope, const string& name)
	{
		Orphan(&childScope);
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
				Datum& tableDatum = (*mVectorArray[i]).second;
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
				Datum& datumContainingScope = (*mVectorArray[i]).second;
				for (uint32_t j = 0; j < datumContainingScope.Size(); ++j)
				{
					Scope *scopeToDelete = datumContainingScope.Get<Scope*>(j);
					if (scopeToDelete != nullptr)
					{
						scopeToDelete->mParentScope = nullptr;
						delete scopeToDelete;
					}
				}
			}
		}
		mHashmap.Clear();
		mVectorArray.Clear();
	}

	string Scope::ToString() const
	{
		string scopeString;
		for (uint32_t i = 0; i < mVectorArray.Size(); ++i)
		{
			PairType *pair = mVectorArray[i];
			scopeString += mEnumNames[static_cast<uint32_t>(pair->second.Type())] + ": " + std::to_string(pair->second.Size()) + ", ";
		}
		if (scopeString.length() > 0)
		{
			scopeString.erase(scopeString.length() - 2);
		}
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

	void Scope::Orphan(Scope* childScope)
	{
		Scope* parentScope = childScope->mParentScope;
		if (parentScope == nullptr)
		{
			throw exception("void Scope::Orphan(Scope* childScope): Cannot adopt the root scope.");
		}

		for (uint32_t i = 0; i < parentScope->mVectorArray.Size(); ++i)
		{
			if (parentScope->mVectorArray[i]->second.Type() == DatumType::TABLE)
			{
				if (parentScope->mVectorArray[i]->second.Remove(childScope))
				{
					return;
				}
			}
		}
		assert("The passed scope could not be found.");		
	}

	void Scope::PerformDeepCopy(const Scope& rhsScope)
	{
		for (uint32_t i = 0; i < rhsScope.mVectorArray.Size(); ++i)
		{
			PairType *pair = rhsScope.mVectorArray[i];
			if (pair->second.Type() == DatumType::TABLE)						//This condition could have been. if (pair->second.Type() == DatumType::TABLE && pair->second.Size() != 0)
			{																	//But for understanding purposes and for future enhancements, it is better to have the implemented way
				if (pair->second.Size() == 0)
				{
					//Perform a shallow if the Datum is set to Type TABLE but is empty.
					Datum& datum = Append(pair->first);
					datum = pair->second;
				}
				else
				{
					for (uint32_t j = 0; j < pair->second.Size(); ++j)
					{
						Scope& newScope = AppendScope(pair->first);
						newScope.PerformDeepCopy(pair->second[j]);
					}
				}
			}
			else
			{
				Datum& datum = Append(pair->first);
				datum = pair->second;
			}
		}
	}
}