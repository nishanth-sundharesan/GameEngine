#include "Pch.h"
#include "HealAction.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	RTTI_DEFINITIONS(HealAction);

	HealAction::HealAction()
		:mIsHealCalled(false)
	{
		InitializeSignatures();
	}

	void HealAction::Update(WorldState& worldState)
	{
		worldState.SetCurrentAction(this);

		mIsHealCalled = true;

		worldState.SetCurrentAction(nullptr);
	}

	bool HealAction::IsHealCalled() const
	{
		return mIsHealCalled;
	}

	void HealAction::InitializeSignatures()
	{
		Action::InitializeSignatures();
	}
}