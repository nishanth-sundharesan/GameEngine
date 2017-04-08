#include "Pch.h"
#include "XmlParseHelperIf.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(XmlParseHelperIf);

	// Names of the Xml elements for If, Then and Else
	const string XmlParseHelperIf::mXmlElementNameIf = "if";
	const string XmlParseHelperIf::mXmlElementNameThen = "then";
	const string XmlParseHelperIf::mXmlElementNameElse = "else";

	//Attribute names
	const string XmlParseHelperIf::mAttributeLhsValueName = "lhsvalue";
	const string XmlParseHelperIf::mAttributeRhsValueName = "rhsvalue";
	const string XmlParseHelperIf::mAttributeClassName = "classname";
	const string XmlParseHelperIf::mAttributeInstanceName = "instancename";

	Hashmap<string, std::function<XmlParseHelperIf::HandlerParameters>> XmlParseHelperIf::mHashmapIfXmlHandlers =
	{
		{make_pair(mXmlElementNameIf, IfElementHandler)},
		{make_pair(mXmlElementNameThen, ThenElementHandler)},
		{make_pair(mXmlElementNameElse, ElseElementHandler)}
	};


	XmlParseHelperIf::XmlParseHelperIf(XmlParseMaster& xmlParseMaster)
		:IXmlParseHelper(xmlParseMaster), mIsInsideIfElement(false)
	{
	}

	void XmlParseHelperIf::Initialize()
	{
		mIsInsideIfElement = false;
	}

	bool XmlParseHelperIf::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		SharedDataWSE* sharedDataWSE = sharedData.As<SharedDataWSE>();
		if (sharedDataWSE == nullptr)
		{
			return false;
		}
		if (mHashmapIfXmlHandlers.ContainsKey(name))
		{
			if (!attributes.ContainsKey(mAttributeClassName) || !attributes.ContainsKey(mAttributeInstanceName))
			{
				throw exception("The ClassName attribute or the InstanceName attribute is not found.");
			}
			(mHashmapIfXmlHandlers[name])(*sharedDataWSE, attributes, mIsInsideIfElement);
			return true;
		}
		return false;
	}

	bool XmlParseHelperIf::EndElementHandler(SharedData& sharedData, const string& name)
	{
		SharedDataWSE* sharedDataWSE = sharedData.As<SharedDataWSE>();
		if (sharedDataWSE == nullptr)
		{
			return false;
		}
		if (name == mXmlElementNameIf)
		{
			sharedDataWSE->AssignComparisonsForIf();
			sharedDataWSE->GotoParentScope();
			mIsInsideIfElement = false;

			return true;
		}
		else if (mHashmapIfXmlHandlers.ContainsKey(name))
		{
			return true;
		}
		return false;
	}

	void XmlParseHelperIf::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		if (DoesContainData(value, length))
		{
			throw exception("The Xml element cannot contain data inside them.");
		}
		sharedData;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperIf::Clone() const
	{
		return new XmlParseHelperIf(*mXmlParseMaster);
	}

	void XmlParseHelperIf::IfElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes, bool& isInsideIfElement)
	{
		if (!attributes.ContainsKey(mAttributeLhsValueName) || !attributes.ContainsKey(mAttributeRhsValueName))
		{
			throw exception("The LhsValue attribute or the RhsValue attribute is not found.");
		}
		if (isInsideIfElement)
		{
			throw exception("An 'If' xml element cannot be present inside another 'If' xml element.");
		}
		isInsideIfElement = true;
		sharedDataWSE.CreateActionIf(attributes[mAttributeClassName], attributes[mAttributeInstanceName], attributes[mAttributeLhsValueName], attributes[mAttributeRhsValueName]);
	}

	void XmlParseHelperIf::ThenElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes, bool& isInsideIfElement)
	{
		if (!isInsideIfElement)
		{
			throw exception("'Then' xml element has to be present inside 'If' xml element.");
		}
		sharedDataWSE.AddThenAction(attributes[mAttributeClassName], attributes[mAttributeInstanceName]);
	}

	void XmlParseHelperIf::ElseElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<string, string>& attributes, bool& isInsideIfElement)
	{
		if (!isInsideIfElement)
		{
			throw exception("'Else' xml element has to be present inside 'If' xml element.");
		}
		sharedDataWSE.AddElseAction(attributes[mAttributeClassName], attributes[mAttributeInstanceName]);
	}
}