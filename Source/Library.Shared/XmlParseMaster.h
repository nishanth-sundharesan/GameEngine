#pragma once
#include "Vector.h"
#include "RTTI.h"
#include <expat.h>

namespace GameEngineLibrary
{
	/** Forward declaring the IXmlParseHelper class as we are including XmlParseMaster.h inside the XmlParseHelper classes.
	*/
	class IXmlParseHelper;

	/** XmlParseMaster class which reads XML using Expat library.
	*/
	class XmlParseMaster final
	{		
	public:
		/** Embedded SharedData class whose (instance)data will be shared across XmlParseHelper classes and XmlParseMaster.
		*/
		class SharedData :public RTTI
		{
		public:
			/** Zero parameterized constructor. Initializes the private members of the class.
			*/
			SharedData();

			/** Marking the copy constructor as delete as creating an exact copy of the SharedData class would be inappropriate
			*	Note: Clone() can be used to make copies of SharedData.
			*	@see Clone()
			*/
			SharedData(const SharedData&) = delete;

			/** Marking the assignment operator as delete as creating an exact copy of the SharedData class would be inappropriate
			*	Note: Clone() can be used to make copies of SharedData.
			*	@see Clone()
			*/
			SharedData& operator=(const SharedData&) = delete;

			/** Virtual Clone function which creates copies of SharedData.
			*	Note: The depth variable is reset to 0 for the cloned SharedData.
			*	This function is marked as virtual so that the derived classes can Clone their copies in the sense of a virtual constructor.
			*/
			virtual SharedData* Clone() const;

			/** Initializes/Defaults the members of the class.
			*/
			virtual void Initialize();

			/** Takes in the address of the XmlParseMaster class and saves it as a private copy.
			*	@param xmlParseMaster The address of the XmlParseMaster class
			*/
			void SetXmlParseMaster(XmlParseMaster& xmlParseMaster);

			/** Returns the address of the XmlParseMaster class(which was cached using SetXmlParseMaster()), returns nullptr if it wasn't cached.
			*	@returns Returns the address of the XmlParseMaster class(which was cached using SetXmlParseMaster()), returns nullptr if it wasn't cached.
			*/
			XmlParseMaster* GetXmlParseMaster();

			/** Returns the address of the XmlParseMaster class(which was cached using SetXmlParseMaster()), returns nullptr if it wasn't cached.
			*	@returns Returns the address of the XmlParseMaster class(which was cached using SetXmlParseMaster()), returns nullptr if it wasn't cached.
			*/
			const XmlParseMaster* GetXmlParseMaster() const;

			/** Increments the private integer depth variable.
			*	Note: This function is usually called when a start xml element is found while parsing.
			*/
			void IncrementDepth();

			/** Decrements the private integer depth variable.
			*	Note: This function is usually called when an end xml element is found while parsing.
			*/
			void DecrementDepth();

			/** Returns a copy of the private integer depth variable.
			*	@returns Returns a copy of the private integer depth variable.
			*/
			std::int32_t Depth() const;

			/** Defaulted virtual destructor.
			*/
			virtual ~SharedData() = default;
		protected:
			/** Clones the internal members of the SharedData class.
			*	Note: This method gets called from the Clone().
			*	@param sharedData The cloned SharedData pointer.
			*/
			virtual void CloneInternalMembers(SharedData* sharedData) const;

			/** Cached address of the XmlParseMaster class.
			*/
			XmlParseMaster* mXmlParseMaster;

			/** The cached depth integer variable.
			*	Indicates the depth of the XML reached while parsing.
			*/
			std::int32_t mDepth;
		public:
			RTTI_DECLARATIONS(SharedData, RTTI);
		};

	public:
		/** Parameterized constructor for XmlParseMaster.
		*	Creates an Expat object, registers the static callbacks and initializes member variables.
		*	@param sharedData The address of the SharedData class associated with the XmlParseMaster class.
		*/
		XmlParseMaster(SharedData& sharedData);

		/** Marking the copy constructor as delete as creating an exact copy of the XmlParseMaster class would be inappropriate
		*	Note: Clone() can be used to make copies of XmlParseMaster.
		*	@see Clone()
		*/
		XmlParseMaster(const XmlParseMaster&) = delete;

		/** Marking the assignment operator as delete as creating an exact copy of the XmlParseMaster class would be inappropriate
		*	Note: Clone() can be used to make copies of XmlParseMaster.
		*	@see Clone()
		*/
		XmlParseMaster& operator=(const XmlParseMaster&) = delete;

		/**	Frees the Expat object and deletes the helpers of cloned parsers.
		*/
		~XmlParseMaster();

		/** Clone method which creates copies of XmlParseMaster.
		*	Note: The internal state of the Expat object is not replicated. The bool mIsCloned is set to true for the cloned XmlParseMaster.
		*	The XmlParserHelpers associated with this object is replicated using their clone methods.
		*/
		XmlParseMaster* Clone() const;

