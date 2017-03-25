#include "Pch.h"
#include "XmlParseHelperPrimitives.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(XmlParseHelperPrimitives);

	const string XmlParseHelperPrimitives::mXmlElementNameInteger = "integer";
	const string XmlParseHelperPrimitives::mXmlElementNameFloat = "float";
	const string XmlParseHelperPrimitives::mXmlElementNameString = "string";
	const string XmlParseHelperPrimitives::mXmlElementNameVec4 = "vec4";
	const string XmlParseHelperPrimitives::mXmlElementNameMat4x4 = "mat4x4";
	const string XmlParseHelperPrimitives::mXmlElementAttributeKeyName = "name";
	const string XmlParseHelperPrimitives::mXmlElementAttributeKeyValue = "value";

	Hashmap<string, DatumType> XmlParseHelperPrimitives::mHashmapXmlElementNames =
	{
		{make_pair(mXmlElementNameInteger, DatumType::INT32_T)},
		{make_pair(mXmlElementNameFloat, DatumType::FLOAT)},
		{make_pair(mXmlElementNameString, DatumType::STRING)},
		{make_pair(mXmlElementNameVec4, DatumType::GLM_VECTOR4)},
		{make_pair(mXmlElementNameMat4x4, DatumType::GLM_MATRIX4X4)}
	};

	XmlParseHelperPrimitives::XmlParseHelperPrimitives(XmlParseMaster& xmlParseMaster)
		:IXmlParseHelper(xmlParseMaster), mPrimitiveStringData(string()), mDatumName(string()), isValueFound(true)
	{
	}

	void XmlParseHelperPrimitives::Initialize()
	{
		mPrimitiveStringData = string();
		mDatumName = string();
		isValueFound = true;
	}

	bool XmlParseHelperPrimitives::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || !mHashmapXmlElementNames.ContainsKey(name))
		{
			return false;
		}
		if (!attributes.ContainsKey(mXmlElementAttributeKeyName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}

		if (attributes.ContainsKey(mXmlElementAttributeKeyValue))
		{
			Datum& datum = sharedDataTable->GetScope()->Append(attributes[mXmlElementAttributeKeyName]);
			datum.SetType(mHashmapXmlElementNames[name]);

			if (name == mXmlElementNameVec4 || name == mXmlElementNameMat4x4)
			{
				datum.PushFromString(name + attributes[mXmlElementAttributeKeyValue]);
			}
			else
			{
				datum.PushFromString(attributes[mXmlElementAttributeKeyValue]);
			}
		}
		else
		{
			mDatumName = attributes[mXmlElementAttributeKeyName];
			isValueFound = false;
		}
		return true;
	}

	bool XmlParseHelperPrimitives::EndElementHandler(SharedData& sharedData, const string& name)
	{
		if (!isValueFound && mPrimitiveStringData == string())
		{
			throw exception("No data found for the inserted element.");
		}

		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || !mHashmapXmlElementNames.ContainsKey(name))
		{
			return false;
		}

		if (!isValueFound)
		{
			Datum& datum = sharedDataTable->GetScope()->Append(mDatumName);
			datum.SetType(mHashmapXmlElementNames[name]);

			if (name == mXmlElementNameVec4 || name == mXmlElementNameMat4x4)
			{
				datum.PushFromString(name + mPrimitiveStringData);
			}
			else
			{
				datum.PushFromString(mPrimitiveStringData);
			}			
		
			mPrimitiveStringData = string();
			isValueFound = true;
		}
		return true;
	}

	void XmlParseHelperPrimitives::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		if (isValueFound)
		{
			throw exception("Data for the current Xml element was already present.");
		}
		mPrimitiveStringData.append(value);

		sharedData;
		length;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperPrimitives::Clone() const
	{
		return new XmlParseHelperPrimitives(*mXmlParseMaster);
	}
}