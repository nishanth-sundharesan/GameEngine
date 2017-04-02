#include "Pch.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	WorldState::WorldState(const GameTime& gameTime)
		:mGameTime(gameTime)
	{
	}

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
}