#include "Pch.h"
#include "AttackAction.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	RTTI_DEFINITIONS(AttackAction);

	AttackAction::AttackAction()
		:mIsAttackCalled(false)
	{
		InitializeSignatures();
	}

	void AttackAction::Update(WorldState& worldState)
	{
		worldState.SetCurrentAction(this);

		mIsAttackCalled = true;

		worldState.SetCurrentAction(nullptr);
	}

	bool AttackAction::IsAttackCalled()
	{
		return mIsAttackCalled;
	}

	void AttackAction::InitializeSignatures()
	{
		Action::InitializeSignatures();
	}
}