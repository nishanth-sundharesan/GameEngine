/** Xml Parse Helper for World, Sector and Entity.
*/
#pragma once
#include "IXmlParseHelper.h"
#include "SharedDataWSE.h"
#include <functional>

namespace GameEngineLibrary
{
	/** XmlParseHelper for World, Entity, Sector plus for all the supported primitive types(int32_t, float_t, std::string, glm::vec4, glm::mat4x4)
	*/
	class XmlParseHelperWSE :public IXmlParseHelper
	{
	public:
		/** Parameterized constructor which takes in a reference to XmlParseMaster and registers itself to XmlParseMaster. Defaults the private members of the class.
		*	@param xmlParseMaster The XmlParseMaster to which the XmlParseHelper has to be registered to.
		*/
		explicit XmlParseHelperWSE(XmlParseMaster& xmlParseMaster);

		/** Marking the copy constructor as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperWSE(const XmlParseHelperWSE&) = delete;

		/** Marking the assignment operator as delete. Use Clone() instead
		*	@see Clone()
		*/
		XmlParseHelperWSE& operator=(const XmlParseHelperWSE&) = delete;

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
		*	@exception This function throws exception when it is called as this helper does not handle any data inside Xml elements.
		*/
		virtual void CharacterDataHandler(SharedData& sharedData, const std::string& value, const std::int32_t length, bool isCompleteData) override;

		/** Virtual function which creates copies of XmlParseHelperWSE.
		*	@returns Returns the cloned XmlParseHelperWSE.
		*/
		virtual IXmlParseHelper* Clone() const override;

		/** Defaulted destructor.
		*/
		virtual ~XmlParseHelperWSE() = default;		
	private:
		/** Type defining the parameters to the private static element handlers.
		*/
		using HandlerParameters = void(SharedDataWSE&, const Hashmap<string, string>&);

		/** This handler gets called when a start World Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the World element does not contain the name attribute.
		*/
		static void WorldElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes);

		/** This handler gets called when a start Sector Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the Sector element does not contain the name attribute.
		*/
		static void SectorElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes);

		/** This handler gets called when a start Entity Xml element is found.
		*	@param sharedDataWSE Reference to the shared data associated with this helper.
		*	@param attributes Hashmap of the key-value pairs of attributes.
		*	@exception Throws exception if the Sector element does not contain the className attribute or instanceName attribute.
		*/
		static void EntityElementHandler(SharedDataWSE& sharedDataWSE, const Hashmap<std::string, std::string>& attributes);
		
		/** The name of the World xml element.
		*/
		static const std::string mXmlElementNameWorld;

		/** The name of the Sector xml element.
		*/
		static const std::string mXmlElementNameSector;

		/** The name of the Entity xml element.
		*/
		static const std::string mXmlElementNameEntity;

		/** The name of the Integer xml element.
		*/
		static const std::string mXmlElementNameInteger;

		/** The name of the Float xml element.
		*/
		static const std::string mXmlElementNameFloat;

		/** The name of the String xml element.
		*/
		static const std::string mXmlElementNameString;

		/** The name of the Vec4 xml element.
		*/
		static const std::string mXmlElementNameVec4;

		/** The name of the Mat4x4 xml element.
		*/
		static const std::string mXmlElementNameMat4x4;

		/** The name of the value Attribute.
		*/
		static const std::string mXmlElementAttributeKeyValue;

		/** The name of the name Attribute.
		*/
		static const std::string mXmlElementAttributeKeyName;

		/** The name of the Entity class name attribute.
		*/
		static const std::string mEntityElementClassName;

		/** The name of the Entity Instance name attribute.
		*/
		static const std::string mEntityElementInstanceName;

		/** Static Hashmap containing Handlers for each Xml element.
		*/
		static Hashmap<std::string, std::function<HandlerParameters>> mHashmapWSEXmlHandlers;

		/** Static Hashmap containing the Xml(int32_t, float_t, string) element names paired to DatumTypes.
		*/
		static Hashmap<std::string, DatumType> mHashmapPrimitiveTypes;

		/** Static Hashmap containing the Xml(glm::vec4, glm::mat4x4) element names paired to DatumTypes.
		*/
		static Hashmap<std::string, DatumType> mHashmapGlmPrimitiveTypes;
	public:
		RTTI_DECLARATIONS(XmlParseHelperWSE, IXmlParseHelper);
	};
}