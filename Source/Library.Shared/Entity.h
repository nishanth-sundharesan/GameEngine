#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	class Sector;

	class Entity :public Attributed
	{
	public:
		Entity();

		Entity(const Entity&) = delete;

		Entity& operator=(const Entity&) = delete;

		std::string& Name();

		const std::string& Name() const;

		void SetName(const std::string& name);

		Sector& GetSector();

		const Sector& GetSector() const;

		void Update(WorldState& worldState);

		virtual ~Entity() = default;

	protected:
		virtual void InitializeSignatures();

	private:
		std::string mName;

	public:
		RTTI_DECLARATIONS(Entity, Attributed);
	};
}