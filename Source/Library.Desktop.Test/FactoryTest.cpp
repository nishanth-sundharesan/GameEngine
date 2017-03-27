#include "Pch.h"
#include "CppUnitTest.h"
#include "Factory.h"
#include "FooProduct.h"

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

		TEST_METHOD(TestMethod1)
		{
			FooProductFactory fooProductFactory;
			RTTI* rttiProduct = Factory<RTTI>::Create("FooProduct");
			FooProduct* fooProduct = rttiProduct->As<FooProduct>();

			Assert::IsFalse(fooProduct == nullptr);
			Assert::AreEqual(fooProduct->GetIntegerData(), 100);
			Assert::IsTrue(fooProductFactory.ClassName() == "FooProduct");

			delete fooProduct;
		}

	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState FactoryTest::sStartMemState;
}