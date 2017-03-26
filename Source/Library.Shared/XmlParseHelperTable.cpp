#include "Pch.h"
#include "XmlParseHelperTable.h"
#include "SharedDataTable.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(XmlParseHelperTable);

	const string XmlParseHelperTable::mXmlElementScopeName = "scope";
	const string XmlParseHelperTable::mXmlElementAttributeKeyName = "name";

	XmlParseHelperTable::XmlParseHelperTable(XmlParseMaster& xmlParseMaster)
		:IXmlParseHelper(xmlParseMaster)
	{
	}

	void XmlParseHelperTable::Initialize()
	{
	}

	bool XmlParseHelperTable::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || name != mXmlElementScopeName)
		{
			return false;
		}
		if (!attributes.ContainsKey(mXmlElementAttributeKeyName))
		{
			throw exception("The Attribute 'name' is not present in the Scope XML element.");
		}

		sharedDataTable->AppendNewScope(attributes[mXmlElementAttributeKeyName]);
		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(SharedData& sharedData, const string& name)
	{
		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || name != mXmlElementScopeName)
		{
			return false;
		}
		sharedDataTable->GotoParentScope();
		return true;
	}

	void XmlParseHelperTable::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		throw exception("XmlParseHelperTable cannot handle any data inside the Scope XML element.");
		sharedData;
		value;
		length;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone() const
	{
		return new XmlParseHelperTable(*mXmlParseMaster);
	}
}