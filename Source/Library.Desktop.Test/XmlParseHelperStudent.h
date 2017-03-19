#pragma once
#include "IXmlParseHelper.h"

using namespace GameEngineLibrary;

namespace GameEngineLibrary
{
	class XmlParseMaster;
}

namespace UnitTestSupportClasses
{	
	class XmlParseHelperStudent final :IXmlParseHelper
	{
	public:
		XmlParseHelperStudent(XmlParseMaster* xmlParseMaster);

		virtual bool StartElementHandler(const std::string& name, const Hashmap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(const std::string& name) override;

		virtual void CharacterDataHandler(const string& value, const int32_t length) override;

		~XmlParseHelperStudent();
	private:
		XmlParseMaster* mXmlParseMaster;
	};
}