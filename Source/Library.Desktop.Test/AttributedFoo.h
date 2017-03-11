#pragma once
#include <cstdint>
#include "Attributed.h"

namespace UnitTestSupportClasses
{
	class AttributedFoo :public GameEngineLibrary::Attributed
	{
	public:
		AttributedFoo();

		AttributedFoo(const AttributedFoo& rhs);

		AttributedFoo(AttributedFoo&& rhs);

		AttributedFoo& operator=(const AttributedFoo& rhs);

		AttributedFoo& operator=(AttributedFoo&& rhs);

		virtual ~AttributedFoo();
	public:
		int32_t mInternalInteger;
		float_t mInternalFloat;
		std::string mInternalString;
		glm::vec4 mInternalVector4;
		glm::mat4x4 mInternalMatrix4x4;

		int32_t* mInternalIntegerArray;
		float_t* mInternalFloatArray;
		std::string* mInternalStringArray;
		glm::vec4* mInternalVector4Array;
		glm::mat4x4* mInternalMatrix4x4Array;

		int32_t mExternalInteger;
		float_t mExternalFloat;
		std::string mExternalString;
		glm::vec4 mExternalVector4;
		glm::mat4x4 mExternalMatrix4x4;

		int32_t* mExternalIntegerArray;
		float_t* mExternalFloatArray;
		std::string* mExternalStringArray;
		glm::vec4* mExternalVector4Array;
		glm::mat4x4* mExternalMatrix4x4Array;
				
		Scope* scopeToAdopt;
	protected:
		virtual void InitializeSignatures();		
	private:
		const uint32_t mArraySize = 2;
		void Clear();
		void InitializeInternalSignatures();
		void InitializeExternalSignatures();
		void AssignValuesToMembers();
		void InstantiateArrays(const std::uint32_t size);
		void PerformDeepCopy(const AttributedFoo& rhs);
		void PerformShallowCopyAndDefaultValues(AttributedFoo& rhs);
	public:
		RTTI_DECLARATIONS(AttributedFoo, GameEngineLibrary::Attributed);
	};
}
