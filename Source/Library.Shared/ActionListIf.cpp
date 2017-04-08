#include "Pch.h"
#include "ActionListIf.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ActionListIf);

	const string ActionListIf::sConditionAttributeName = "Condition";

	ActionListIf::ActionListIf()
		:mLhsValueDatum(nullptr), mRhsValueDatum(nullptr), mThenBlockAction(nullptr), mElseBlockAction(nullptr)
	{
		InitializeSignatures();
	}

	void ActionListIf::Update(WorldState& worldState)
	{
		worldState.SetCurrentAction(this);
		assert(mLhsValueDatum != nullptr);
		assert(mRhsValueDatum != nullptr);
		assert(mThenBlockAction != nullptr);

		if (*mLhsValueDatum == *mRhsValueDatum)
		{
			mThenBlockAction->Update(worldState);
		}
		else if (mElseBlockAction != nullptr)
		{
			mElseBlockAction->Update(worldState);
		}
		worldState.SetCurrentAction(nullptr);
	}

	void ActionListIf::SetComparisons(const Datum& lhsValue, const Datum& rhsValue)
	{
		mLhsValueDatum = &lhsValue;
		mRhsValueDatum = &rhsValue;
	}

	void ActionListIf::SetThenBlockAction(const Action& thenBlockAction)
	{
		mThenBlockAction = const_cast<Action*>(&thenBlockAction);
	}

	void ActionListIf::SetElseBlockAction(const Action& elseBlockAction)
	{
		mElseBlockAction = const_cast<Action*>(&elseBlockAction);
	}

	void ActionListIf::InitializeSignatures()
	{
		Action::InitializeSignatures();
	}
}