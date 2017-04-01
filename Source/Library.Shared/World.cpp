#include "Pch.h"
#include "World.h"
#include <assert.h>

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(World);

	const string World::sSectorsName = "Sectors";
	const string World::sAttributeName = "WorldName";

	World::World(const string& name)
		:mName(name), mSectorDatum(nullptr)
	{
		InitializeSignatures();
	}

	string& World::Name()
	{
		return const_cast<string&>(const_cast<const World*>(this)->Name());
	}

	const string& World::Name() const
	{
		return mName;
	}

	void World::SetName(const string& name)
	{
		mName = name;
	}

	Datum& World::Sectors()
	{
		return const_cast<Datum&>(const_cast<const World*>(this)->Sectors());
	}

	const Datum& World::Sectors() const
	{		
		return *mSectorDatum;
	}

	void World::AdoptSector(Sector& sector)
	{
		AddNestedScopeAttribute(sector, sSectorsName);
	}

	Sector& World::CreateSector(const string& name)
	{
		Sector* sector = new Sector(name);
		AddNestedScopeAttribute(*sector, sSectorsName);		
		return *sector;
	}

	void World::Update(WorldState& worldState)
	{
		worldState.SetCurrentWorld(this);

		assert(mSectorDatum != nullptr);
		for (uint32_t i = 0; i < mSectorDatum->Size(); ++i)
		{
			assert(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i))->Is(Sector::TypeIdClass()));
			(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i)))->Update(worldState);
		}
		worldState.SetCurrentWorld(nullptr);
	}

	void World::InitializeSignatures()
	{
		AddExternalAttribute(sAttributeName, &mName, 1);
		mSectorDatum = &AddEmptyNestedScopeAttribute(sSectorsName);
	}
}