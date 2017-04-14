#include "Pch.h"
#include "EventMessage.h"
#include "World.h"

using namespace std;

namespace GameEngineLibrary
{
	EventMessage::EventMessage(int32_t subType, World& world, WorldState& worldState)
		:mSubType(subType), mWorld(&world), mWorldState(&worldState)
	{
	}

	int32_t EventMessage::GetSubType() const
	{
		return mSubType;
	}

	void EventMessage::SetSubType(const int32_t& subType)
	{
		mSubType = subType;
	}

	void EventMessage::SetWorld(World& world)
	{
		mWorld = &world;
	}

	World& EventMessage::GetWorld()
	{
		return const_cast<World&>(const_cast<const EventMessage*>(this)->GetWorld());
	}

	const World& EventMessage::GetWorld() const
	{
		return *mWorld;
	}

	WorldState& EventMessage::GetWorldState()
	{
		return const_cast<WorldState&>(const_cast<const EventMessage*>(this)->GetWorldState());
	}

	const WorldState& EventMessage::GetWorldState() const
	{
		return *mWorldState;
	}
}