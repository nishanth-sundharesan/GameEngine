#include "Pch.h"
#include "World.h"
#include <assert.h>

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(World);

	const string World::mSectorsName = "Sectors";

	World::World(const string& name)
		:mName(name), mSectorDatum(nullptr)
	{
		mName = name;
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
		AddNestedScopeAttribute(sector, mSectorsName);
	}

	Sector& World::CreateSector(const string& instanceName)
	{
		Sector* sector = new Sector(instanceName);
		AddNestedScopeAttribute(*sector, mSectorsName);		
		return *sector;
	}

	void World::Update(WorldState& worldState)
	{
		worldState.SetCurrentWorld(*this);

		assert(mSectorDatum != nullptr);
		for (uint32_t i = 0; i < mSectorDatum->Size(); ++i)
		{
			assert(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i))->Is(Sector::TypeIdClass()));
			(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i)))->Update(worldState);
		}
	}

	void World::InitializeSignatures()
	{
		AddExternalAttribute("WorldName", &mName, 1);
		mSectorDatum = &AddEmptyNestedScopeAttribute(mSectorsName);
	}
}