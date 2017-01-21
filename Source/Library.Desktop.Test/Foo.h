#pragma once
#include <cstdint>

namespace UnitTestSupportClasses
{
	class Foo
	{
	public:
		Foo();
		Foo(int32_t data);
		Foo(const Foo& foo);
		~Foo();

		int32_t GetIntegerData() const;
		int32_t* GetIntegerPointer() const;

		void SetIntegerDataAndPointer(const int32_t data);

		Foo& operator=(const Foo& rhs);
		bool operator==(const Foo& rhs) const;
	private:
		int32_t mIntData;
		int32_t* mIntPointer;
		void Clear();
	};
}