#pragma once
#include "Entity.h"

namespace GameEngineLibrary
{
	/** Forward declared World class.
	*/
	class World;

	/** Finalized sector class containing chunks of data.
	*/
	class Sector final :public Attributed
	{
	public:
		/** Parameterized constructor which takes in the name of the Sector.
		*	@param name The name of the Sector to be assigned.
		*/
		Sector(const std::string& name);

		/** Marked copy constructor as delete. Cannot copy Sectors.
		*/
		Sector(const Sector&) = delete;

		/** Marked assignment operator as delete. Cannot copy Sectors.
		*/
		Sector& operator=(const Sector&) = delete;
		
		/** Returns the name of the Sector.
		*	@returns Returns the name of the Sector.
		*/
		std::string& Name();

		/** Returns the name of the Sector.
		*	@returns Returns the name of the Sector.
		*/
		const std::string& Name() const;

		/** Sets the name of the Sector.
		*	@param name The new name of the Sector.
		*/
		void SetName(const std::string& name);

		/** Returns the Datum containing the Entities.
		*	@returns Returns the Datum containing the Entities.
		*/
		Datum& Entities();

		/** Returns the Datum containing the Entities.
		*	@returns Returns the Datum containing the Entities.
		*/
		const Datum& Entities() const;

		/** Creates a new Entity inside the Sector and returns it.
		*	@param entityClassName The Entity class to be created
		*	@param instanceName The name of the Entity to be created.
		*	@returns Returns the newly created Entity.
		*/
		Entity& CreateEntity(const std::string& entityClassName, const std::string& instanceName);
		
		/** Returns the reference to the World in which the Sector is present.
		*	@returns Returns the reference to the world in which the Sector is present.
		*/
		World& GetWorld();

		/** Returns the reference to the World in which the Sector is present.
		*	@returns Returns the reference to the world in which the Sector is present.
		*/
		const World& GetWorld() const;
		
		/** Adopts the passed Entity into the current Sector.
		*	@param entity The Entity to be adopted to the current Sector.
		*/
		void AdoptEntity(Entity& entity);
		
		/** Updates all the Entities contained in the Sector.
		*	@param WorldState Represents the state of the World. While in this function, updates the Sector* to this of the WorldState..
		*/
		void Update(WorldState& worldState);

		/** Defaulted Sector's destructor.
		*/
		~Sector() = default;
	private:
		/** Initializes the signatures(member variables) of the Sector.
		*/
		void InitializeSignatures();

		/** The name of the Sector.
		*/
		std::string mName;

		/** Cached datum containing Entities.
		*/
		Datum* mEntityDatum;		

		/** The name aside which the Entities are added to the scope.
		*/
		static const std::string sEntitiesName;

		/** The attribute name for the name of the Sector.
		*/
		static const std::string sAttributeName;
	public:
		RTTI_DECLARATIONS(Sector, Attributed);
	};
}
