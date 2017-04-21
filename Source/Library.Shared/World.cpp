#include "Pch.h"
#include "World.h"
#include <assert.h>
#include <string.h>

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(World);

	const string World::sSectorsName = "Sectors";
	const string World::sAttributeName = "Name";
	const char* World::sDelimiterString = ".";

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

	Datum& World::SearchDatum(const string& path)
	{
		return const_cast<Datum&>(const_cast<const World*>(this)->SearchDatum(path));		
	}

	const Datum& World::SearchDatum(const string& path) const
	{
		const char *name = FindNextDataChunk(const_cast<char*>(path.c_str()));													//Fetching the World's name

		Scope* scopeToSearch = const_cast<World*>(this);
		Datum* primitiveDatum = nullptr;
		Datum* tableDatum = nullptr;
		uint32_t cachedIndex = 0;

		while (true)
		{
			primitiveDatum = scopeToSearch->Find(sAttributeName);
			if (primitiveDatum != nullptr && primitiveDatum->Get<string>() == name)
			{
				name = FindNextDataChunk();
				if (scopeToSearch->Find(name) != nullptr)
				{
					primitiveDatum = scopeToSearch->Find(name);
					break;
				}

				//Proceed to Child Table* inside the Scope.
				for (uint32_t i = 0; i < scopeToSearch->Size(); ++i)
				{
					if ((*scopeToSearch)[i].Type() == DatumType::TABLE)
					{
						tableDatum = &(*scopeToSearch)[i];
						cachedIndex = 0;

						if (tableDatum->Size() == 0)
						{
							throw exception("Path to an Empty Table.");
						}
						else
						{
							scopeToSearch = tableDatum->Get<Scope*>(cachedIndex);
							break;
						}
					}
				}
			}
			else
			{
				//Proceed to next Table* inside the Datum.
				cachedIndex++;
				if (cachedIndex < tableDatum->Size())
				{
					scopeToSearch = tableDatum->Get<Scope*>(cachedIndex);
				}
				else
				{
					throw exception("Invalid Path");
				}
			}
		}
		return *primitiveDatum;
	}

	void World::AddActionToDelete(Action& actionToDelete)
	{
		lock_guard<mutex> lock(mMutex);
		mActionsToDelete.PushBack(&actionToDelete);
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

		//Updating the Event Queue
		mEventQueue.Update(worldState.GetGameTime());

		assert(mSectorDatum != nullptr);
		for (uint32_t i = 0; i < mSectorDatum->Size(); ++i)
		{
			assert(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i))->Is(Sector::TypeIdClass()));
			(static_cast<Sector*>(mSectorDatum->Get<Scope*>(i)))->Update(worldState);
		}

		for (auto& actionPointer : mActionsToDelete)
		{
			Orphan(actionPointer);
			delete actionPointer;
		}
		mActionsToDelete.Clear();
		worldState.SetCurrentWorld(nullptr);
	}

	EventQueue& World::GetEventQueue()
	{
		return const_cast<EventQueue&>(const_cast<const World*>(this)->GetEventQueue());
	}

	const EventQueue& World::GetEventQueue() const
	{
		return mEventQueue;
	}

	const char* World::FindNextDataChunk(char* path) const
	{
		const char* nextChunk;
		if (path == nullptr)
		{
#pragma warning(suppress: 4996)
			nextChunk = strtok(nullptr, sDelimiterString);
		}
		else
		{
#pragma warning(suppress: 4996)
			nextChunk = strtok(path, sDelimiterString);
		}
		if (nextChunk == nullptr)
		{
			throw exception("Invalid Path");
		}
		return nextChunk;
	}

	void World::InitializeSignatures()
	{
		Attributed::InitializeSignatures();
		AddExternalAttribute(sAttributeName, &mName, 1);
		mSectorDatum = &AddEmptyNestedScopeAttribute(sSectorsName);
	}
}