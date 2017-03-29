#include "Pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "FooProduct.h"
#include "BarChildProduct.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;

namespace LibraryDesktopTest
{
	TEST_CLASS(FactoryTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
		}

		TEST_METHOD_CLEANUP(Cleanup)
		{
#pragma warning(suppress: 4101)															//Suppressing the warning "'endMemState': unreferenced local variable" and "'diffMemState': unreferenced local variable" in release mode
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
		}

		TEST_METHOD(TestFooProduct)
		{
			FooProductFactory fooProductFactory;
			FooProductFactory anotherFooProductFactory;

			RTTI* rttiFooProduct = Factory<RTTI>::Create("FooProduct");
			FooProduct* fooProduct = rttiFooProduct->As<FooProduct>();

			Assert::IsFalse(fooProduct == nullptr);
			Assert::AreEqual(fooProduct->GetIntegerData(), 100);
			Assert::IsTrue(fooProductFactory.ClassName() == "FooProduct");

			fooProduct->SetIntegerData(200);
			Assert::AreEqual(fooProduct->GetIntegerData(), 200);

			Factory<RTTI>* anotherBarProduct = Factory<RTTI>::Find("FooProduct");
			Assert::IsFalse(anotherBarProduct == nullptr);
			Assert::IsTrue(anotherBarProduct->ClassName() == "FooProduct");
			Assert::IsTrue(anotherBarProduct == &fooProductFactory);

			Assert::IsTrue(Factory<RTTI>::Find("BarProduct") == nullptr);
			Assert::IsTrue(Factory<RTTI>::Create("BarProduct") == nullptr);

			Hashmap<string, Factory<RTTI>*>::Iterator hashmapIterator = fooProductFactory.begin();
			Assert::IsTrue(hashmapIterator == Factory<RTTI>::begin());
			Assert::IsTrue(++hashmapIterator == Factory<RTTI>::end());

			int32_t mSize = 0;
			for (auto& iterator : fooProductFactory)
			{
				++mSize;
				iterator;
			}
			Assert::AreEqual(mSize, 1);

			delete fooProduct;
		}

		TEST_METHOD(TestFooProductRTTI)
		{
			FooProductFactory fooProductFactory;
			FooProductFactory fooProductFactoryOne;

			RTTI* rttiFooProduct = Factory<RTTI>::Create("FooProduct");
			FooProduct* fooProduct = rttiFooProduct->As<FooProduct>();

			Assert::IsTrue(fooProduct->Equals(rttiFooProduct));
			Assert::IsTrue(fooProduct->Is("FooProduct"));
			Assert::IsFalse(fooProduct->Is("FooProd"));

			rttiFooProduct = fooProduct->QueryInterface(fooProduct->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(rttiFooProduct) == fooProduct);
			Assert::IsTrue(fooProduct->Equals(rttiFooProduct));
			Assert::IsTrue(fooProduct->QueryInterface(fooProduct->TypeIdClass() + 30) == nullptr);

			Assert::IsTrue(fooProduct->Is(fooProduct->TypeIdClass()));
			Assert::IsTrue(fooProduct->Is(FooProduct::TypeIdClass()));
			Assert::IsFalse(fooProduct->Is(FooProduct::TypeIdClass() + 20));

			Assert::IsTrue(fooProduct->TypeIdInstance() == FooProduct::TypeIdClass());

			delete fooProduct;
		}

		TEST_METHOD(TestBarProduct)
		{
			BarChildProductFactory barChildProductFactory;
			BarChildProductFactory anotherBarChildProductFactory;

			BarProduct* barProduct = Factory<BarProduct>::Create("BarChildProduct");
			BarChildProduct* barChildProduct = static_cast<BarChildProduct*>(barProduct);

			Assert::IsFalse(barChildProduct == nullptr);
			Assert::AreEqual(barChildProduct->GetIntegerData(), 20);
			Assert::IsTrue(barChildProductFactory.ClassName() == "BarChildProduct");

			Factory<BarProduct>* anotherBarProduct = Factory<BarProduct>::Find("BarChildProduct");
			Assert::IsFalse(anotherBarProduct == nullptr);
			Assert::IsTrue(anotherBarProduct->ClassName() == "BarChildProduct");
			Assert::IsTrue(anotherBarProduct == &barChildProductFactory);

			Assert::IsTrue(Factory<BarProduct>::Find("BarProduct") == nullptr);
			Assert::IsTrue(Factory<BarProduct>::Create("BarProduct") == nullptr);

			// *START* Re-initializing RTTI factory
			FooProductFactory fooProductFactory;
			RTTI* rttiFooProduct = Factory<RTTI>::Create("FooProduct");

			Assert::IsTrue(Factory<BarProduct>::Find("FooProduct") == nullptr);
			Assert::IsTrue(Factory<BarProduct>::Create("FooProduct") == nullptr);

			Assert::IsTrue(Factory<RTTI>::Find("BarChildProduct") == nullptr);
			Assert::IsTrue(Factory<RTTI>::Create("BarChildProduct") == nullptr);

			delete rttiFooProduct;
			// *END*

			barChildProduct->SetIntegerData(350);
			Assert::AreEqual(barChildProduct->GetIntegerData(), 350);

			Hashmap<string, Factory<BarProduct>*>::Iterator hashmapIterator = barChildProductFactory.begin();
			Assert::IsTrue(hashmapIterator == Factory<BarProduct>::begin());
			Assert::IsTrue(++hashmapIterator == Factory<BarProduct>::end());

			int32_t mSize = 0;
			for (auto& iterator : barChildProductFactory)
			{
				++mSize;
				iterator;
			}
			Assert::AreEqual(mSize, 1);

			delete barChildProduct;
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState FactoryTest::sStartMemState;
}