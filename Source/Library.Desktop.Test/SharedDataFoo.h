#pragma once
#include "XmlParseMaster.h"
#include "Hashmap.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	class SharedDataFoo :public XmlParseMaster::SharedData
	{
	public:
		SharedDataFoo();

		SharedDataFoo(const SharedDataFoo&) = delete;

		SharedDataFoo& operator=(const SharedDataFoo&) = delete;

		virtual SharedData* Clone() const override;

		virtual ~SharedDataFoo() = default;

		virtual void Initialize() override;

		Hashmap<std::string, std::string> mKeyValuePairs;

		std::string mReadCharacterData;

		RTTI_DECLARATIONS(SharedDataFoo, SharedData);
	};
}