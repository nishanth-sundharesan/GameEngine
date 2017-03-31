#include "Pch.h"
#include "Entity.h"
#include "Sector.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Entity);

	Entity::Entity()
	{
		InitializeSignatures();
	}

	string& Entity::Name()
	{
		return const_cast<string&>(const_cast<const Entity*>(this)->Name());
	}

	const string& Entity::Name() const
	{
		return mName;
	}

	void Entity::SetName(const string& name)
	{
		mName = name;
	}

	Sector& Entity::GetSector()
	{
		return const_cast<Sector&>(const_cast<const Entity*>(this)->GetSector());
	}

	const Sector& Entity::GetSector() const
	{
		assert(mParentScope->Is("Sector"));
		return *(static_cast<const Sector*>(mParentScope));
	}

	void Entity::Update(WorldState& worldState)
	{		
		worldState;
	}

	void Entity::InitializeSignatures()
	{
		AddExternalAttribute("EntityName", &mName, 1);
	}
}