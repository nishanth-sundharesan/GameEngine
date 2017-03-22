#pragma once
#include "IXmlParseHelper.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	class XmlParseHelperTracer final :public IXmlParseHelper
	{
	public:
		XmlParseHelperTracer(XmlParseMaster& xmlParseMaster);

		virtual bool StartElementHandler(SharedData& sharedData, const std::string& name, const Hashmap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(SharedData& sharedData, const std::string& name) override;

		virtual void CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData) override;

		virtual void Initialize() override;

		virtual IXmlParseHelper* Clone() override;

		~XmlParseHelperTracer();

		bool mIsInitialized;

		std::int32_t mStartElementHandlerCount;

		std::int32_t mEndElementHandlerCount;

		std::int32_t mCharDataHandlerCount;

		std::int32_t mMaxDepth;

	private:
		XmlParseMaster* mXmlParseMaster;

		std::string mXmlHandlerName;

		bool mWasPreviousDataCompleted;

		bool mIsCurrentlyHandlingData;
	};
}