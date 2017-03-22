#include "Pch.h"
#include "XmlParseHelperStudent.h"
#include "SharedDataFoo.h"

using namespace std;

namespace UnitTestSupportClasses
{
	XmlParseHelperStudent::XmlParseHelperStudent(XmlParseMaster& xmlParseMaster)
		:mXmlParseMaster(&xmlParseMaster), mXmlHandlerName("Student"), mStartElementHandlerCount(0), mEndElementHandlerCount(0), mCharDataHandlerCount(0), mMaxDepth(0), mIsInitialized(false), mIsCurrentlyHandlingData(false)
	{
		mXmlParseMaster->AddHelper(*this);
	}

	bool XmlParseHelperStudent::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		++mStartElementHandlerCount;
		if (sharedData.Depth() > mMaxDepth)
		{
			mMaxDepth = sharedData.Depth();
		}

		if (mIsCurrentlyHandlingData || (name == mXmlHandlerName && sharedData.Is(SharedDataFoo::TypeIdClass())))
		{
			SharedDataFoo& sharedDataFoo = static_cast<SharedDataFoo&>(sharedData);
			sharedDataFoo.mKeyValuePairs = attributes;
			mIsCurrentlyHandlingData = true;
			return true;
		}
		return false;
	}

	bool XmlParseHelperStudent::EndElementHandler(SharedData& sharedData, const string& name)
	{
		++mEndElementHandlerCount;
		bool isEndElementFound = name == mXmlHandlerName && sharedData.Is(SharedDataFoo::TypeIdClass());
		if (isEndElementFound)
		{
			mIsCurrentlyHandlingData = false;
		}
		return (mIsCurrentlyHandlingData || isEndElementFound);
	}

	void XmlParseHelperStudent::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		++mCharDataHandlerCount;

		SharedDataFoo& sharedDataFoo = static_cast<SharedDataFoo&>(sharedData);
		if (!mWasPreviousDataCompleted)
		{
			sharedDataFoo.mReadCharacterData += value;
		}
		else
		{
			sharedDataFoo.mReadCharacterData = value;
		}
		mWasPreviousDataCompleted = isCompleteData;
		length;
	}

	void XmlParseHelperStudent::Initialize()
	{
		mIsInitialized = true;
		mStartElementHandlerCount = 0;
		mEndElementHandlerCount = 0;
		mCharDataHandlerCount = 0;
		mMaxDepth = 0;
		mIsCurrentlyHandlingData = false;
	}

	IXmlParseHelper* XmlParseHelperStudent::Clone()
	{
		XmlParseHelperStudent* clonedXmlParseHelper = new XmlParseHelperStudent(*mXmlParseMaster);
		clonedXmlParseHelper->mXmlHandlerName = mXmlHandlerName;
		return clonedXmlParseHelper;
	}

	XmlParseHelperStudent::~XmlParseHelperStudent()
	{
		mXmlParseMaster->RemoveHelper(*this);
	}
}
