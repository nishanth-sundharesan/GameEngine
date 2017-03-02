#include "Pch.h"
#include "Attributed.h"

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::Scope);

namespace GameEngineLibrary
{
	uint32_t Attributed::sAttributedCount = 0;

	Hashmap<uint64_t, Vector<string>> Attributed::sPrescribedAttributes;

	Attributed::Attributed()
	{
		AddInternalAttribute("this", this);
		++sAttributedCount;
	}

	Attributed::Attributed(const Attributed& rhs)
	{
		rhs;
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		rhs;
		return *this;
		// TODO: insert return statement here
	}

	Attributed::~Attributed()
	{
		if (sAttributedCount == 0)
		{
			sPrescribedAttributes.Clear();
		}
		else
		{
			--sAttributedCount;
		}
	}

	bool Attributed::IsPrescribedAttribute(const string& name)
	{
		return sPrescribedAttributes[TypeIdInstance()].Find(name) != sPrescribedAttributes[TypeIdInstance()].end();
	}

	bool Attributed::IsAuxiliaryAttribute(const string& name)
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	bool Attributed::IsAttribute(const string& name)
	{
		return (Find(name) != nullptr);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(const string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw exception("Datum& Attributed::AppendAuxiliaryAttribute(const string& name): Trying to append an Auxliary attribute having the same name as that of a prescribed attribute.");
		}
		return Append(name);
	}

	uint32_t Attributed::AuxiliaryBegin()
	{
		return sPrescribedAttributes[TypeIdInstance()].Size() + 1;
	}

	void Attributed::AddInternalAttribute(const string& name, const int32_t value)
	{
		AddAttribute(name, DatumType::INT32_T).PushBack(value);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const float_t value)
	{
		AddAttribute(name, DatumType::FLOAT).PushBack(value);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const string& value)
	{
		AddAttribute(name, DatumType::STRING).PushBack(value);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const glm::vec4 value)
	{
		AddAttribute(name, DatumType::GLM_VECTOR4).PushBack(value);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const glm::mat4x4 value)
	{
		AddAttribute(name, DatumType::GLM_MATRIX4X4).PushBack(value);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const RTTI* value)
	{
		AddAttribute(name, DatumType::POINTER).PushBack(value);
		AddToPrescribedAtributesList(name);
	}



	void Attributed::AddExternalAttribute(const string& name, const int32_t* pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::INT32_T).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddExternalAttribute(const string& name, const float_t* pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::FLOAT).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddExternalAttribute(const string& name, const string* pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::STRING).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddExternalAttribute(const string& name, const glm::vec4* pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::GLM_VECTOR4).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddExternalAttribute(const string& name, const glm::mat4x4* pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::GLM_MATRIX4X4).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddExternalAttribute(const string& name, const RTTI** pointerToData, const uint32_t size)
	{
		AddAttribute(name, DatumType::POINTER).SetStorage(pointerToData, size);
		AddToPrescribedAtributesList(name);
	}

#pragma region Private Methods
	Datum& Attributed::AddAttribute(const string& name, const DatumType datumType)
	{
		Datum& datum = Append(name);
		datum.SetType(datumType);
		return datum;
	}

	void Attributed::AddToPrescribedAtributesList(const string& name)
	{
		Vector<string>& prescribedAttributes = sPrescribedAttributes[TypeIdInstance()];
		if (prescribedAttributes.Find(name) != prescribedAttributes.end())
		{
			prescribedAttributes.PushBack(name);
		}
	}
#pragma endregion
}