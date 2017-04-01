#pragma once
#include "Sector.h"

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

		/** Updates all the Sectors contained in the World.
		*	@param WorldState Represents the state of the World. While in this function, updates the World* to this of the WorldState..
		*/
		void Update(WorldState& worldState);

		/** Defaulted World's destructor.
		*/
		~World() = default;
	private:
		/** Initializes the signatures(member variables) of the World.
		*/
		void InitializeSignatures();

		/** The name of the World.
		*/
		std::string mName;

		/** Cached datum containing Sectors.
		*/
		Datum* mSectorDatum;

		/** The name aside which the Sectors are added to the scope.
		*/
		static const std::string sSectorsName;

		/** The attribute name for the name of the World.
		*/
		static const std::string sAttributeName;
	public:
		RTTI_DECLARATIONS(World, Attributed);
	};
}