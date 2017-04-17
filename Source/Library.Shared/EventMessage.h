#pragma once
#include "Attributed.h"

namespace GameEngineLibrary
{
	/** Forward declared World class.
	*/
	class World;

	/** Forward declared WorldState class.
	*/
	class WorldState;

	/** EventMessage class which acts as a payload between ActionEvents and ReactionAttributed.
	*/
	class EventMessage :public Attributed
	{
	public:
		/** Parameterized constructor. Caches the subType and game engine's WorldState object.
		*	@param subType The subType filter of the event.
		*	@param worldState The WorldState object of the game engine.
		*/
		EventMessage(const std::string& subType, WorldState& worldState);

		/** Defaulted copy constructor.
		*/
		EventMessage(const EventMessage&) = default;

		/** Defaulted assignment operator.
		*/
		EventMessage& operator=(const EventMessage&) = default;

		/** Returns the subType of the EventMessage.
		*	@returns Returns the subType of the EventMessage.
		*/
		const std::string& GetSubType() const;

		/** Returns the cached WorldState object.
		*	@returns Returns the cached WorldState object.
		*/
		WorldState& GetWorldState();

		/** Returns the cached WorldState object.
		*	@returns Returns the cached WorldState object.
		*/
		const WorldState& GetWorldState() const;

		/** Defaulted virtual destructor.
		*/
		virtual ~EventMessage() = default;
	protected:
		/** Initializes the signatures(member variables) of the Entity.
		*/
		virtual void InitializeSignatures();
	private:
		/** Cached subType of the event.
		*/
		std::string mSubType;

		/** Cached WorldState object.
		*/
		WorldState* mWorldState;
	};
}