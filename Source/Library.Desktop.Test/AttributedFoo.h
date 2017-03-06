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

		AttributedFoo& operator=(const AttributedFoo& rhs);

		~AttributedFoo();
	private:
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

		const uint32_t mSize = 5;
		void InitializeSignatures();
	public:
		RTTI_DECLARATIONS(AttributedFoo, GameEngineLibrary::Attributed);
	};
}
