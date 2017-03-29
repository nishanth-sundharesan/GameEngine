#pragma once
#include "RTTI.h"
#include "Factory.h"

namespace UnitTestSupportClasses
{
	class FooProduct :public GameEngineLibrary::RTTI
	{
	public:
		FooProduct();

		FooProduct(const FooProduct&) = default;

		FooProduct& operator=(const FooProduct&) = default;

		virtual ~FooProduct() = default;

		void SetIntegerData(std::int32_t integerData);

		std::int32_t GetIntegerData();

	private:
		std::int32_t mIntegerData;

	public:
		RTTI_DECLARATIONS(FooProduct, RTTI);
	};

	ConcreteFactory(GameEngineLibrary::RTTI, FooProduct);
}