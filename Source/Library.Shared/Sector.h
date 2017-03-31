#pragma once
#include "Entity.h"

namespace GameEngineLibrary
{
	class World;

	class Sector final :public Attributed
	{
	public:
		Sector(const std::string& name);

		Sector(const Sector&) = delete;

		Sector& operator=(const Sector&) = delete;
		
		std::string& Name();

		const std::string& Name() const;

		void SetName(const std::string& name);

		Datum& Entities();

		const Datum& Entities() const;

		Entity& CreateEntity(const std::string& entityClassName, const std::string& instanceName);
		
		World& GetWorld();

		const World& GetWorld() const;
		
		void AdoptEntity(Entity& entity);
		
		void Update(WorldState& worldState);

	private:
		void InitializeSignatures();

		std::string mName;

		Datum* mEntityDatum;		

		static const std::string mEntitiesName;

	public:
		RTTI_DECLARATIONS(Sector, Attributed);
	};
}
