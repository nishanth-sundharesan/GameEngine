#include "Pch.h"
#include "Attributed.h"

using namespace std;

RTTI_DEFINITIONS(GameEngineLibrary::Attributed);

namespace GameEngineLibrary
{
	uint32_t Attributed::sAttributedCount = 0;

	Hashmap<uint64_t, Vector<string>> Attributed::sPrescribedAttributes;

	Attributed::Attributed()
	{
		++sAttributedCount;
		AddInternalAttribute("this", this);
	}

	Attributed::Attributed(const Attributed& rhs)
		:Scope(rhs)
	{
		++sAttributedCount;
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed::Attributed(Attributed&& rhs)
		: Scope(move(rhs))
	{
		++sAttributedCount;
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = static_cast<RTTI*>(this);
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(move(rhs));
			(*this)["this"] = static_cast<RTTI*>(this);
		}
		return *this;
	}

	Attributed::~Attributed()
	{
		--sAttributedCount;
		if (sAttributedCount == 0)
		{
			sPrescribedAttributes.Clear();
		}
	}

	bool Attributed::IsPrescribedAttribute(const string& name)
	{
		return !(sPrescribedAttributes[TypeIdInstance()].Find(name) == sPrescribedAttributes[TypeIdInstance()].end());
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
		return sPrescribedAttributes[TypeIdInstance()].Size() + 1;								//+ 1 is for the "this" pointer which is added in the constructor
	}

	void Attributed::AddInternalAttribute(const string& name, const int32_t value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const float_t value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const string& value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const glm::vec4& value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const glm::mat4x4& value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalAttribute(const string& name, const RTTI* value)
	{
		Append(name) = value;
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalArrayAttribute(const string& name, const int32_t* intArray, const uint32_t size)
	{
		Datum& datum = AddAttribute(name, DatumType::INT32_T);
		datum.Clear();
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(intArray[i]);
		}
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalArrayAttribute(const string& name, const float_t* floatArray, const uint32_t size)
	{
		Datum& datum = AddAttribute(name, DatumType::FLOAT);
		datum.Clear();
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(floatArray[i]);
		}
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalArrayAttribute(const string& name, const std::string* stringArray, const uint32_t size)
	{
		Datum& datum = AddAttribute(name, DatumType::STRING);
		datum.Clear();
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(stringArray[i]);
		}
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalArrayAttribute(const string& name, const glm::vec4* vec4Array, const uint32_t size)
	{
		Datum& datum = AddAttribute(name, DatumType::GLM_VECTOR4);
		datum.Clear();
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(vec4Array[i]);
		}
		AddToPrescribedAtributesList(name);
	}

	void Attributed::AddInternalArrayAttribute(const string& name, const glm::mat4x4* mat4x4Array, const uint32_t size)
	{
		Datum& datum = AddAttribute(name, DatumType::GLM_MATRIX4X4);
		datum.Clear();
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(mat4x4Array[i]);
		}
		AddToPrescribedAtributesList(name);
	}

	Scope& Attributed::AddNestedScopeAttribute(const string& name)
	{
		Scope& scope = AppendScope(name);
		AddToPrescribedAtributesList(name);
		return scope;
	}

	void Attributed::AddNestedScopeAttribute(Scope& childScope, const string& name)
	{
		Adopt(childScope, name);
		AddToPrescribedAtributesList(name);
	}

	Datum& Attributed::AddEmptyNestedScopeAttribute(const string& name)
	{
		return AddAttribute(name, DatumType::TABLE);
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
		if (prescribedAttributes.Find(name) == prescribedAttributes.end())
		{
			prescribedAttributes.PushBack(name);
		}
	}
#pragma endregion
}