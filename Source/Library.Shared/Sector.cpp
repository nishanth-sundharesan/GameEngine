#include "Pch.h"
#include "Sector.h"
#include "World.h"

using namespace std;

namespace GameEngineLibrary
{
	string Sector::Name()
	{
		return string();
	}

	void Sector::SetName(const string& name)
	{
		name;
	}

	Datum& Sector::Entities()
	{
		return tempDatum;
	}
	const Datum& Sector::Entities() const
	{
		return tempDatum;
	}

	Entity& Sector::CreateEntity(const string& entityClassName, const string& instanceName)
	{
		entityClassName;
		instanceName;
		return entity;
	}

	World& Sector::GetWorld()
	{
		//Read assignment before implementing it
		return *mAssociatedWorld;
	}

	const World& Sector::GetWorld() const
	{
		// TODO: insert return statement here
		return *mAssociatedWorld;
	}

	void Sector::SetWorld(const World& world)
	{
		// TODO: insert return statement here
		world;
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState;
	}
}