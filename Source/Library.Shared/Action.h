#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "Factory.h"

namespace GameEngineLibrary
{
	/** Base class for all the actions to be performed in the GameEngine.
	*/
	class Action :public Attributed
	{
	public:
		/** Default constructor. Initializes the private members of the class.
		*/
		Action();

		/** Marked copy constructor as delete. Cannot copy Actions.
		*/
		Action(const Action&) = delete;

		/** Marked assignment operator as delete. Cannot copy Actions.
		*/
		Action& operator=(const Action&) = delete;

		/** Updates the overriden method of the derived class.
		*	@param WorldState Represents the state of the World. While in this function, the derived classes updates the Action* to this of the WorldState.
		*/
		virtual void Update(WorldState& worldState) = 0;

		/** Sets the name of the Action.
		*	@param name The name of the action to be set.
		*/
		void SetName(const std::string& name);

		/** Returns the name of the Action.
		*	@returns Returns the name of the Action.
		*/
		std::string& Name();

		/** Returns the name of the Action.
		*	@returns Returns the name of the Action.
		*/
		const std::string& Name() const;

		/** Defaulted destructor.
		*/
		virtual ~Action() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures();

		/** The attribute name of the Action.
		*/
		static const std::string sActionsName;
	private:
		/** The name of the Action.
		*/
		std::string mName;

		/** The Attribute name for the attributes.
		*/
		static const std::string sAttributeName;
	public:
		RTTI_DECLARATIONS(Action, Attributed);
	};
}

#define ActionFactory(ConcreteAction) ConcreteFactory(GameEngineLibrary::Action, ConcreteAction)				