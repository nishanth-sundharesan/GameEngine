#include "Pch.h"
#include "ActionCreateAction.h"
#include "Entity.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ActionCreateAction);

	ActionCreateAction::ActionCreateAction()
		:mActionClassName(string()), mActionInstanceName(string())
	{
		InitializeSignatures();
	}

	void ActionCreateAction::SetClassAndInstanceName(const string& actionClassName, const string& actionInstanceName)
	{
		mActionClassName = actionClassName;
		mActionInstanceName = actionInstanceName;
	}

	void ActionCreateAction::Update(WorldState& worldState)
	{
		assert(mActionClassName != string());
		assert(mActionInstanceName != string());
		if (worldState.GetCurrentAction() != nullptr)
		{
			assert(static_cast<ActionList*>(worldState.GetCurrentAction()) != nullptr);
			static_cast<ActionList*>(worldState.GetCurrentAction())->CreateAction(mActionClassName, mActionInstanceName);
		}
		else
		{
			worldState.GetCurrentEntity()->CreateAction(mActionClassName, mActionInstanceName);;
		}
	}

	void ActionCreateAction::InitializeSignatures()
	{
		Action::InitializeSignatures();
	}
}