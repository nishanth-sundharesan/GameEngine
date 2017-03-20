#include "Pch.h"
#include "SharedDataFoo.h"

RTTI_DEFINITIONS(UnitTestSupportClasses::SharedDataFoo);

namespace UnitTestSupportClasses
{
	XmlParseMaster::SharedData* SharedDataFoo::Clone() const
	{		
		SharedData* newSharedData = new SharedDataFoo();
		newSharedData->SetXmlParseMaster(const_cast<XmlParseMaster*>(GetXmlParseMaster()));
		return newSharedData;
	}
}