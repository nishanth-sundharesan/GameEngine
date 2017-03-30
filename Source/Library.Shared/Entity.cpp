#include "Pch.h"
#include "Entity.h"
#include "Sector.h"

using namespace std;

namespace GameEngineLibrary
{
	string Entity::Name()
	{
		return string();
	}

	void Entity::SetName(const string& name)
	{
		name;
	}

	Sector& Entity::GetSector()
	{
		return *mAssociatedSector;
	}

	const Sector& Entity::GetSector() const
	{
		return *mAssociatedSector;
	}

	void Entity::SetSector(const Sector& sector)
	{
		//Read assignment before implementing
		sector;
	}

	void Entity::Update(WorldState& worldState)
	{
		//Read assignment before implementing
		worldState;
	}
}