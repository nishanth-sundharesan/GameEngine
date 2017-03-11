#pragma once
#include <cstdint>
#include "AttributedFoo.h"

namespace UnitTestSupportClasses
{
	class AttributedFooChild :public AttributedFoo
	{
	public:
		AttributedFooChild();

		AttributedFooChild(const AttributedFooChild& rhs);

		AttributedFooChild(AttributedFooChild&& rhs);

		AttributedFooChild& operator=(const AttributedFooChild& rhs);

		AttributedFooChild& operator=(AttributedFooChild&& rhs);

		virtual ~AttributedFooChild();

		int32_t mInternalIntegerChild;
		float_t mInternalFloatChild;

		int32_t* mInternalIntegerArrayChild;
		float_t* mInternalFloatArrayChild;

		int32_t mExternalIntegerChild;
		float_t mExternalFloatChild;

		int32_t* mExternalIntegerArrayChild;
		float_t* mExternalFloatArrayChild;
	protected:
		virtual void InitializeSignatures() override;
	private:
		const uint32_t mArraySize = 2;
		void InstantiateArrays(const std::uint32_t size);
		void AssignValuesToMembers();
		void InitializeInternalSignatures();
		void InitializeExternalSignatures();
		void PerformDeepCopy(const AttributedFooChild& rhs);
		void Clear();
		void PerformShallowCopyAndDefaultValues(AttributedFooChild& rhs);
	public:
		RTTI_DECLARATIONS(AttributedFooChild, AttributedFoo);
	};
}
