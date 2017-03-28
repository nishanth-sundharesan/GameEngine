#pragma once
#include "BarProduct.h"
#include "Factory.h"

namespace UnitTestSupportClasses
{
	class BarChildProduct : public BarProduct
	{
	public:
		BarChildProduct() = default;

		BarChildProduct(const BarChildProduct&) = default;

		BarChildProduct& operator=(const BarChildProduct&) = default;

		virtual ~BarChildProduct() = default;

		virtual void SetIntegerData(std::int32_t integerData) override;

		virtual std::int32_t GetIntegerData() override;
	};

	ConcreteFactory(BarProduct, BarChildProduct);
}