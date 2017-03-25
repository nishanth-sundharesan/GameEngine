#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataTable.h"

namespace GameEngineLibrary
{	
	/** XmlParseHelper for all the supported primitive types(int32_t, float_t, std::string, glm::vec4, glm::mat4x4)
	*/
	class XmlParseHelperPrimitives : public IXmlParseHelper
	{		
	public:
		/** Parameterized constructor which takes in a reference to XmlParseMaster and registers itself to XmlParseMaster. Defaults the private members of the class.		
		*	@param xmlParseMaster The XmlParseMaster to which the XmlParseHelper has to be registered to.
		*/
		explicit XmlParseHelperPrimitives(XmlParseMaster& xmlParseMaster);

		/** Marking the copy constructor as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperPrimitives(const XmlParseHelperPrimitives&) = delete;

		/** Marking the assignment operator as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperPrimitives& operator=(const XmlParseHelperPrimitives&) = delete;

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

		/** Virtual function which creates copies of XmlParseHelperPrimitives.		
		*/
		virtual IXmlParseHelper* Clone() const override;

		/** Defaulted destructor.
		*/
		virtual ~XmlParseHelperPrimitives() = default;
	private:
		/** static const string containing the name of the integer XmlElement.
		*/
		static const std::string mXmlElementNameInteger;

		/** static const string containing the name of the float XmlElement.
		*/
		static const std::string mXmlElementNameFloat;

		/** static const string containing the name of the string XmlElement.
		*/
		static const std::string mXmlElementNameString;

		/** static const string containing the name of the vec4 XmlElement.
		*/
		static const std::string mXmlElementNameVec4;

		/** static const string containing the name of the mat4x4 XmlElement.
		*/
		static const std::string mXmlElementNameMat4x4;

		/** static const string containing the name of the "name" attribute of an XmlElement.
		*/
		static const std::string mXmlElementAttributeKeyName;

		/** static const string containing the name of the "value" attribute of an XmlElement.
		*/
		static const std::string mXmlElementAttributeKeyValue;

		/** A Hashmap containing the xml element names paired to DatumTypes.
		*/
		static Hashmap<std::string, DatumType> mHashmapXmlElementNames;

		/** Cached data between calls made to CharacterDataHandler.
		*/
		std::string mPrimitiveStringData;

		/** Cached name of the Datum.
		*/
		std::string mDatumName;		

		/** Set to true if the data was not found in the value attribute(waiting for data which might be present between the Xml elements), false otherwise.
		*/
		bool isValueFound;
	public:
		RTTI_DECLARATIONS(XmlParseHelperPrimitives, IXmlParseHelper);
	};
}