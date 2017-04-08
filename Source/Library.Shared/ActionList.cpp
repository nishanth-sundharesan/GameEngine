#include "Pch.h"
#include "ActionList.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ActionList);

	const string ActionList::sActionsName = "Actions";

	ActionList::ActionList()
	{
		InitializeSignatures();
	}

	Datum& ActionList::Actions()
	{
		return const_cast<Datum&>(const_cast<const ActionList*>(this)->Actions());
	}

	const Datum& ActionList::Actions() const
	{
		return *mActionsDatum;
	}

	Action& ActionList::CreateAction(const string& actionClassName, const string& actionInstanceName)
	{
		Action* action = Factory<Action>::Create(actionClassName);
		action->SetName(actionInstanceName);
		AddNestedScopeAttribute(*action, sActionsName);
		return *action;
	}

	void ActionList::Update(WorldState& worldState)
	{
		worldState.SetCurrentAction(this);
		assert(mActionsDatum != nullptr);

		for (uint32_t i = 0; i < mActionsDatum->Size(); ++i)
		{
			assert(static_cast<Action*>(mActionsDatum->Get<Scope*>(i))->As<Action>() != nullptr);
			(static_cast<Action*>(mActionsDatum->Get<Scope*>(i)))->Update(worldState);
		}

		worldState.SetCurrentAction(nullptr);
	}

	void ActionList::InitializeSignatures()
	{
		Action::InitializeSignatures();
		mActionsDatum = &AddEmptyNestedScopeAttribute(sActionsName);
	}
}