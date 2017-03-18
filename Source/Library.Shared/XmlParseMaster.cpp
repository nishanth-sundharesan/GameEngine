#include "Pch.h"
#include "XmlParseMaster.h"
#include <fstream>

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::XmlParseMaster::SharedData);

namespace GameEngineLibrary
{
	XmlParseMaster::SharedData::SharedData()
		:mDepth(0), mXmlParseMaster(nullptr)
	{
		// TODO comeback and check again
	}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone() const
	{
		// TODO comeback and check again 
		// returns the address of a SharedData object which has the same internal state as “this” except ready for a fresh file.
		SharedData* newSharedData = new SharedData();
		newSharedData->mDepth = this->mDepth;
		newSharedData->mXmlParseMaster = this->mXmlParseMaster;
		return nullptr;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
	}

	XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster()
	{
		return const_cast<XmlParseMaster*>(const_cast<const SharedData*>(this)->GetXmlParseMaster());
	}

	const XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mXmlParseMaster;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
		--mDepth;
	}

	int32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	XmlParseMaster::XmlParseMaster(SharedData* sharedData)
		:mSharedData(sharedData), currentXmlParseHelper(nullptr), mParser(XML_ParserCreate(NULL))
	{
		// TODO sharedData->   //Initialize sharedData or what is this?		
		// TODO Check if we need & or not?
		XML_SetElementHandler(mParser, &XmlParseMaster::StartElementHandler, &XmlParseMaster::EndElementHandler);
		XML_SetCharacterDataHandler(mParser, &XmlParseMaster::CharacterDataHandler);
		XML_SetUserData(mParser, this);
	}

	XmlParseMaster::~XmlParseMaster()
	{
		//TODO  delete helpers of cloned parsers.
		XML_ParserFree(mParser);
	}

	XmlParseMaster* XmlParseMaster::Clone() const
	{
		//TODO read webcourses and implement this
		return nullptr;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper& xmlParseHelper)
	{
		mXmlParseHelpers.PushBack(&xmlParseHelper);
	}

	bool XmlParseMaster::RemoveHelper(IXmlParseHelper& xmlParseHelper)
	{
		return mXmlParseHelpers.Remove(&xmlParseHelper);
	}

	void XmlParseMaster::Parse(const char* rawXmlData, const uint32_t length, const bool isLastXmlChunkData)
	{
		// TODO char* can change to string
		if (XML_Parse(mParser, rawXmlData, length, isLastXmlChunkData) == 0)
		{
			int32_t parseErrorCode = XML_GetErrorCode(mParser);
			const char* parseErrorMessage = (const char*)XML_ErrorString((XML_Error)parseErrorCode);
			string exceptionMessage = "Error while parsing XML. ErrorCode: " + to_string(parseErrorCode) + "ErrorMessage: " + parseErrorMessage;
			XML_ParserFree(mParser);
			throw new exception(exceptionMessage.c_str());
		}
	}

	void XmlParseMaster::ParseFromFile(const string& fileName)
	{
		mCurrentParsingFile = fileName;

		std::ifstream xmlInput(fileName);
		if (!xmlInput)
		{
			string exceptionMessage = "Cannot open the file " + fileName + " for reading";
			throw new exception(exceptionMessage.c_str());
		}

		std::string readInputLine;
		while (std::getline(xmlInput, readInputLine))
		{
			Parse(readInputLine.c_str(), readInputLine.length(), !xmlInput.eof());
		}
		xmlInput.close();
	}

	string XmlParseMaster::GetFileName() const
	{
		return mCurrentParsingFile;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData()
	{
		return const_cast<SharedData*>(const_cast<const XmlParseMaster*>(this)->GetSharedData());
	}

	const XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
	}

	void XmlParseMaster::SetSharedData(SharedData* sharedData)
	{
		mSharedData = sharedData;
	}

	void XmlParseMaster::StartElementHandler(void* userData, const char* name, const char** attributes)
	{
		Hashmap<std::string, std::string> attributeHashmap;
		for (int32_t i = 0; attributes[i]; i += 2)
		{
			pair<string, string> tempPair = make_pair(attributes[i], attributes[i + 1]);
			attributeHashmap.Insert(tempPair);
		}

		XmlParseMaster* xmlParseMaster = static_cast<XmlParseMaster*>(userData);
		xmlParseMaster->mSharedData->IncrementDepth();

		for (auto& xmlParseHelper : xmlParseMaster->mXmlParseHelpers)
		{
			if (xmlParseHelper->StartElementHandler(name, attributeHashmap))
			{
				xmlParseMaster->currentXmlParseHelper = xmlParseHelper;
				break;
			}
		}
	}

	void XmlParseMaster::EndElementHandler(void* userData, const char* name)
	{
		XmlParseMaster* xmlParseMaster = static_cast<XmlParseMaster*>(userData);
		xmlParseMaster->mSharedData->DecrementDepth();

		for (auto& xmlParseHelper : xmlParseMaster->mXmlParseHelpers)
		{
			if (xmlParseHelper->EndElementHandler(name))
			{
				xmlParseMaster->currentXmlParseHelper = nullptr;
				break;
			}
		}
	}

	void XmlParseMaster::CharacterDataHandler(void* userData, const char* value, int32_t length)
	{
		XmlParseMaster* xmlParseMaster = static_cast<XmlParseMaster*>(userData);
		xmlParseMaster->currentXmlParseHelper->CharacterDataHandler(value, length);
	}
}
