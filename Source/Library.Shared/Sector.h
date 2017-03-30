#pragma once
#include "Entity.h"

namespace GameEngineLibrary
{
	class World;

	class Sector final :public Attributed
	{
	public:
		//TODO Can be made const std::string&
		std::string Name();

		void SetName(const std::string& name);

		Datum& Entities();

		const Datum& Entities() const;

		Entity& CreateEntity(const std::string& entityClassName, const std::string& instanceName);
		
		World& GetWorld();

		const World& GetWorld() const;
		
		void SetWorld(const World& world);

		//TODO accept the parameter  WorldState reference
		void Update(WorldState& worldState);

	private:
		std::string mName;

		Datum tempDatum;

		Entity entity;

		World* mAssociatedWorld;
	};
}
