#include "Pch.h"
#include "XmlParseHelperTracer.h"
#include "SharedDataFoo.h"

using namespace std;

namespace UnitTestSupportClasses
{
	XmlParseHelperTracer::XmlParseHelperTracer(XmlParseMaster& xmlParseMaster)
		:mXmlParseMaster(&xmlParseMaster), mXmlHandlerName("Tracer"), mStartElementHandlerCount(0), mEndElementHandlerCount(0), mCharDataHandlerCount(0), mMaxDepth(0), mIsInitialized(false), mIsCurrentlyHandlingData(false)
	{
		mXmlParseMaster->AddHelper(*this);
	}

	bool XmlParseHelperTracer::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		++mStartElementHandlerCount;
		if (sharedData.Depth() > mMaxDepth)
		{
			mMaxDepth = sharedData.Depth();
		}

		attributes;
		if (mIsCurrentlyHandlingData || (name == mXmlHandlerName && sharedData.Is(SharedData::TypeIdClass())))
		{						
			mIsCurrentlyHandlingData = true;
			return true;
		}
		return false;
	}

	bool XmlParseHelperTracer::EndElementHandler(SharedData& sharedData, const string& name)
	{
		++mEndElementHandlerCount;
		bool isEndElementFound = name == mXmlHandlerName && sharedData.Is(SharedData::TypeIdClass());
		if (isEndElementFound)
		{
			mIsCurrentlyHandlingData = false;
		}
		return (mIsCurrentlyHandlingData || isEndElementFound);
	}

	void XmlParseHelperTracer::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		++mCharDataHandlerCount;
		
		mWasPreviousDataCompleted = isCompleteData;

		sharedData;
		value;
		length;				
	}

	void XmlParseHelperTracer::Initialize()
	{
		mIsInitialized = true;
		mStartElementHandlerCount = 0;
		mEndElementHandlerCount = 0;
		mCharDataHandlerCount = 0;
		mMaxDepth = 0;
		mIsCurrentlyHandlingData = false;
	}

	IXmlParseHelper* XmlParseHelperTracer::Clone()
	{
		XmlParseHelperTracer* clonedXmlParseHelper = new XmlParseHelperTracer(*mXmlParseMaster);
		clonedXmlParseHelper->mXmlHandlerName = mXmlHandlerName;
		return clonedXmlParseHelper;
	}

	XmlParseHelperTracer::~XmlParseHelperTracer()
	{
		mXmlParseMaster->RemoveHelper(*this);
	}
}
