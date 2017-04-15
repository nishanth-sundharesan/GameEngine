#pragma once
#include "Event.h"
#include "ActionList.h"
#include "EventMessage.h"

namespace GameEngineLibrary
{
	/** Abstract Reaction base class.
	*/
	class Reaction :public EventSubscriber, public ActionList
	{
	public:
		/** Defaulted constructor.
		*/
		Reaction() = default;

		/** Marked copy constructor as delete. Cannot copy Reactions.
		*/
		Reaction(const Reaction&) = delete;

		/** Marked assignment operator as delete. Cannot copy Reactions.
		*/
		Reaction& operator=(const Reaction&) = delete;

		/** Defaulted virtual destructor.
		*/
		virtual ~Reaction() = default;		
	public:
		RTTI_DECLARATIONS(Reaction, ActionList);
	};	
}

#define ReactionFactory(ConcreteReaction) ConcreteFactory(GameEngineLibrary::Action, ConcreteReaction)