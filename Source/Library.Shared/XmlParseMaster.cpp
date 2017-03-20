#include "Pch.h"
#include "XmlParseMaster.h"
#include "IXmlParseHelper.h"
#include <fstream>
#include <assert.h>

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::XmlParseMaster::SharedData);

namespace GameEngineLibrary
{
#pragma region SharedData Implementation
	XmlParseMaster::SharedData::SharedData()
		:mDepth(0), mXmlParseMaster(nullptr)
	{
	}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone() const
	{
		SharedData* newSharedData = new SharedData();
		CloneInternalMembers(newSharedData);
		return newSharedData;
	}

	void XmlParseMaster::SharedData::Initialize()
	{
		mDepth = 0;
	}

	inline void XmlParseMaster::SharedData::SetXmlParseMaster(XmlParseMaster* xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
	}

	inline XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster()
	{
		return const_cast<XmlParseMaster*>(const_cast<const SharedData*>(this)->GetXmlParseMaster());
	}

	const XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster() const
	{
		return mXmlParseMaster;
	}

	inline void XmlParseMaster::SharedData::IncrementDepth()
	{
		++mDepth;
	}

	inline void XmlParseMaster::SharedData::DecrementDepth()
	{
		--mDepth;
		assert(mDepth >= 0);
	}

	int32_t XmlParseMaster::SharedData::Depth() const
	{
		return mDepth;
	}

	void XmlParseMaster::SharedData::CloneInternalMembers(SharedData* sharedData) const
	{
		sharedData->mDepth = 0;
		sharedData->mXmlParseMaster = mXmlParseMaster;
	}
#pragma endregion

#pragma region XmlParseMaster Implementation
	XmlParseMaster::XmlParseMaster(SharedData& sharedData)
		:mSharedData(&sharedData), mCurrentXmlParseHelper(nullptr), mParser(XML_ParserCreate(NULL)), mIsCloned(false), mCurrentParsingFile(""), mIsCurrentlyParsing(false)
	{
		mSharedData->Initialize();
		mSharedData->SetXmlParseMaster(this);

		XML_SetElementHandler(mParser, StartElementHandler, EndElementHandler);				//Register call backs for StartElementHandler and EndElementHandler
		XML_SetCharacterDataHandler(mParser, CharacterDataHandler);							//Register call back for CharacterDataHandler
		XML_SetUserData(mParser, this);
	}

	XmlParseMaster::~XmlParseMaster()
	{
		XML_ParserFree(mParser);
		if (mIsCloned)
		{
			delete mSharedData;
			for (auto& xmlParseHelper : mXmlParseHelpers)
			{
				delete xmlParseHelper;
			}
		}
	}

	XmlParseMaster* XmlParseMaster::Clone() const
	{
		XmlParseMaster* clonedXmlParseMaster = new XmlParseMaster(*(mSharedData->Clone()));
		clonedXmlParseMaster->mIsCloned = true;
		clonedXmlParseMaster->mCurrentParsingFile = mCurrentParsingFile;

		for (auto& xmlParseHelper : mXmlParseHelpers)
		{
			clonedXmlParseMaster->mXmlParseHelpers.PushBack(xmlParseHelper->Clone());
		}
		return clonedXmlParseMaster;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper& xmlParseHelper)
	{
		if (!mIsCloned)
		{
			mXmlParseHelpers.PushBack(&xmlParseHelper);
		}
	}

	bool XmlParseMaster::RemoveHelper(IXmlParseHelper& xmlParseHelper)
	{
		return mXmlParseHelpers.Remove(&xmlParseHelper);
	}

	void XmlParseMaster::Parse(const string& rawXmlData, const uint32_t length, const bool isLastXmlChunkData)
	{
		if (!mIsCurrentlyParsing)
		{
			InitializeSharedDataAndxmlParseHelpers();
			mIsCurrentlyParsing = true;
		}
		if (isLastXmlChunkData)
		{
			mIsCurrentlyParsing = false;
		}

		if (XML_Parse(mParser, rawXmlData.c_str(), length, isLastXmlChunkData) == 0)
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

		ifstream xmlInputFile(fileName);
		if (!xmlInputFile)
		{
			string exceptionMessage = "Cannot open the file " + fileName + " for reading";
			throw new exception(exceptionMessage.c_str());
		}

		string readInputLine;
		while (getline(xmlInputFile, readInputLine))
		{
			Parse(readInputLine, readInputLine.length(), xmlInputFile.eof());
		}
		xmlInputFile.close();
	}

	string XmlParseMaster::GetFileName() const
	{
		return mCurrentParsingFile;
	}

	void XmlParseMaster::SetSharedData(SharedData* sharedData)
	{
		mSharedData = sharedData;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData()
	{
		return const_cast<SharedData*>(const_cast<const XmlParseMaster*>(this)->GetSharedData());
	}

	const XmlParseMaster::SharedData* XmlParseMaster::GetSharedData() const
	{
		return mSharedData;
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
			if (xmlParseHelper->StartElementHandler(*xmlParseMaster->mSharedData, name, attributeHashmap))
			{
				xmlParseMaster->mCurrentXmlParseHelper = xmlParseHelper;
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
			if (xmlParseHelper->EndElementHandler(*xmlParseMaster->mSharedData, name))
			{
				xmlParseMaster->mCurrentXmlParseHelper = nullptr;
				break;
			}
		}
	}

	void XmlParseMaster::CharacterDataHandler(void* userData, const char* value, int32_t length)
	{
		XmlParseMaster* xmlParseMaster = static_cast<XmlParseMaster*>(userData);

		if (xmlParseMaster->mCurrentXmlParseHelper != nullptr)
		{
			xmlParseMaster->mCurrentXmlParseHelper->CharacterDataHandler(*xmlParseMaster->mSharedData, string(value, length), length, value[length] == '<');
		}
	}

	void XmlParseMaster::InitializeSharedDataAndxmlParseHelpers()
	{
		mSharedData->Initialize();															//Initializing the SharedData
		for (auto& xmlParseHelper : mXmlParseHelpers)										//Initializing all the helper objects.
		{
			xmlParseHelper->Initialize();
		}
	}
#pragma endregion
}
