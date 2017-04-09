#pragma once
#include "Action.h"

namespace UnitTestSupportClasses
{
	class HealAction :public GameEngineLibrary::Action
	{
	public:
		HealAction();

		virtual void Update(GameEngineLibrary::WorldState& worldState) override;

		bool IsHealCalled();

		virtual ~HealAction() = default;

	protected:
		virtual void InitializeSignatures() override;

	private:
		bool mIsHealCalled;

	public:
		RTTI_DECLARATIONS(HealAction, Action);
	};

	ActionFactory(HealAction);
}