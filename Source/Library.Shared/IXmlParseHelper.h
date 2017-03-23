#pragma once
#include "Hashmap.h"
#include "XmlParseMaster.h"

namespace GameEngineLibrary
{
	/** Abstract base class for all the Xml Parse Helper classes.
	*/
	class IXmlParseHelper
	{
	public:
		typedef XmlParseMaster::SharedData SharedData;

		/** Parameterized constructor which takes in a reference to XmlParseMaster and registers itself to XmlParseMaster.
		*	@param xmlParseMaster The XmlParseMaster to which the XmlParseHelper has to be registered to.
		*/
		IXmlParseHelper(XmlParseMaster& xmlParseMaster);

		/** Marking the copy constructor as delete. Use Clone() instead
		*	@see Clone()
		*/
		IXmlParseHelper(const IXmlParseHelper&) = delete;

		/** Marking the assignment operator as delete. Use Clone() instead
		*	@see Clone()
		*/
		IXmlParseHelper& operator=(const IXmlParseHelper&) = delete;

		/** Pure virtual function Initialize.
		*	The overridden method might initialize the members of the class
		*/
		virtual void Initialize() = 0;

		/** Pure virtual function StartElementHandler.
		*	It is called when the XmlParseMaster first encounters a start Xml element.
		*	This function is called from the XmlParseMaster and is passed with the name of the Xml element. If this function handles that element then it returns true, false otherwise.
		*	@param name The name of the Xml element.
		*	@param attributes Hashmap of Attribute name and value pairs.
		*	@returns Returns true if this function handles the passed xml element, false otherwise
		*/
		virtual bool StartElementHandler(SharedData& sharedData, const std::string& name, const Hashmap<std::string, std::string>& attributes) = 0;

		/** Pure virtual function EndElementHandler.
		*	It is called when the XmlParseMaster encounters an end Xml element.
		*	This function is called from the XmlParseMaster and is passed with the name of the Xml element. If this function handles that element then it returns true, false otherwise.
		*	@param name The name of the Xml element.
		*	@returns Returns true if this function handles the passed xml element, false otherwise
		*/
		virtual bool EndElementHandler(SharedData& sharedData, const std::string& name) = 0;

		/** Pure virtual function CharacterDataHandler.
		*	It is called when the XmlParseMaster encounters data inside Xml elements.
		*	@param value The string value containing the data between the Xml elements.
		*	@param length The length of the value.
		*/
		virtual void CharacterDataHandler(SharedData& sharedData, const std::string& value, const std::int32_t length, bool isCompleteData) = 0;

		/** Pure virtual function which creates copies of IXmlParseHelper.
		*	This function is marked as virtual so that the derived classes can Clone their copies in the sense of a virtual constructor.
		*/
		virtual IXmlParseHelper* Clone() = 0;

		/** Virtual destructor. Unregisters itself from the cached XmlParseMaster.
		*/
		virtual ~IXmlParseHelper();
	protected:
		/** The XmlParseMaster to which the inherited XmlParseHelper is registered to.
		*/
		XmlParseMaster *mXmlParseMaster;
	};
}