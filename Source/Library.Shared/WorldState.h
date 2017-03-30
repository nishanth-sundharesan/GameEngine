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
	private:
		GameTime mGameTime;

		World* mWorld;

		Sector* mSector;

		Entity* mEntity;

		Action* mAction;
	};
}
