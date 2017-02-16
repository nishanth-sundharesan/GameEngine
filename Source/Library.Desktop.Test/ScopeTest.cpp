#include "Pch.h"
#include "CppUnitTest.h"
#include "Scope.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace std;

namespace LibraryDesktopTest
{		
	TEST_CLASS(ScopeTest)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			Scope s;
		}
	};
}