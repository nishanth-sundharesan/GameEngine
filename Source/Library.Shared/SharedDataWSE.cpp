#include "Pch.h"
#include "SharedDataWSE.h"

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(SharedDataWSE);

	SharedDataWSE::SharedDataWSE()
		:mWorld(nullptr), mSector(nullptr), mEntity(nullptr), mCurrentScope(nullptr)
	{
	}

	void SharedDataWSE::Initialize()
	{
		SharedData::Initialize();

		delete mWorld;
		mWorld = nullptr;
		mSector = nullptr;
		mEntity = nullptr;
		mSector = nullptr;
	}

	XmlParseMaster::SharedData* SharedDataWSE::Clone() const
	{
		SharedData* newSharedData = new SharedDataWSE();
		CloneInternalMembers(newSharedData);
		return newSharedData;		
	}

	void SharedDataWSE::CreateWorld(const string& name)
	{
		if (mWorld != nullptr)
		{
			throw exception("Multiple Worlds found.");
		}
		mWorld = new World(name);
		mCurrentScope = mWorld;
	}

	World* SharedDataWSE::GetWorld()
	{
		return const_cast<World*>(const_cast<const SharedDataWSE*>(this)->GetWorld());
	}

	const World* SharedDataWSE::GetWorld() const
	{
		return mWorld;
	}

	void SharedDataWSE::CreateSector(const string& name)
	{
		if (mWorld == nullptr)
		{
			throw exception("A Sector has to be present inside the World.");
		}
		mSector = &(mWorld->CreateSector(name));
		mCurrentScope = mSector;
	}

	void SharedDataWSE::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		if (mSector == nullptr)
		{
			throw exception("An Entity has to be present inside the Sector.");
		}
		mEntity = &(mSector->CreateEntity(className, instanceName));
		mCurrentScope = mEntity;
	}

	void SharedDataWSE::AppendPrimitiveData(const string& name, const DatumType datumType, const string& value)
	{
		if (mCurrentScope == nullptr)
		{
			throw exception("Primitives has to be present in a scope.");
		}
		Datum& datum = mCurrentScope->Append(name);
		datum.SetType(datumType);
		datum.PushFromString(value);
	}

	void SharedDataWSE::GotoParentScope()
	{
		if (mCurrentScope == nullptr)
		{
			throw exception("Invalid Xml.");
		}
		mCurrentScope = mCurrentScope->GetParent();
	}

	SharedDataWSE::~SharedDataWSE()
	{
		delete mWorld;
		mWorld = nullptr;
	}

	void SharedDataWSE::CloneInternalMembers(SharedData* sharedData) const
	{
		SharedData::CloneInternalMembers(sharedData);
	}
}