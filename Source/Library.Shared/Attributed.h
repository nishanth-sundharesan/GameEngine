#pragma once
#include <cstdint>
#include "Scope.h"

namespace GameEngineLibrary
{
	class Attributed :Scope
	{
	public:
		Attributed();

		Attributed(const Attributed& rhs);

		Attributed& operator=(const Attributed& rhs);



		virtual ~Attributed();

		bool IsPrescribedAttribute(const std::string& name);

		bool IsAuxiliaryAttribute(const std::string& name);

		bool IsAttribute(const std::string& name);

		Datum& AppendAuxiliaryAttribute(const std::string& name);

		uint32_t AuxiliaryBegin();
	protected:

		void AddInternalAttribute(const std::string& name, const std::int32_t value);

		void AddInternalAttribute(const std::string& name, const std::float_t value);

		void AddInternalAttribute(const std::string& name, const std::string& value);

		void AddInternalAttribute(const std::string& name, const glm::vec4 value);

		void AddInternalAttribute(const std::string& name, const glm::mat4x4 value);

		void AddInternalAttribute(const std::string& name, const RTTI* value);


		void AddExternalAttribute(const std::string& name, const std::int32_t* pointerToData, const std::uint32_t size);

		void AddExternalAttribute(const std::string& name, const std::float_t* pointerToData, const std::uint32_t size);

		void AddExternalAttribute(const std::string& name, const std::string* pointerToData, const std::uint32_t size);

		void AddExternalAttribute(const std::string& name, const glm::vec4* pointerToData, const std::uint32_t size);

		void AddExternalAttribute(const std::string& name, const glm::mat4x4* pointerToData, const std::uint32_t size);

		void AddExternalAttribute(const std::string& name, const RTTI** pointerToData, const std::uint32_t size);
	private:
		Datum& AddAttribute(const std::string& name, const DatumType datumType);

		void AddToPrescribedAtributesList(const std::string& name);

		static Hashmap<uint64_t, Vector<string>> sPrescribedAttributes;

		static uint32_t sAttributedCount;

		RTTI_DECLARATIONS(Scope, RTTI);

		Datum tempDatum;
	};
}