#include "Pch.h"
#include "EventMessage.h"
#include "World.h"

using namespace std;

namespace GameEngineLibrary
{
	EventMessage::EventMessage(const string& subType, WorldState& worldState)
		:mSubType(subType), mWorldState(&worldState)
	{
		InitializeSignatures();
	}

	const string& EventMessage::GetSubType() const
	{
		return mSubType;
	}
	
	WorldState& EventMessage::GetWorldState()
	{
		return const_cast<WorldState&>(const_cast<const EventMessage*>(this)->GetWorldState());
	}

	const WorldState& EventMessage::GetWorldState() const
	{
		return *mWorldState;
	}

	void EventMessage::InitializeSignatures()
	{
		Attributed::InitializeSignatures();
	}
}