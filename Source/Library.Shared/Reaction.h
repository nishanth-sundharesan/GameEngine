#pragma once
#include "Event.h"
#include "ActionList.h"
#include "EventMessage.h"

namespace GameEngineLibrary
{
	class Reaction :public EventSubscriber, public ActionList
	{
	public:
		Reaction() = default;

		Reaction(const Reaction&) = delete;

		Reaction& operator=(const Reaction&) = delete;

		virtual ~Reaction() = default;
	
		RTTI_DECLARATIONS(Reaction, ActionList);
	};

	RTTI_DEFINITIONS(Reaction);
}

#define ReactionFactory(ConcreteReaction) ConcreteFactory(GameEngineLibrary::Reaction, ConcreteReaction)