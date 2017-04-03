#include "Pch.h"
#include "XmlParseHelperWSE.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(XmlParseHelperWSE);

	//Names of the Xml elements for World, Sector and Entity
	const string XmlParseHelperWSE::mXmlElementNameWorld = "world";
	const string XmlParseHelperWSE::mXmlElementNameSector = "sector";
	const string XmlParseHelperWSE::mXmlElementNameEntity = "entity";

	//Names of the Xml elements for int32_t, float_t and string types.
	const string XmlParseHelperWSE::mXmlElementNameInteger = "integer";
	const string XmlParseHelperWSE::mXmlElementNameFloat = "float";
	const string XmlParseHelperWSE::mXmlElementNameString = "string";

	//Names of the Xml elements for glm::vec4 and glm::mat4x4 types.
	const string XmlParseHelperWSE::mXmlElementNameVec4 = "vec4";
	const string XmlParseHelperWSE::mXmlElementNameMat4x4 = "mat4x4";

	//Attribute names
	const string XmlParseHelperWSE::mXmlElementAttributeKeyValue = "value";
	const string XmlParseHelperWSE::mXmlElementAttributeKeyName = "name";
	const string XmlParseHelperWSE::mEntityElementClassName = "classname";
	const string XmlParseHelperWSE::mEntityElementInstanceName = "instancename";

	Hashmap<string, std::function<XmlParseHelperWSE::HandlerParameters>> XmlParseHelperWSE::mHashmapWSEXmlHandlers =
	{
		{make_pair(mXmlElementNameWorld, WorldElementHandler)},
		{make_pair(mXmlElementNameSector, SectorElementHandler)},
		{make_pair(mXmlElementNameEntity, EntityElementHandler)},
	};

	Hashmap<string, DatumType> XmlParseHelperWSE::mHashmapPrimitiveTypes =
	{
		{ make_pair(mXmlElementNameInteger, DatumType::INT32_T) },
		{ make_pair(mXmlElementNameFloat, DatumType::FLOAT) },
		{ make_pair(mXmlElementNameString, DatumType::STRING) }
	};

	Hashmap<string, DatumType> XmlParseHelperWSE::mHashmapGlmPrimitiveTypes =
	{
		{ make_pair(mXmlElementNameVec4, DatumType::GLM_VECTOR4) },
		{ make_pair(mXmlElementNameMat4x4, DatumType::GLM_MATRIX4X4) }
	};

	XmlParseHelperWSE::XmlParseHelperWSE(XmlParseMaster& xmlParseMaster)
		:IXmlParseHelper(xmlParseMaster)
	{
	}

	void XmlParseHelperWSE::Initialize()
	{
	}

	bool XmlParseHelperWSE::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		SharedDataWSE* sharedDataWSE = sharedData.As<SharedDataWSE>();
		if (sharedDataWSE == nullptr)
		{
			return false;
		}
		if (mHashmapWSEXmlHandlers.ContainsKey(name))
		{
			(mHashmapWSEXmlHandlers[name])(*sharedDataWSE, attributes);
			return true;
		}
		else if (mHashmapPrimitiveTypes.ContainsKey(name))
		{
			if (!attributes.ContainsKey(mXmlElementAttributeKeyName) || !attributes.ContainsKey(mXmlElementAttributeKeyValue))
			{
				throw exception("The Attribute 'name' or 'value' is not present in the XML element.");
			}
			sharedDataWSE->AppendPrimitiveData(attributes[mXmlElementAttributeKeyName], mHashmapPrimitiveTypes[name], attributes[mXmlElementAttributeKeyValue]);
			return true;
		}
		else if (mHashmapGlmPrimitiveTypes.ContainsKey(name))
		{
			if (!attributes.ContainsKey(mXmlElementAttributeKeyName) || !attributes.ContainsKey(mXmlElementAttributeKeyValue))
			{
				throw exception("The Attribute 'name' or 'value' is not present in the XML element.");
			}
			sharedDataWSE->AppendPrimitiveData(attributes[mXmlElementAttributeKeyName], mHashmapGlmPrimitiveTypes[name], name + attributes[mXmlElementAttributeKeyValue]);
			return true;
		}
		return false;
	}

	bool XmlParseHelperWSE::EndElementHandler(SharedData& sharedData, const string& name)
	{
		SharedDataWSE* sharedDataWSE = sharedData.As<SharedDataWSE>();
		if (sharedDataWSE == nullptr)
		{
			return false;
		}
		if (mHashmapWSEXmlHandlers.ContainsKey(name))
		{
			sharedDataWSE->GotoParentScope();
			return true;
		}
		else if (mHashmapPrimitiveTypes.ContainsKey(name) || mHashmapGlmPrimitiveTypes.ContainsKey(name))
		{
			return true;
		}
		return false;
	}

	void XmlParseHelperWSE::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		throw exception("The Xml element cannot contain data inside them.");
		sharedData;
		value;
		length;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperWSE::Clone() const
	{
		return new XmlParseHelperWSE(*mXmlParseMaster);
	}

	void XmlParseHelperWSE::WorldElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mXmlElementAttributeKeyName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}
		sharedDataWSE.CreateWorld(attributes[mXmlElementAttributeKeyName]);
	}

	void XmlParseHelperWSE::SectorElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mXmlElementAttributeKeyName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}
		sharedDataWSE.CreateSector(attributes[mXmlElementAttributeKeyName]);
	}

	void XmlParseHelperWSE::EntityElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mEntityElementClassName) || !attributes.ContainsKey(mEntityElementInstanceName))
		{
			throw exception("The ClassName attribute or the InstanceName attribute is not found.");
		}
		sharedDataWSE.CreateEntity(attributes[mEntityElementClassName], attributes[mEntityElementInstanceName]);
	}
}