		/** Takes in a reference to the IXmlParseHelper object and adds it to the list of IXmlParseHelper objects. Cannot add IXmlParseHelper to the cloned XmlParseMaster.
		*	@param xmlParseHelper The reference to an IXmlParseHelper object which has to be added to the list of IXmlParseHelper objects.
		*	@returns Returns true if the xmlParseHelper was successfully added, false otherwise.
		*/
		bool AddHelper(IXmlParseHelper& xmlParseHelper);

		/** Takes in a reference to the IXmlParseHelper object and removes it from the list of IXmlParseHelper objects.
		*	@param xmlParseHelper The reference to an IXmlParseHelper object which has to be removed from the list of IXmlParseHelper objects.
		*	@returns Returns true if the xmlParseHelper was successfully removed, false otherwise.
		*/
		bool RemoveHelper(IXmlParseHelper& xmlParseHelper);

		/** Parses the Xml character buffer.
		*	@param rawXmlData A character buffer containing the Xml elements to be parsed.
		*	@param length The length of the character buffer.
		*	@param isLastXmlChunkData True is passed if the character buffer contains the last chunk of the XmlData.
		*	@exception Throws an exception if the Expat's Xml parsing fails.
		*/
		void Parse(const std::string& rawXmlData, const std::uint32_t length, const bool isLastXmlChunkData);

		/** Takes in the string containing the path to the Xml file(along with the filename) and parses the entire file.
		*	@param fileName The string containing the path to the Xml file(along with the filename).
		*	@exception Throws an exception if the file couldn't be opened.
		*/
		void ParseFromFile(const std::string& fileName);

		/** Returns the string containing the path to the Xml file(along with the filename).
		*	@returns Returns the string containing the path to the Xml file(along with the filename)
		*/
		std::string GetFileName() const;

		/** Takes in the address of the SharedData class and saves it as a private copy(caches it). Cannot Set SharedData for a cloned XmlMaster.
		*	@param sharedData The address of the SharedData class.
		*	@returns Returns true if the sharedData was successfully set, false otherwise.
		*/
		bool SetSharedData(SharedData& sharedData);

		/** Returns the address of the SharedData class(which was cached using SetSharedData()).
		*	@returns Returns the address of the SharedData class(which was cached using SetSharedData()).
		*/
		SharedData* GetSharedData();

		/** Returns the address of the SharedData class(which was cached using SetSharedData()).
		*	@returns Returns the address of the SharedData class(which was cached using SetSharedData()).
		*/
		const SharedData* GetSharedData() const;
	private:
		/** static StartElementHandler function which has the same parameters as required by the Expat's callback StartElementHandler function.
		*	Note: This function is registered to Expat and is called by Expat when it Expat finds a start Xml element .
		*	@param userData Expat returns the cached void*. This userData was cached using the function XML_SetUserData().
		*	@param name 1-D Character array containing the name of the Xml Element.
		*	@param attributes 2-D Character array containing the key-value pairs for the attributes
		*/
		static void StartElementHandler(void *userData, const char *name, const char **attributes);

		/** static EndElementHandler function which has the same parameters as required by the Expat's callback EndElementHandler function.
		*	Note: This function is registered to Expat and is called by Expat when it Expat finds an end Xml element .
		*	@param userData Expat returns the cached void*. This userData was cached using the function XML_SetUserData().
		*	@param name 1-D Character array containing the name of the Xml Element.
		*/
		static void EndElementHandler(void *userData, const char *name);

		/** static CharacterDataHandler function which has the same parameters as required by the Expat's callback CharacterDataHandler function.
		*	Note: This function is registered to Expat and is called by Expat when it finds the data inside Xml elements.
		*	@param userData Expat returns the cached void*. This userData was cached using the function XML_SetUserData().
		*	@param value 1-D Character array containing the data inside the Xml elements.
		*	@param length The length of the value (i.e 1-D character array).
		*/
		static void CharacterDataHandler(void *userData, const char *value, std::int32_t length);

		/** Initializes the SharedData and XmlParseHelpers.
		*/
		void InitializeSharedDataAndxmlParseHelpers();
	private:
		/** Object of an Expat's XML_Parser
		*/
		XML_Parser mParser;

		/** Contains the path of the current Xml file being parsed.
		*/
		string mCurrentParsingFile;

		/** Cached pointer to the SharedData object.
		*/
		SharedData* mSharedData;

		/** Cached pointer to the IXmlParseHelper object.
		*/
		IXmlParseHelper* mCurrentXmlParseHelper;

		/** List of Xml parse helpers.
		*/
		Vector<IXmlParseHelper*> mXmlParseHelpers;

		/** Set to True if it is the ClonedXmlParser, false otherwise.
		*/
		bool mIsCloned;

		/** Set to True if the parser is in between of parsing, false otherwise.
		*/
		bool mIsCurrentlyParsing;
	};
}