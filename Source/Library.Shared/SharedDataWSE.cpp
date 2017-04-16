#include "Pch.h"
#include "SharedDataWSE.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"

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
		if (mWorld == nullptr || mCurrentScope != mWorld)
		{
			throw exception("A Sector has to be present inside the World.");
		}
		mSector = &(mWorld->CreateSector(name));
		mCurrentScope = mSector;
	}

	void SharedDataWSE::CreateEntity(const string& className, const string& instanceName)
	{
		if (mSector == nullptr || mCurrentScope != mSector)
		{
			throw exception("An Entity has to be present inside the Sector.");
		}
		mEntity = &(mSector->CreateEntity(className, instanceName));
		mCurrentScope = mEntity;
	}

	void SharedDataWSE::CreateActionList(const string& name)
	{
		if (mEntity == nullptr || mCurrentScope != mEntity)
		{
			throw exception("ActionList has to be present inside the Entity.");
		}
		mActionList = &(mEntity->CreateActionList(name));
		mCurrentScope = mActionList;
	}

	void SharedDataWSE::CreateAction(const string& className, const string& instanceName)
	{
		if (mCurrentScope != nullptr)
		{
			if (mCurrentScope == mActionList)
			{
				mCurrentScope = &(mActionList->CreateAction(className, instanceName));
			}
			else if (mCurrentScope == mEntity)
			{
				mCurrentScope = &(mEntity->CreateAction(className, instanceName));
			}
			else
			{
				throw exception("An Action has to be present inside an Entity or an ActionList.");
			}
		}
		else
		{
			throw exception("An Action has to be present inside an Entity or an ActionList.");
		}
	}

	void SharedDataWSE::CreateReaction(const string& className, const string& instanceName, const string& subType)
	{
		if (mEntity == nullptr || mCurrentScope != mEntity)
		{
			throw exception("Reaction has to be present inside the Entity.");
		}
		mActionList = static_cast<ActionList*>(&mEntity->CreateAction(className, instanceName));
		static_cast<ReactionAttributed*>(mActionList)->SetSubType(mWorld->SearchDatum(subType));
		mCurrentScope = mActionList;
	}

	void SharedDataWSE::CreateActionEvent(const string& className, const string& instanceName, const string& subType, const string& delay)
	{
		CreateAction(className, instanceName);
		static_cast<ActionEvent*>(mCurrentScope)->SetSubTypeAndDelay(mWorld->SearchDatum(subType), delay);
	}

	void SharedDataWSE::AddThenAction(const string& className, const string& instanceName) const
	{
		assert(mCurrentScope != nullptr && mCurrentScope == mActionListIf);									//Exception test has already been performed in XmlParseHelper
		mActionListIf->SetThenBlockAction(mActionListIf->CreateAction(className, instanceName));
	}

	void SharedDataWSE::AddElseAction(const string& className, const string& instanceName) const
	{
		assert(mCurrentScope != nullptr && mCurrentScope == mActionListIf);									//Exception test has already been performed in XmlParseHelper
		mActionListIf->SetElseBlockAction(mActionListIf->CreateAction(className, instanceName));
	}

	void SharedDataWSE::AddActionCreateAction(const string& className, const string& instanceName, const string& actionClassName, const string& actionInstanceName) const
	{
		if (mCurrentScope != nullptr)
		{
			if (mCurrentScope == mActionList)
			{
				static_cast<ActionCreateAction&>(mActionList->CreateAction(className, instanceName)).SetClassAndInstanceName(actionClassName, actionInstanceName);
			}
			else if (mCurrentScope == mEntity)
			{
				static_cast<ActionCreateAction&>(mEntity->CreateAction(className, instanceName)).SetClassAndInstanceName(actionClassName, actionInstanceName);
			}
			else
			{
				throw exception("ActionCreateAction has to be present inside an Entity or an ActionList.");
			}
		}
		else
		{
			throw exception("ActionCreateAction has to be present inside an Entity or an ActionList.");
		}
	}

	void SharedDataWSE::AddActionDeleteAction(const string& className, const string& instanceName, const string& actionInstanceName) const
	{
		if (mCurrentScope != nullptr)
		{
			if (mCurrentScope == mActionList)
			{
				static_cast<ActionDeleteAction&>(mActionList->CreateAction(className, instanceName)).SetActionToDelete(actionInstanceName);
			}
			else if (mCurrentScope == mEntity)
			{
				static_cast<ActionDeleteAction&>(mEntity->CreateAction(className, instanceName)).SetActionToDelete(actionInstanceName);
			}
			else
			{
				throw exception("ActionDeleteAction has to be present inside an Entity or an ActionList.");
			}
		}
		else
		{
			throw exception("ActionDeleteAction has to be present inside an Entity or an ActionList.");
		}
	}

	void SharedDataWSE::CreateActionIf(const string& className, const string& instanceName, const string& lhsComparisonPathValue, const string& rhsComparisonPathValue)
	{
		if (mCurrentScope != nullptr)
		{
			if (mCurrentScope == mActionList)
			{
				mCurrentScope = &(mActionList->CreateAction(className, instanceName));
			}
			else if (mCurrentScope == mEntity)
			{
				mCurrentScope = &(mEntity->CreateAction(className, instanceName));
			}
			else
			{
				throw exception("An If has to be present inside an Entity or an ActionList.");
			}
			assert(static_cast<ActionListIf*>(mCurrentScope) != nullptr);
			mActionListIf = static_cast<ActionListIf*>(mCurrentScope);

			mLhsComparisonPathValue = lhsComparisonPathValue;
			mRhsComparisonPathValue = rhsComparisonPathValue;
		}
		else
		{
			throw exception("An If has to be present inside an Entity or an ActionList.");
		}
	}

	void SharedDataWSE::AssignComparisonsForIf() const
	{
		assert(mActionListIf != nullptr);
		mActionListIf->SetComparisons(mWorld->SearchDatum(mLhsComparisonPathValue), mWorld->SearchDatum(mRhsComparisonPathValue));
	}

	void SharedDataWSE::AppendPrimitiveData(const string& name, const DatumType datumType, const string& value) const
	{
		if (mCurrentScope == nullptr)
		{
			throw exception("Primitives has to be present in a scope.");
		}

		Datum& datum = static_cast<Attributed*>(mCurrentScope)->AppendAuxiliaryAttribute(name);
		datum.SetType(datumType);
		datum.PushFromString(value);
	}

	void SharedDataWSE::GotoParentScope()
	{
		assert(mCurrentScope != nullptr);		
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