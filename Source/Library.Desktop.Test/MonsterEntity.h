#pragma once
#include "Entity.h"
#include "Factory.h"

namespace UnitTestSupportClasses
{
	class MonsterEntity :public GameEngineLibrary::Entity
	{
	public:
		MonsterEntity();

		MonsterEntity(const MonsterEntity&) = delete;

		MonsterEntity& operator=(const MonsterEntity&) = delete;

		virtual void Update(GameEngineLibrary::WorldState& worldState);

		~MonsterEntity() = default;
	protected:
		virtual void InitializeSignatures() override;

	public:
		RTTI_DECLARATIONS(MonsterEntity, Entity);
	};

	ConcreteFactory(GameEngineLibrary::Entity, MonsterEntity);
}