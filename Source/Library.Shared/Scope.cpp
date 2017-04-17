#include "Pch.h"
#include "Scope.h"
#include <assert.h>

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::Scope);

namespace GameEngineLibrary
{
	Scope::Scope(const uint32_t size) :
		mHashmap(max<uint32_t>(1U, size)), mOrderVector(max<uint32_t>(1U, size)), mParentScope(nullptr)
	{
		AssignEnumToString();
	}

	Scope::Scope(const Scope& rhsScope)
		:mParentScope(nullptr)
	{
		AssignEnumToString();
		PerformDeepCopy(rhsScope);
	}

	Scope::Scope(Scope&& rhs)
		:mParentScope(rhs.mParentScope), mOrderVector(move(rhs.mOrderVector)), mHashmap(move(rhs.mHashmap))
	{
		//Going through the child scopes and reassigning the parent pointer.
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
		{
			if (mOrderVector[i]->second.Type() == DatumType::TABLE)
			{
				Datum& datumContainingScope = (*mOrderVector[i]).second;
				for (uint32_t j = 0; j < datumContainingScope.Size(); ++j)
				{
					Scope *scopeToDelete = datumContainingScope.Get<Scope*>(j);
					if (scopeToDelete != nullptr)
					{
						scopeToDelete->mParentScope = this;
					}
				}
			}
		}
		PointerFixUp(&rhs);
		AssignEnumToString();

		rhs.mParentScope = nullptr;
		rhs.mOrderVector.Clear();
		rhs.mHashmap.Clear();
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

	Scope& Scope::operator=(Scope&& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			mParentScope = rhs.mParentScope;
			mOrderVector = move(rhs.mOrderVector);
			mHashmap = move(rhs.mHashmap);
			
			//Going through the child scopes and reassigning the parent pointer.
			for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
			{
				if (mOrderVector[i]->second.Type() == DatumType::TABLE)
				{
					Datum& datumContainingScope = mOrderVector[i]->second;
					for (uint32_t j = 0; j < datumContainingScope.Size(); ++j)
					{
						Scope *scopeToDelete = datumContainingScope.Get<Scope*>(j);
						if (scopeToDelete != nullptr)
						{
							scopeToDelete->mParentScope = this;
						}
					}
				}
			}
			PointerFixUp(&rhs);					
			rhs.mParentScope = nullptr;			
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
		mHashmapIterator = (mHashmap.Find(name));
		if (mHashmapIterator == mHashmap.end())
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
		const Datum *foundDatum = scopeToSearch->Find(name);

		while (foundDatum == nullptr && scopeToSearch->GetParent() != nullptr)
		{
			scopeToSearch = scopeToSearch->GetParent();
			foundDatum = scopeToSearch->Find(name);
		}

		if (foundDatum != nullptr && scope != nullptr)
		{
			*scope = const_cast<Scope*>(scopeToSearch);
		}

		return foundDatum;
	}

	Datum& Scope::Append(const string& name)
	{
		if (name == "")
		{
			throw exception("Datum& Scope::Append(const string& name): The name of the scope to adopt is empty.");
		}

		bool isNewValueInserted = false;
		mHashmapIterator = mHashmap.Insert(make_pair(name, Datum()), isNewValueInserted);

		if (isNewValueInserted)
		{
			//Push back to the order vector as we are creating a new record
			mOrderVector.PushBack(&(*mHashmapIterator));
		}
		return (mHashmapIterator->second);
	}

	Scope& Scope::AppendScope(const string& name)
	{
		return AppendScope(name, new Scope());
	}

	Scope& Scope::AppendScope(const string& name, Scope* newlyInsertedScope)
	{
		if (name == "")
		{
			throw exception("Scope& Scope::AppendScope(const string& name, Scope* newlyInsertedScope): The name of the scope to append is empty.");
		}

		bool isNewValueInserted = false;
		mHashmapIterator = mHashmap.Insert(make_pair(name, Datum()), isNewValueInserted);
		if (isNewValueInserted)
		{
			//Push back to the order vector as we are creating a new record
			mOrderVector.PushBack(&(*mHashmapIterator));
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

	void Scope::PointerFixUp(Scope* oldScope)
	{
		//Making sure that the parent is pointing to the appropriate child
		if (mParentScope != nullptr)
		{
			for (uint32_t i = 0; i < mParentScope->mOrderVector.Size(); ++i)
			{
				if (mParentScope->mOrderVector[i]->second.Type() == DatumType::TABLE)
				{
					Datum& datumContainingScope = mParentScope->mOrderVector[i]->second;
					for (uint32_t j = 0; j < datumContainingScope.Size(); ++j)
					{
						if (datumContainingScope.Get<Scope*>(j) == oldScope)
						{
							datumContainingScope.Set(this, j);
							return;
						}
					}
				}
			}
		}
		assert("Pointer Fix up failed");
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
		if (name == "")
		{
			throw exception("void Scope::Adopt(Scope& childScope, const string& name): The name of the scope to adopt is empty.");
		}

		if (this == &childScope)
		{
			throw exception("void Scope::Adopt(Scope& childScope, const string& name): Cannot adopt itself.");
		}

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

	uint32_t Scope::Size() const
	{
		return mOrderVector.Size();
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
		// TODO remove the exception check. Vector already does it.
		if (index >= mOrderVector.Size())
		{
			throw out_of_range("const Datum& Scope::operator[](const std::uint32_t index) const: The specified index is out of range.");
		}
		return mOrderVector[index]->second;
	}

	bool Scope::operator==(const Scope& rhsScope) const
	{
		if (this == &rhsScope)
		{
			return true;
		}
		if (mOrderVector.Size() != rhsScope.mOrderVector.Size())
		{
			return false;
		}
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
		{
			if (*mOrderVector[i] != *rhsScope.mOrderVector[i])
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
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
		{
			if ((*mOrderVector[i]).second.Type() == DatumType::TABLE)
			{
				Datum& tableDatum = (*mOrderVector[i]).second;
				for (uint32_t j = 0; j < tableDatum.Size(); ++j)
				{
					if (&tableDatum[j] == scope)
					{
						return (*mOrderVector[i]).first;
					}
				}
			}
		}
		return string();
	}

	void Scope::Clear()
	{
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
		{
			if ((*mOrderVector[i]).second.Type() == DatumType::TABLE)
			{
				Datum& datumContainingScope = (*mOrderVector[i]).second;
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
		mOrderVector.Clear();
	}

	string Scope::ToString() const
	{
		string scopeString;
		for (uint32_t i = 0; i < mOrderVector.Size(); ++i)
		{
			PairType *pair = mOrderVector[i];
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

	const Vector<pair<string, Datum>*>& Scope::GetAllAttributes() const
	{
		return mOrderVector;
	}

	void Scope::Orphan(Scope* childScope)
	{
		Scope* parentScope = childScope->mParentScope;
		if (parentScope == nullptr)
		{
			return;			
		}

		for (uint32_t i = 0; i < parentScope->mOrderVector.Size(); ++i)
		{
			if (parentScope->mOrderVector[i]->second.Type() == DatumType::TABLE)
			{
				if (parentScope->mOrderVector[i]->second.Remove(childScope))
				{
					return;
				}
			}
		}
		assert("The passed scope could not be found.");
	}

	void Scope::PerformDeepCopy(const Scope& rhsScope)
	{
		mOrderVector.Reserve(rhsScope.mOrderVector.Size());

		for (uint32_t i = 0; i < rhsScope.mOrderVector.Size(); ++i)
		{
			PairType *pair = rhsScope.mOrderVector[i];
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