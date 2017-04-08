#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataWSE.h"
#include <functional>

namespace GameEngineLibrary
{
	/** XmlParseHelper for If statements. If must contain "then" xml element and can contain "else" xml element.
	*/
	class XmlParseHelperIf :public IXmlParseHelper
	{
	public:
		/** Parameterized constructor which takes in a reference to XmlParseMaster and registers itself to XmlParseMaster. Defaults the private members of the class.
		*	@param xmlParseMaster The XmlParseMaster to which the XmlParseHelper has to be registered to.
		*/
		explicit XmlParseHelperIf(XmlParseMaster& xmlParseMaster);

		/** Marking the copy constructor as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperIf(const XmlParseHelperIf&) = delete;

		/** Marking the assignment operator as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperIf& operator=(const XmlParseHelperIf&) = delete;

		/** Initializes/Defaults the members of the class.
		*/
		virtual void Initialize() override;

		/** StartElementHandler which gets called by the registered XmlParseMaster when a new xml element is found.
		*	@param sharedData The shared data associated with the XmlParseMaster.
		*	@param name The name of the Xml Element.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@returns Returns true if the function handles the xml element, false otherwise
		*	@exception Throws exception if the If or Then or Else element does not contain the instanceName or className attribute.
		*/
		virtual bool StartElementHandler(SharedData& sharedData, const std::string& name, const Hashmap<std::string, std::string>& attributes) override;

		/** EndElementHandler which gets called by the registered XmlParseMaster when an end xml element is found.
		*	@param sharedData The shared data associated with the XmlParseMaster.
		*	@param name The name of the Xml Element.
		*	@returns Returns true if the function handles the xml element, false otherwise
		*/
		virtual bool EndElementHandler(SharedData& sharedData, const std::string& name) override;

		/** CharacterDataHandler which gets called by the registered XmlParseMaster when the data between xml elements are found.
		*	@param sharedData The shared data associated with the XmlParseMaster.
		*	@param value The data between the Xml elements.
		*	@param length The length of the value/data.
		*	@param isCompleteData True is passed when complete data is passed, false otherwise.
		*	@exception This function throws exception if it contains any data other than the tab space.
		*/
		virtual void CharacterDataHandler(SharedData& sharedData, const std::string& value, const std::int32_t length, bool isCompleteData) override;

		/** Virtual function which creates copies of XmlParseHelperIf.
		*	@returns Returns the cloned XmlParseHelperIf.
		*/
		virtual IXmlParseHelper* Clone() const override;

		/** Defaulted destructor.
		*/
		virtual ~XmlParseHelperIf() = default;
	private:
		/** Type defining the parameters for the private static element handlers.
		*/
		using HandlerParameters = void(SharedDataWSE&, const Hashmap<string, string>&, bool&);

		/** This handler gets called when a start If Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the If element is present inside another If element or if it does not contain the lhsValue or rhsType attribute.
		*/
		static void IfElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes, bool& IsInsideIfElement);

		/** This handler gets called when a start Then Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the Then element is not present inside If element.
		*/
		static void ThenElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes, bool& IsInsideIfElement);

		/** This handler gets called when a start Else Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the Else element is not present inside If element.
		*/
		static void ElseElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes, bool& IsInsideIfElement);

		/** The name of the If xml element.
		*/
		static const std::string mXmlElementNameIf;

		/** The name of the Then xml element.
		*/
		static const std::string mXmlElementNameThen;

		/** The name of the Else xml element.
		*/
		static const std::string mXmlElementNameElse;

		/** LhsValue attribute name.
		*/
		static const std::string mAttributeLhsValueName;

		/** RhsValue attribute name.
		*/
		static const std::string mAttributeRhsValueName;

		/** ClassName attribute name.
		*/
		static const std::string mAttributeClassName;

		/** InstanceName attribute name.
		*/
		static const std::string mAttributeInstanceName;

		/** Static Hashmap containing Handlers for each Xml element.
		*/
		static Hashmap<std::string, std::function<HandlerParameters>> mHashmapIfXmlHandlers;

		/** Set to true if the parsing stage is inside "if" xml element, false otherwise.
		*/
		bool mIsInsideIfElement;
	public:
		RTTI_DECLARATIONS(XmlParseHelperIf, IXmlParseHelper);
	};
}