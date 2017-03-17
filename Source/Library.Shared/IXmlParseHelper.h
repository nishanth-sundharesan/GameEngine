#pragma once

class IXmlParseHelper
{
public:
	void Initialize();

	// Can return bool
	void StartElementHandler(void* userData, const char* name, const char* args[]);

	// Can return bool
	void EndElementHandler(void* userData, const char* name);

	void CharDataHandler(void* userData, const char* value, int length);

	virtual IXmlParseHelper* Clone() = 0;

	virtual ~IXmlParseHelper();
};