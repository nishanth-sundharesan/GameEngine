#pragma once

namespace UnitTestSupportClasses
{
	class BarProduct
	{
	public:
		BarProduct();

		BarProduct(const BarProduct&) = default;

		BarProduct& operator=(const BarProduct&) = default;

		virtual ~BarProduct() = default;

		virtual void SetIntegerData(std::int32_t integerData) = 0;

		virtual std::int32_t GetIntegerData() = 0;

	protected:
		std::int32_t mIntegerData;
	};
}