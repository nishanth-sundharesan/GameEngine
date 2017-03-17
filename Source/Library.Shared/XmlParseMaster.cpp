#include "Pch.h"
#include "XmlParseMaster.h"

namespace GameEngineLibrary
{
	XmlParseMaster::SharedData::SharedData()
	{
	}

	XmlParseMaster::SharedData* XmlParseMaster::SharedData::Clone()
	{
		return nullptr;
	}

	void XmlParseMaster::SharedData::SetXmlParseMaster()
	{
	}

	XmlParseMaster* XmlParseMaster::SharedData::GetXmlParseMaster()
	{
		return nullptr;
	}

	void XmlParseMaster::SharedData::IncrementDepth()
	{
	}

	void XmlParseMaster::SharedData::DecrementDepth()
	{
	}

	int32_t XmlParseMaster::SharedData::Depth()
	{
		return depth;
	}

	XmlParseMaster::XmlParseMaster(SharedData* sharedData)
	{
		// TODO sharedData->   //Initialize sharedData
		mParser = XML_ParserCreate(NULL);
		// TODO Register static calls backs

		mParser;
		sharedData;
	}

	XmlParseMaster::~XmlParseMaster()
	{

	}

	XmlParseMaster* XmlParseMaster::Clone()
	{
		return nullptr;
	}

	void XmlParseMaster::AddHelper(IXmlParseHelper* xmlParseHelper)
	{
		mXmlParseHelpers.PushBack(xmlParseHelper);
	}

	bool XmlParseMaster::RemoveHelper(IXmlParseHelper* xmlParseHelper)
	{
		return mXmlParseHelpers.Remove(xmlParseHelper);
	}

	void XmlParseMaster::Parse(char* rawXmlData, uint32_t length, bool isLastXmlChunkData)
	{
		XML_Parse(mParser, rawXmlData, length, isLastXmlChunkData);
	}

	void XmlParseMaster::ParseFromFile(char* fileName)
	{
		fileName;
		XML_SetElementHandler(mParser, &XmlParseMaster::StartElementHandler, &XmlParseMaster::EndElementHandler);
	}

	char* XmlParseMaster::GetFileName()
	{
		return nullptr;
	}

	XmlParseMaster::SharedData* XmlParseMaster::GetSharedData()
	{
		return nullptr;
	}

	void XmlParseMaster::SetSharedData(SharedData* sharedData)
	{
		sharedData;
	}

	void XmlParseMaster::StartElementHandler(void *userData, const char *name, const char **atts)
	{
		userData;
		name;
		atts;
		/*for (auto& value : mXmlParseHelpers)
		{
			
		}*/				
	}

	void XmlParseMaster::EndElementHandler(void *userData, const char *name)
	{
		userData;
		name;
	}

	void XmlParseMaster::CharDataHandler()
	{
	}
}
