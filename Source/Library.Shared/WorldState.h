#pragma once
#include "GameTime.h"

namespace GameEngineLibrary
{
	class World;

	class Sector;

	class Entity;

	class Action;

	class WorldState
	{
	public:
		GameTime& GetGameTime();

		const GameTime& GetGameTime() const;

		void SetGameTime(const GameTime& gameTime);

		void SetCurrentWorld(World& world);

		World& GetCurrentWorld();

		const World& GetCurrentWorld() const;

		void SetCurrentSector(Sector& sector);

		Sector& GetCurrentSector();

		const Sector& GetCurrentSector() const;

		void SetCurrentEntity(Entity& entity);

		Entity& GetCurrentEntity();

		const Entity& GetCurrentEntity() const;
	private:
		GameTime mGameTime;

		World* mWorld;

		Sector* mSector;

		Entity* mEntity;

		Action* mAction;
	};
}
