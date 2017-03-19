#include "Pch.h"
#include "XmlParseHelperStudent.h"
#include "XmlParseMaster.h"

using namespace std;

namespace UnitTestSupportClasses
{
	XmlParseHelperStudent::XmlParseHelperStudent(XmlParseMaster* xmlParseMaster)
	{
		mXmlParseMaster = xmlParseMaster;
		mXmlParseMaster->AddHelper(*this);
	}

	bool XmlParseHelperStudent::StartElementHandler(const string& name, const Hashmap<string, string>& attributes)
	{
		name;
		attributes;
		return false;
	}

	bool XmlParseHelperStudent::EndElementHandler(const string& name)
	{
		name;
		return false;
	}

	void XmlParseHelperStudent::CharacterDataHandler(const string& value, const int32_t length)
	{
		value;
		length;
	}

	XmlParseHelperStudent::~XmlParseHelperStudent()
	{
		mXmlParseMaster->RemoveHelper(*this);
	}
}
