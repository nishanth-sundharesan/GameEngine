#pragma once
#include "Action.h"

namespace UnitTestSupportClasses
{
	class AttackAction :public GameEngineLibrary::Action
	{
	public:
		AttackAction();

		virtual void Update(GameEngineLibrary::WorldState& worldState) override;

		bool IsAttackCalled() const;

		virtual ~AttackAction() = default;

	protected:
		virtual void InitializeSignatures() override;

	private:
		bool mIsAttackCalled;

	public:
		RTTI_DECLARATIONS(AttackAction, Action);
	};

	ActionFactory(AttackAction);
}