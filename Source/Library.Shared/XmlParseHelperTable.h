#pragma once
#include "IXmlParseHelper.h"

namespace GameEngineLibrary
{
	/** XmlParseHelper for the Scope element.
	*/
	class XmlParseHelperTable : public IXmlParseHelper
	{
		typedef XmlParseMaster::SharedData SharedData;
	public:
		/** Parameterized constructor which takes in a reference to XmlParseMaster and registers itself to XmlParseMaster.
		*	@param xmlParseMaster The XmlParseMaster to which the XmlParseHelper has to be registered to.
		*/
		explicit XmlParseHelperTable(XmlParseMaster& xmlParseMaster);

		/** Marking the copy constructor as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperTable(const XmlParseHelperTable&) = delete;

		/** Marking the assignment operator as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperTable& operator=(const XmlParseHelperTable&) = delete;

		/** Initializes/Defaults the members of the class.
		*/
		virtual void Initialize() override;
		
		/** StartElementHandler which gets called by the registered XmlParseMaster when a new xml element is found.
		*	@param sharedData The shared data associated with the XmlParseMaster.
		*	@param name The name of the Xml Element.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@returns Returns true if the function handles the xml element, false otherwise
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
		*/
		virtual void CharacterDataHandler(SharedData& sharedData, const std::string& value, const std::int32_t length, bool isCompleteData) override;

		/** Virtual function which creates copies of XmlParseHelperTable.
		*/
		virtual IXmlParseHelper* Clone() const override;

		/** Defaulted destructor.
		*/
		virtual ~XmlParseHelperTable() = default;
	private:
		/** static const string containing the name of the scope XmlElement.
		*/
		static const std::string mXmlElementScopeName;

		/** static const string containing the name of the "name" attribute of the XmlElement.
		*/
		static const std::string mXmlElementAttributeKeyName;
	public:
		RTTI_DECLARATIONS(XmlParseHelperTable, IXmlParseHelper);
	};
}