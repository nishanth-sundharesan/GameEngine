#include "Pch.h"
#include "Sector.h"
#include "World.h"
#include "Factory.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Sector);

	const string Sector::mEntitiesName = "Entities";

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

	Entity& Sector::CreateEntity(const string& entityClassName, const string& instanceName)
	{
		Entity* entity = Factory<Entity>::Create(entityClassName);
		entity->SetName(instanceName);		

		AddNestedScopeAttribute(*entity, mEntitiesName);	
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
		AddNestedScopeAttribute(entity, mEntitiesName);
	}

	void Sector::Update(WorldState& worldState)
	{
		assert(mEntityDatum != nullptr);
		for (uint32_t i = 0; i < mEntityDatum->Size(); ++i)
		{
			assert(static_cast<Entity*>(mEntityDatum->Get<Scope*>(i))->As<Entity>() != nullptr);
			(static_cast<Entity*>(mEntityDatum->Get<Scope*>(i)))->Update(worldState);
		}
	}

	void Sector::InitializeSignatures()
	{
		AddExternalAttribute("SectorName", &mName, 1);
		mEntityDatum = &AddEmptyNestedScopeAttribute(mEntitiesName);
	}
}