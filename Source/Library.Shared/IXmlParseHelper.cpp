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

	bool IXmlParseHelper::DoesContainData(const string& data, const int32_t length)
	{
		for (int32_t i = 0; i < length; ++i)
		{
			if (data[i] != '\t')
			{
				return true;
			}
		}
		return false;
	}
}