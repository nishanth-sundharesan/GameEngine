#pragma once
#include "XmlParseMaster.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	class SharedDataFoo :public XmlParseMaster::SharedData
	{
	public:
		SharedDataFoo() = default;

		SharedDataFoo(const SharedDataFoo&) = delete;

		SharedDataFoo& operator=(const SharedDataFoo&) = delete;

		virtual SharedData* Clone() const override;

		virtual ~SharedDataFoo() = default;

		RTTI_DECLARATIONS(SharedDataFoo, SharedData);
	};
}