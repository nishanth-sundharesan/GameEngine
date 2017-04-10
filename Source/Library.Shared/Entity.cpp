#include "Pch.h"
#include "Entity.h"
#include "Sector.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Entity);

	const string Entity::sActionsName = "Actions";
	const string Entity::sActionCreateActionName = "CreateAction";
	const string Entity::sAttributeName = "Name";

	Entity::Entity()
	{
		InitializeSignatures();
	}

	string& Entity::Name()
	{
		return const_cast<string&>(const_cast<const Entity*>(this)->Name());
	}

	const string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const string& name)
	{
		mName = name;
	}

	Datum& Entity::Actions()
	{
		return const_cast<Datum&>(const_cast<const Entity*>(this)->Actions());
	}

	const Datum& Entity::Actions() const
	{
		return *mActionsDatum;
	}

	ActionList& Entity::CreateActionList(const string& actionListName)
	{
		ActionList* actionList = new ActionList();
		actionList->SetName(actionListName);
		AddNestedScopeAttribute(*actionList, sActionsName);
		return *actionList;
	}

	Action& Entity::CreateAction(const string& actionClassName, const string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		action->SetName(actionInstanceName);
		AddNestedScopeAttribute(*action, sActionsName);
		return *action;
	}	
	
	Sector& Entity::GetSector()
	{
		return const_cast<Sector&>(const_cast<const Entity*>(this)->GetSector());
	}

	const Sector& Entity::GetSector() const
	{
		assert(mParentScope->Is("Sector"));
		return *(static_cast<const Sector*>(mParentScope));
	}

	void Entity::Update(WorldState& worldState)
	{
		worldState.SetCurrentEntity(this);
		assert(mActionsDatum != nullptr);

		for (uint32_t i = 0; i < mActionsDatum->Size(); ++i)
		{
			assert(static_cast<Action*>(mActionsDatum->Get<Scope*>(i))->As<Action>() != nullptr);
			(static_cast<Action*>(mActionsDatum->Get<Scope*>(i)))->Update(worldState);
		}
		worldState.SetCurrentEntity(nullptr);
	}

	void Entity::InitializeSignatures()
	{
		AddExternalAttribute(sAttributeName, &mName, 1);
		mActionsDatum = &AddEmptyNestedScopeAttribute(sActionsName);
	}
}