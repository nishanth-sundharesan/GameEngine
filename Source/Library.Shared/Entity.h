#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	/** Forward declared Sector class.
	*/
	class Sector;

	/** Entity class containing actions to be performed.
	*/
	class Entity :public Attributed
	{
	public:
		/** Default constructor. Initializes private members of the class.
		*/
		Entity();

		/** Marked copy constructor as delete. Cannot copy Entities.
		*/
		Entity(const Entity&) = delete;

		/** Marked assignment operator as delete. Cannot copy Entities.
		*/
		Entity& operator=(const Entity&) = delete;

		/** Returns the name of the Entity.
		*	@returns Returns the name of the Entity.
		*/
		std::string& Name();

		/** Returns the name of the Entity.
		*	@returns Returns the name of the Entity.
		*/
		const std::string& Name() const;

		/** Sets the name of the Entity.
		*	@param name The new name of the Entity.
		*/
		void SetName(const std::string& name);

		/** Returns the reference to the Sector in which the Entity is present.
		*	@returns Returns the reference to the Sector in which the Entity is present.
		*/
		Sector& GetSector();

		/** Returns the reference to the Sector in which the Entity is present.
		*	@returns Returns the reference to the Sector in which the Entity is present.
		*/
		const Sector& GetSector() const;

		/** Updates all the Actions contained in the Entity.
		*	@param WorldState Represents the state of the World. While in this function, updates the Entity* to this of the WorldState.
		*/
		virtual void Update(WorldState& worldState);

		/** Defaulted Entity's destructor.
		*/
		virtual ~Entity() = default;
	protected:
		/** Initializes the signatures(member variables) of the Entity.
		*/
		virtual void InitializeSignatures();
	private:
		/** The name of the Entity.
		*/
		std::string mName;
	public:
		RTTI_DECLARATIONS(Entity, Attributed);
	};
}