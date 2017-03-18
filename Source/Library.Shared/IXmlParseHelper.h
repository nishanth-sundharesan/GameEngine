#pragma once
#include "Hashmap.h"

namespace GameEngineLibrary
{
	class IXmlParseHelper
	{
	public:
		IXmlParseHelper() = default;

		IXmlParseHelper(const IXmlParseHelper&) = delete;

		IXmlParseHelper& operator=(const IXmlParseHelper&) = delete;

		void Initialize();

		virtual bool StartElementHandler(const std::string& name, const Hashmap<std::string, std::string>& attributes) = 0;

		virtual bool EndElementHandler(const std::string& name) = 0;

		void CharacterDataHandler(const std::string& value, const std::int32_t length);

		virtual IXmlParseHelper* Clone() = 0;

		virtual ~IXmlParseHelper() = default;
	};
}