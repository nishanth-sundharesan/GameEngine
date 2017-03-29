#include "Pch.h"
#include "IXmlParseHelper.h"

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(IXmlParseHelper);

	IXmlParseHelper::IXmlParseHelper(XmlParseMaster& xmlParseMaster)
		:mXmlParseMaster(&xmlParseMaster)
	{
		mXmlParseMaster->AddHelper(*this);
	}

	IXmlParseHelper::~IXmlParseHelper()
	{
		mXmlParseMaster->RemoveHelper(*this);
	}
}