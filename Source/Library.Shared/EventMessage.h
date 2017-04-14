#pragma once
#include "Attributed.h"

namespace GameEngineLibrary
{
	class World;

	class WorldState;

	class EventMessage :public Attributed
	{
	public:		
		EventMessage(std::int32_t subType, World& world, WorldState& worldState);

		EventMessage(const EventMessage&) = default;

		EventMessage& operator=(const EventMessage&) = default;

		std::int32_t GetSubType() const;

		void SetSubType(const std::int32_t& subType);

		void SetWorld(World& world);

		World& GetWorld();

		const World& GetWorld() const;

		WorldState& GetWorldState();

		const WorldState& GetWorldState() const;

		virtual ~EventMessage() = default;
	private:
		std::int32_t mSubType;

		World* mWorld;

		WorldState* mWorldState;
	};
}