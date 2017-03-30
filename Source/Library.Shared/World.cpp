#include "Pch.h"
#include "World.h"

using namespace std;

namespace GameEngineLibrary
{
	string World::Name()
	{
		return string();
	}

	void World::SetName(const string& name)
	{
		name;
	}

	Datum& World::Sectors()
	{
		return tempDatum;
	}

	const Datum& World::Sectors() const
	{
		return tempDatum;
	}

	Sector& World::CreateSector(const string& instanceName)
	{
		instanceName;
		return tempSector;
	}

	void World::Update()
	{
	}
}