#pragma once
#include "Sector.h"
#include "EventQueue.h"

namespace GameEngineLibrary
{
	/** Finalized World class which contains all the game data.
	*/
	class World final :public Attributed
	{
	public:
		/** Parameterized constructor which takes in the name of the World.
		*	@param name The name of the World to be assigned.
		*/
		World(const std::string& name);

		/** Marked copy constructor as delete. Cannot copy Worlds.
		*/
		World(const World&) = delete;

		/** Marked assignment operator as delete. Cannot copy Worlds.
		*/
		World& operator=(const World&) = delete;

		/** Returns the name of the World.
		*	@returns Returns the name of the World.
		*/
		std::string& Name();

		/** Returns the name of the World.
		*	@returns Returns the name of the World.
		*/
		const std::string& Name() const;

		/** Sets the name of the World.
		*	@param name The new name of the World.
		*/
		void SetName(const std::string& name);
		
		/** Returns the Datum containing the Sectors.
		*	@returns Returns the Datum containing the Sectors.
		*/
		Datum& Sectors();

		/** Returns the Datum containing the Sectors.
		*	@returns Returns the Datum containing the Sectors.
		*/
		const Datum& Sectors() const;

		/** Creates a new Sector inside the World and returns it.
		*	@param name The name of the Sector to be created.
		*	@returns Returns the newly created Sector.
		*/
		Sector& CreateSector(const std::string& name);

		/** Adopts the passed Sector into the current World.
		*	@param sector The Sector to be adopted to the current World.
		*/
		void AdoptSector(Sector& sector);

		/** Takes in the path and searches for the specified Datum.
		*	@param path The path to the Datum delimited by '.' .
		*	@returns Returns the reference to found Datum.
		*	@exception Throws an exception if the datum is not found.
		*/
		Datum& SearchDatum(const std::string& path);

		/** Takes in the path and searches for the specified Datum.
		*	@param path The path to the Datum delimited by '.' .
		*	@returns Returns the reference to found Datum.
		*	@exception Throws an exception if the datum is not found.
		*/
		const Datum& SearchDatum(const std::string& path) const;

		/** Adds the action to the Vector, which will be after a frame.
		*	@param actionToDelete The action to delete.
		*/
		void AddActionToDelete(Action& actionToDelete);

		/** Updates all the Sectors contained in the World.
		*	@param WorldState Represents the state of the World. While in this function, updates the World* to this of the WorldState.
		*/
		void Update(WorldState& worldState);

		/** Returns the cached EventQueue.
		*	@returns Returns the cached EventQueue.
		*/
		EventQueue& GetEventQueue();

		/** Returns the cached EventQueue.
		*	@returns Returns the cached EventQueue.
		*/
		const EventQueue& GetEventQueue() const;

		/** Defaulted World's destructor.
		*/
		~World() = default;
	private:		
		/** Private helper method to fetch the next data chunk.
		*	@param path The data from which the chunk has to be fetched.
		*	@returns Returns the next data chunk present after the delimiter.
		*/
		const char* FindNextDataChunk(char* path = nullptr) const;

		/** Initializes the signatures(member variables) of the World.
		*/
		void InitializeSignatures();

		/** The name aside which the Sectors are added to the scope.
		*/
		static const std::string sSectorsName;

		/** The attribute name for the name of the World.
		*/
		static const std::string sAttributeName;

		/** The delimiter string for evaluating path.
		*/
		static const char* sDelimiterString;

		/** The name of the World.
		*/
		std::string mName;

		/** Cached datum containing Sectors.
		*/
		const Datum* mSectorDatum;

		/** Event queue for the game engine.
		*/
		EventQueue mEventQueue;

		/** List of Action pointers to delete.
		*/
		Vector<Action*> mActionsToDelete;
	public:
		RTTI_DECLARATIONS(World, Attributed);
	};
}