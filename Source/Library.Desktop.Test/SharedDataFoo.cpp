#include "Pch.h"
#include "SharedDataFoo.h"

RTTI_DEFINITIONS(UnitTestSupportClasses::SharedDataFoo);

namespace UnitTestSupportClasses
{
	SharedDataFoo::SharedDataFoo()
		:mReadCharacterData("")
	{
	}

	XmlParseMaster::SharedData* SharedDataFoo::Clone() const
	{		
		SharedData* newSharedData = new SharedDataFoo();
		CloneInternalMembers(newSharedData);
		return newSharedData;
	}

	void SharedDataFoo::Initialize()
	{
		SharedData::Initialize();
		mReadCharacterData = "";
		mKeyValuePairs.Clear();		
	}
}