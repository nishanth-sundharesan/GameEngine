#pragma once
#include <cstdint>

class Foo
{
public:
	Foo();
	Foo(int32_t pData);
	Foo(const Foo& foo);
	~Foo();

	int32_t getIntegerData() const;
	int32_t* getIntegerPointer() const;

	void setIntegerDataAndPointer(const int32_t pData);	

	Foo& operator=(const Foo& rhs);
	bool operator==(const Foo& rhs) const;	
private:
	int32_t mIntData;
	int32_t* mIntPointer;
	void Clear();
};