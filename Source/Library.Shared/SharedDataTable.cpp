#pragma once
#include "Pch.h"
#include "SharedDataTable.h"
#include <assert.h>

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(SharedDataTable);

	SharedDataTable::SharedDataTable()
		:mScope(nullptr)
	{
	}

	void SharedDataTable::Initialize()
	{
		SharedData::Initialize();
		delete mScope;
		mScope = nullptr;
	}

	XmlParseMaster::SharedData* SharedDataTable::Clone() const
	{
		SharedData* newSharedData = new SharedDataTable();
		CloneInternalMembers(newSharedData);
		return newSharedData;
	}

	Scope* SharedDataTable::GetScope()
	{
		return const_cast<Scope*>(const_cast<const SharedDataTable*>(this)->GetScope());
	}

	const Scope* SharedDataTable::GetScope() const
	{
		return mScope;
	}

	void SharedDataTable::AppendNewScope(const string& name)
	{
		if (mScope == nullptr)
		{
			mScope = new Scope();
		}
		else
		{
			mScope = &(mScope->AppendScope(name));
		}
	}

	void SharedDataTable::GotoParentScope()
	{
		assert(mScope != nullptr);
		if (mScope->GetParent() != nullptr)
		{
			mScope = mScope->GetParent();
		}
	}

	SharedDataTable::~SharedDataTable()
	{
		delete mScope;
	}

	void SharedDataTable::CloneInternalMembers(SharedData* sharedData) const
	{
		SharedData::CloneInternalMembers(sharedData);
		if (mScope != nullptr)
		{
			(static_cast<SharedDataTable*>(sharedData)->mScope) = new Scope(*mScope);
		}		
	}
}