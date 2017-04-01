#pragma once
#include "GameTime.h"

namespace GameEngineLibrary
{
	/** Forward declared World class.
	*/
	class World;

	/** Forward declared Sector class.
	*/
	class Sector;

	/** Forward declared Entity class.
	*/
	class Entity;

	/** Forward declared Action class.
	*/
	class Action;

	/** Represents the state of the World.
	*/
	class WorldState
	{
	public:
		/** Parameterized constructor which takes in the GameTime object and caches it.
		*	@param gameTime The GameTime object which is to be cached.
		*/
		WorldState(const GameTime& gameTime);

		/** Defaulted Copy constructor.
		*/
		WorldState(const WorldState&) = default;

		/** Defaulted Assignment constructor.
		*/
		WorldState& operator=(const WorldState&) = default;

		/** Returns the reference to the cached GameTime object.
		*   @returns Returns the reference to the cached GameTime object.
		*/
		GameTime& GetGameTime();

		/** Returns the reference to the cached GameTime object.
		*   @returns Returns the reference to the cached GameTime object.
		*/
		const GameTime& GetGameTime() const;

		/** Takes in the reference to GameTime object and caches it.
		*	@param gameTime The GameTime object which is to be cached.
		*/
		void SetGameTime(const GameTime& gameTime);

		/** Returns the cached World pointer.
		*	@returns Returns the cached World pointer.
		*/
		World* GetCurrentWorld();

		/** Returns the cached World pointer.
		*	@returns Returns the cached World pointer.
		*/
		const World* GetCurrentWorld() const;

		/** Takes in the pointer to World object and caches it.
		*	@param world The World pointer which is to be cached.
		*/
		void SetCurrentWorld(World* world);
		
		/** Returns the cached Sector pointer.
		*	@returns Returns the cached Sector pointer.
		*/
		Sector* GetCurrentSector();

		/** Returns the cached Sector pointer.
		*	@returns Returns the cached Sector pointer.
		*/
		const Sector* GetCurrentSector() const;

		/** Takes in the pointer to Sector object and caches it.
		*	@param sector The Sector pointer which is to be cached.
		*/
		void SetCurrentSector(Sector* sector);
		
		/** Returns the cached Entity pointer.
		*	@returns Returns the cached Entity pointer.
		*/
		Entity* GetCurrentEntity();

		/** Returns the cached Entity pointer.
		*	@returns Returns the cached Entity pointer.
		*/
		const Entity* GetCurrentEntity() const;

		/** Takes in the pointer to Entity object and caches it.
		*	@param entity The Entity pointer which is to be cached.
		*/
		void SetCurrentEntity(Entity* entity);

		/** Defaulted destructor.
		*/
		~WorldState() = default;
	private:
		/** Cached GameTime. Stores the Game time data structure.
		*/
		GameTime mGameTime;

		/** Cached World pointer. Indicates the World at a current point of execution.
		*/
		World* mWorld;

		/** Cached Sector pointer. Indicates the Sector at a current point of execution.
		*/
		Sector* mSector;

		/** Cached Entity pointer. Indicates the Entity at a current point of execution.
		*/
		Entity* mEntity;

		/** Cached Action pointer. Indicates the Action at a current point of execution.
		*/
		Action* mAction;
	};
}
