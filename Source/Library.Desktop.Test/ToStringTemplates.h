#pragma once
#include "Foo.h"
#include "CppUnitTest.h"

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<>
			inline std::wstring ToString<UnitTestSupportClasses::Foo>(const UnitTestSupportClasses::Foo& t)
			{
				RETURN_WIDE_STRING(t.GetIntegerData());
			}		
		}
	}
}