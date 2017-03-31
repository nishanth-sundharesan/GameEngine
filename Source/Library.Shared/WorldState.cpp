#include "Pch.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	GameTime& WorldState::GetGameTime()
	{
		// TODO:
		return mGameTime;
	}

	const GameTime& WorldState::GetGameTime() const
	{
		// TODO:
		return mGameTime;
	}

	void WorldState::SetGameTime(const GameTime& gameTime)
	{
		gameTime;
	}

	void WorldState::SetCurrentWorld(World& world)
	{
		mWorld = &world;
	}

	World& WorldState::GetCurrentWorld()
	{
		return const_cast<World&>(const_cast<const WorldState*>(this)->GetCurrentWorld());
	}

	const World& WorldState::GetCurrentWorld() const
	{
		return *mWorld;
	}

	void WorldState::SetCurrentSector(Sector& sector)
	{
		mSector = &sector;
	}

	Sector& WorldState::GetCurrentSector()
	{		
		return const_cast<Sector&>(const_cast<const WorldState*>(this)->GetCurrentSector());
	}

	const Sector& WorldState::GetCurrentSector() const
	{		
		return *mSector;
	}

	void WorldState::SetCurrentEntity(Entity& entity)
	{
		mEntity = &entity;
	}

	Entity& WorldState::GetCurrentEntity()
	{
		return const_cast<Entity&>(const_cast<const WorldState*>(this)->GetCurrentEntity());
	}

	const Entity& WorldState::GetCurrentEntity() const
	{
		return *mEntity;
	}
}