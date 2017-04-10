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
	const string XmlParseHelperWSE::mXmlElementNameAction = "action";
	const string XmlParseHelperWSE::mXmlElementNameCreateAction = "createaction";
	const string XmlParseHelperWSE::mXmlElementNameDeleteAction = "deleteaction";
	const string XmlParseHelperWSE::mXmlElementNameActionList = "actionlist";

	//Names of the Xml elements for int32_t, float_t and string types.
	const string XmlParseHelperWSE::mXmlElementNameInteger = "integer";
	const string XmlParseHelperWSE::mXmlElementNameFloat = "float";
	const string XmlParseHelperWSE::mXmlElementNameString = "string";

	//Names of the Xml elements for glm::vec4 and glm::mat4x4 types.
	const string XmlParseHelperWSE::mXmlElementNameVec4 = "vec4";
	const string XmlParseHelperWSE::mXmlElementNameMat4x4 = "mat4x4";

	//Attribute names
	const string XmlParseHelperWSE::mAttributeValue = "value";
	const string XmlParseHelperWSE::mAttributeName = "name";
	const string XmlParseHelperWSE::mAttributeClassName = "classname";
	const string XmlParseHelperWSE::mAttributeInstanceName = "instancename";
	const string XmlParseHelperWSE::mAttributeActionClassName = "actionclassname";
	const string XmlParseHelperWSE::mAttributeActionInstanceName = "actioninstancename";

	Hashmap<string, std::function<XmlParseHelperWSE::HandlerParameters>> XmlParseHelperWSE::mHashmapWSEXmlHandlers =
	{
		{ make_pair(mXmlElementNameWorld, WorldElementHandler) },
		{ make_pair(mXmlElementNameSector, SectorElementHandler) },
		{ make_pair(mXmlElementNameEntity, EntityElementHandler) },
		{ make_pair(mXmlElementNameAction, ActionElementHandler) },
		{ make_pair(mXmlElementNameActionList, ActionListElementHandler) }
	};

	Hashmap<string, std::function<XmlParseHelperWSE::HandlerParameters>> XmlParseHelperWSE::mHashmapActionCreateDeleteHandlers =
	{
		{ make_pair(mXmlElementNameCreateAction, ActionCreateActionElementHandler) },
		{ make_pair(mXmlElementNameDeleteAction, ActionDeleteActionElementHandler) }
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
		else if (mHashmapActionCreateDeleteHandlers.ContainsKey(name))
		{
			(mHashmapActionCreateDeleteHandlers[name])(*sharedDataWSE, attributes);
			return true;
		}
		else if (mHashmapPrimitiveTypes.ContainsKey(name))
		{
			if (!attributes.ContainsKey(mAttributeName) || !attributes.ContainsKey(mAttributeValue))
			{
				throw exception("The Attribute 'name' or 'value' is not present in the XML element.");
			}
			sharedDataWSE->AppendPrimitiveData(attributes[mAttributeName], mHashmapPrimitiveTypes[name], attributes[mAttributeValue]);
			return true;
		}
		else if (mHashmapGlmPrimitiveTypes.ContainsKey(name))
		{
			if (!attributes.ContainsKey(mAttributeName) || !attributes.ContainsKey(mAttributeValue))
			{
				throw exception("The Attribute 'name' or 'value' is not present in the XML element.");
			}
			sharedDataWSE->AppendPrimitiveData(attributes[mAttributeName], mHashmapGlmPrimitiveTypes[name], name + attributes[mAttributeValue]);
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
		else if (mHashmapPrimitiveTypes.ContainsKey(name) || mHashmapGlmPrimitiveTypes.ContainsKey(name) || mHashmapActionCreateDeleteHandlers.ContainsKey(name))
		{
			return true;
		}
		return false;
	}

	void XmlParseHelperWSE::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		if (DoesContainData(value, length))
		{
			throw exception("The Xml element cannot contain data inside them.");
		}
		sharedData;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperWSE::Clone() const
	{
		return new XmlParseHelperWSE(*mXmlParseMaster);
	}

	void XmlParseHelperWSE::WorldElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}
		sharedDataWSE.CreateWorld(attributes[mAttributeName]);
	}

	void XmlParseHelperWSE::SectorElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}
		sharedDataWSE.CreateSector(attributes[mAttributeName]);
	}

	void XmlParseHelperWSE::EntityElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeClassName) || !attributes.ContainsKey(mAttributeInstanceName))
		{
			throw exception("The ClassName attribute or the InstanceName attribute is not found.");
		}
		sharedDataWSE.CreateEntity(attributes[mAttributeClassName], attributes[mAttributeInstanceName]);
	}

	void XmlParseHelperWSE::ActionElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeClassName) || !attributes.ContainsKey(mAttributeInstanceName))
		{
			throw exception("The ClassName attribute or the InstanceName attribute is not found.");
		}
		sharedDataWSE.CreateAction(attributes[mAttributeClassName], attributes[mAttributeInstanceName]);
	}

	void XmlParseHelperWSE::ActionListElementHandler(SharedDataWSE & sharedDataWSE, const Hashmap<std::string, std::string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeName))
		{
			throw exception("The Attribute 'name' is not present in the XML element.");
		}
		sharedDataWSE.CreateActionList(attributes[mAttributeName]);
	}

	void XmlParseHelperWSE::ActionCreateActionElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{		
		if (!attributes.ContainsKey(mAttributeClassName) || !attributes.ContainsKey(mAttributeInstanceName) || !attributes.ContainsKey(mAttributeActionClassName) || !attributes.ContainsKey(mAttributeActionInstanceName))
		{
			throw exception("The ClassName or InstanceName or ActionClassName or ActionInstanceName attribute is not found.");
		}
		sharedDataWSE.AddActionCreateAction(attributes[mAttributeClassName], attributes[mAttributeInstanceName], attributes[mAttributeActionClassName], attributes[mAttributeActionInstanceName]);
	}

	void XmlParseHelperWSE::ActionDeleteActionElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes)
	{
		if (!attributes.ContainsKey(mAttributeClassName) || !attributes.ContainsKey(mAttributeInstanceName) || !attributes.ContainsKey(mAttributeActionInstanceName))
		{
			throw exception("The ClassName or InstanceName or ActionInstanceName attribute is not found.");
		}
		sharedDataWSE.AddActionDeleteAction(attributes[mAttributeClassName], attributes[mAttributeInstanceName], attributes[mAttributeActionInstanceName]);
	}
}