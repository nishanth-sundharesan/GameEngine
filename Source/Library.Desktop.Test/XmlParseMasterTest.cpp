#include "Pch.h"
#include "CppUnitTest.h"
#include "XmlParseMaster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;

namespace LibraryDesktopTest
{		
	TEST_CLASS(XmlParseMasterTest)
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

		TEST_METHOD(TestXmlParseMaster)
		{
			XmlParseMaster::SharedData *sharedData = new XmlParseMaster::SharedData();

			XmlParseMaster xmlParseMaster(sharedData);

			delete sharedData;
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState XmlParseMasterTest::sStartMemState;
}