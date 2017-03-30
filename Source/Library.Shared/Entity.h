#pragma once
#include "Attributed.h"
#include "WorldState.h"

namespace GameEngineLibrary
{
	class Sector;

	class Entity :public Attributed
	{
	public:
		//TODO Can be made const std::string&
		std::string Name();

		void SetName(const std::string& name);
		
		Sector& GetSector();

		const Sector& GetSector() const;

		void SetSector(const Sector& sector);		

		//TODO accept the parameter  WorldState reference
		void Update(WorldState& worldState);

		virtual ~Entity() = default;
	private:
		std::string mName;

		Sector* mAssociatedSector;
	};
}