#pragma once
#include "Sector.h"

namespace GameEngineLibrary
{
	class World final :public Attributed
	{
	public:		
		World(const std::string& name);

		World(const World&) = delete;

		World& operator=(const World&) = delete;

		std::string& Name();

		const std::string& Name() const;

		void SetName(const std::string& name);

		Sector& CreateSector(const std::string& instanceName);

		Datum& Sectors();

		const Datum& Sectors() const;

		void AdoptSector(Sector& sector);		

		void Update(WorldState& worldState);

	private:
		void InitializeSignatures();

		std::string mName;

		Datum* mSectorDatum;

		static const std::string mSectorsName;
	public:
		RTTI_DECLARATIONS(World, Attributed);
	};
}