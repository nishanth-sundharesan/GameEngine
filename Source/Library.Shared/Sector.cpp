#include "Pch.h"
#include "Sector.h"
#include "World.h"
#include "Factory.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Sector);

	const string Sector::sEntitiesName = "Entities";
	const string Sector::sAttributeName = "Name";

	Sector::Sector(const std::string& name)
		:mName(name), mEntityDatum(nullptr)
	{
		InitializeSignatures();
	}

	string& Sector::Name()
	{
		return const_cast<string&>(const_cast<const Sector*>(this)->Name());
	}

	const string& Sector::Name() const
	{
		return mName;
	}

	void Sector::SetName(const string& name)
	{
		mName = name;
	}

	Datum& Sector::Entities()
	{
		return const_cast<Datum&>(const_cast<const Sector*>(this)->Entities());
	}

	const Datum& Sector::Entities() const
	{		
		return *mEntityDatum;
	}

	Entity& Sector::CreateEntity(const string& entityClassName, const string& entityInstanceName)
	{
		Entity* entity = Factory<Entity>::Create(entityClassName);
		entity->SetName(entityInstanceName);
		AddNestedScopeAttribute(*entity, sEntitiesName);
		return *entity;
	}

	World& Sector::GetWorld()
	{
		return const_cast<World&>(const_cast<const Sector*>(this)->GetWorld());
	}

	const World& Sector::GetWorld() const
	{
		assert(mParentScope->Is("World"));
		return *(static_cast<const World*>(mParentScope));
	}

	void Sector::AdoptEntity(Entity& entity)
	{
		AddNestedScopeAttribute(entity, sEntitiesName);
	}

	void Sector::Update(WorldState& worldState)
	{
		worldState.SetCurrentSector(this);
		assert(mEntityDatum != nullptr);

		for (uint32_t i = 0; i < mEntityDatum->Size(); ++i)
		{
			assert(static_cast<Entity*>(mEntityDatum->Get<Scope*>(i))->As<Entity>() != nullptr);
			(static_cast<Entity*>(mEntityDatum->Get<Scope*>(i)))->Update(worldState);
		}
		worldState.SetCurrentSector(nullptr);
	}

	void Sector::InitializeSignatures()
	{
		AddExternalAttribute(sAttributeName, &mName, 1);
		mEntityDatum = &AddEmptyNestedScopeAttribute(sEntitiesName);
	}
}