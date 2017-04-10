#include "Pch.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	WorldState::WorldState(const GameTime& gameTime)
		:mGameTime(gameTime), mWorld(nullptr), mSector(nullptr), mEntity(nullptr), mAction(nullptr)
	{
	}

#pragma region GameTime Setters/Getters Implementations
	GameTime& WorldState::GetGameTime()
	{
		return const_cast<GameTime&>(const_cast<const WorldState*>(this)->GetGameTime());
	}

	const GameTime& WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(const GameTime& gameTime)
	{
		mGameTime = gameTime;
	}
#pragma endregion

#pragma region World Setters/Getters Implementations
	World* WorldState::GetCurrentWorld()
	{
		return const_cast<World*>(const_cast<const WorldState*>(this)->GetCurrentWorld());
	}

	const World* WorldState::GetCurrentWorld() const
	{
		return mWorld;
	}

	void WorldState::SetCurrentWorld(World* world)
	{
		mWorld = world;
	}
#pragma endregion

#pragma region Sector Setters/Getters Implementations
	Sector* WorldState::GetCurrentSector()
	{
		return const_cast<Sector*>(const_cast<const WorldState*>(this)->GetCurrentSector());
	}

	const Sector* WorldState::GetCurrentSector() const
	{
		return mSector;
	}

	void WorldState::SetCurrentSector(Sector* sector)
	{
		mSector = sector;
	}
#pragma endregion

#pragma region Entity Setters/Getters Implementations
	Entity* WorldState::GetCurrentEntity()
	{
		return const_cast<Entity*>(const_cast<const WorldState*>(this)->GetCurrentEntity());
	}

	const Entity* WorldState::GetCurrentEntity() const
	{
		return mEntity;
	}

	void WorldState::SetCurrentEntity(Entity* entity)
	{
		mEntity = entity;
	}
#pragma endregion

#pragma region Action Setters/Getters Implementations
	Action* WorldState::GetCurrentAction()
	{
		return const_cast<Action*>(const_cast<const WorldState*>(this)->GetCurrentAction());
	}

	const Action* WorldState::GetCurrentAction() const
	{
		return mAction;
	}

	void WorldState::SetCurrentAction(Action* action)
	{
		mAction = action;
	}
#pragma endregion
}