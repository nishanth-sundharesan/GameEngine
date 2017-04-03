#include "Pch.h"
#include "MonsterEntity.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	RTTI_DEFINITIONS(MonsterEntity);

	MonsterEntity::MonsterEntity()
	{
		InitializeSignatures();
	}

	void MonsterEntity::Update(GameEngineLibrary::WorldState& worldState)
	{
		Entity::Update(worldState);
	}

	void MonsterEntity::InitializeSignatures()
	{
		Entity::InitializeSignatures();
	}
}