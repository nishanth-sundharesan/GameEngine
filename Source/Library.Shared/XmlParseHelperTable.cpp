#include "Pch.h"
#include "XmlParseHelperTable.h"

RTTI_DEFINITIONS(GameEngineLibrary::XmlParseHelperTable::SharedDataTable);
using namespace std;

const string GameEngineLibrary::XmlParseHelperTable::mXmlElementName = "Scope";
const string GameEngineLibrary::XmlParseHelperTable::mXmlElementAttributeName = "name";

namespace GameEngineLibrary
{
#pragma region SharedDataTable
	XmlParseHelperTable::SharedDataTable::SharedDataTable()
		:mScope(nullptr)
	{
	}

	void XmlParseHelperTable::SharedDataTable::Initialize()
	{
		SharedData::Initialize();
		delete mScope;
		mScope = nullptr;
	}

	XmlParseMaster::SharedData* XmlParseHelperTable::SharedDataTable::Clone() const
	{
		SharedData* newSharedData = new SharedDataTable();
		CloneInternalMembers(newSharedData);
		return newSharedData;
	}

	Scope* XmlParseHelperTable::SharedDataTable::GetScope()
	{
		return const_cast<Scope*>(const_cast<const XmlParseHelperTable::SharedDataTable*>(this)->GetScope());
	}

	const Scope* XmlParseHelperTable::SharedDataTable::GetScope() const
	{
		return mScope;
	}

	void XmlParseHelperTable::SharedDataTable::AppendNewScope(const string& name)
	{
		if (mScope == nullptr)
		{
			mScope = new Scope();
		}
		else
		{
			mScope = &(mScope->AppendScope(name));
		}
	}

	void XmlParseHelperTable::SharedDataTable::GotoParentScope()
	{
		if (mScope->GetParent() != nullptr)
		{
			mScope = mScope->GetParent();
		}
	}

	XmlParseHelperTable::SharedDataTable::~SharedDataTable()
	{
		delete mScope;
	}

	void XmlParseHelperTable::SharedDataTable::CloneInternalMembers(SharedData* sharedData) const
	{
		SharedData::CloneInternalMembers(sharedData);
		*(static_cast<SharedDataTable*>(sharedData)->mScope) = *mScope;
	}
#pragma endregion

#pragma region XmlParseHelperTable
	XmlParseHelperTable::XmlParseHelperTable(XmlParseMaster& xmlParseMaster)
		:IXmlParseHelper(xmlParseMaster)
	{
	}

	void XmlParseHelperTable::Initialize()
	{
	}

	bool XmlParseHelperTable::StartElementHandler(SharedData& sharedData, const string& name, const Hashmap<string, string>& attributes)
	{
		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || name != mXmlElementAttributeName || !attributes.ContainsKey(mXmlElementAttributeName))
		{
			return false;
		}
		sharedDataTable->AppendNewScope(name);
		return true;
	}

	bool XmlParseHelperTable::EndElementHandler(SharedData& sharedData, const string& name)
	{
		SharedDataTable* sharedDataTable = sharedData.As<SharedDataTable>();
		if (sharedDataTable == nullptr || name != mXmlElementAttributeName)
		{
			return false;
		}
		sharedDataTable->GotoParentScope();
		return true;
	}

	void XmlParseHelperTable::CharacterDataHandler(SharedData& sharedData, const string& value, const int32_t length, bool isCompleteData)
	{
		throw exception("XmlParseHelperTable cannot handle any character data.");
		sharedData;
		value;
		length;
		isCompleteData;
	}

	IXmlParseHelper* XmlParseHelperTable::Clone()
	{
		return new XmlParseHelperTable(*mXmlParseMaster);		
	}
#pragma endregion
}