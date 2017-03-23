#include "Pch.h"
#include "IXmlParseHelper.h"

namespace GameEngineLibrary
{
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