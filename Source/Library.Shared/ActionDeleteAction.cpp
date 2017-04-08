#include "Pch.h"
#include "ActionDeleteAction.h"
#include "World.h"

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ActionDeleteAction);

	ActionDeleteAction::ActionDeleteAction()
		:mActionToDelete(string())
	{
		InitializeSignatures();
	}

	void ActionDeleteAction::SetActionToDelete(const string& actionToDelete)
	{
		mActionToDelete = actionToDelete;
	}

	void ActionDeleteAction::Update(WorldState& worldState)
	{
		assert(mActionToDelete != string());
		Datum* datum = Search(sActionsName, nullptr);
		if (datum != nullptr)
		{
			for (uint32_t i = 0; i < datum->Size(); ++i)
			{
				assert(static_cast<Action*>(datum->Get<Scope*>(i)) != nullptr);
				Action* action = static_cast<Action*>(datum->Get<Scope*>(i));
				if (action->Name() == mActionToDelete)
				{
					assert(worldState.GetCurrentWorld() != nullptr);
					worldState.GetCurrentWorld()->AddActionToDelete(*action);
				}
			}
		}
	}

	void ActionDeleteAction::InitializeSignatures()
	{
		Action::InitializeSignatures();
	}
}