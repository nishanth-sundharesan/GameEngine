#include "Pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "Hashmap.h"
#include "ToStringTemplates.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;

namespace LibraryDesktopTest
{
	class FooHashFunctor
	{
	public:
		FooHashFunctor() = default;
		std::uint32_t FooHashFunctor::operator()(const Foo& key) const
		{
			uint32_t hash = 0;

			int32_t firstHalfOfKey = key.GetIntegerData();
			uint8_t* byteArray = reinterpret_cast<uint8_t*>(&firstHalfOfKey);
			for (uint32_t i = 0; i < sizeof(firstHalfOfKey); ++i)
			{
				hash += 65 * byteArray[i];
			}

			int32_t* secondHalfOfKey = key.GetIntegerPointer();
			byteArray = reinterpret_cast<uint8_t*>(secondHalfOfKey);
			for (uint32_t i = 0; i < sizeof(secondHalfOfKey); ++i)
			{
				hash += 65 * byteArray[i];
			}
			return hash;
		}
	};

	TEST_CLASS(HashmapTest)
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

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapConstructorWithSize(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			secondPair;
			thirdPair;

			uint32_t currentSize = 0;
			currentSize;
			Assert::IsTrue(hashmap.Find(firstPair.first) == hashmap.end());

			Assert::IsFalse(hashmap.Remove(firstPair.first));

			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::IsTrue(hashmap.begin() == hashmap.end());

			Assert::IsFalse(hashmap.ContainsKey(firstPair.first));

			Assert::ExpectException<exception>([&] { *(hashmap.begin()); });

			Assert::ExpectException<exception>([&] { (hashmap.begin())->first; });

			const Hashmap<TKey, TValue, HashFunctor> constHashMap = hashmap;
			Assert::ExpectException<exception>([&] { constHashMap[firstPair.first]; });
		}

		TEST_METHOD(HashmapConstructorWithSize)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapConstructorWithSize(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapConstructorWithSize(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapConstructorWithSize(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapConstructorWithSize(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapConstructorWithSize(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapInsert(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			uint32_t currentSize = 0;

			/************************************************************************/
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(firstPair);
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(secondPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(thirdPair);
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Remove(secondPair.first);
			--currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			/************************************************************************/

			hashmap.Clear();
			currentSize = 0;
			Assert::AreEqual(hashmap.Size(), currentSize);

			/**********Rechecking the same code after clearing the hashmap***********/
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(firstPair);
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(secondPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Insert(thirdPair);
			Assert::AreEqual(hashmap.Size(), currentSize);

			hashmap.Remove(secondPair.first);
			--currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			/************************************************************************/
		}

		TEST_METHOD(HashmapInsert)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapInsert(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapInsert(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapInsert(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapInsert(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapInsert(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapFind(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;
			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			iterator = hashmap.Find(firstPair.first);
			Assert::IsTrue((*iterator).second == firstPair.second);

			hashmap.Insert(secondPair);
			iterator = hashmap.Find(secondPair.first);
			Assert::IsTrue((*iterator).second == secondPair.second);

			hashmap.Insert(thirdPair);
			iterator = hashmap.Find(thirdPair.first);
			Assert::IsTrue((*iterator).second == thirdPair.second);

			//Finding all the inserted items again
			iterator = hashmap.Find(firstPair.first);
			Assert::IsTrue((*iterator).second == firstPair.second);

			iterator = hashmap.Find(secondPair.first);
			Assert::IsTrue((*iterator).second == secondPair.second);

			iterator = hashmap.Find(thirdPair.first);
			Assert::IsTrue((*iterator).second == thirdPair.second);

			//Clearing the hashmap
			hashmap.Clear();
			iterator = hashmap.Find(firstPair.first);
			Assert::IsTrue(iterator == hashmap.end());

			iterator = hashmap.Find(secondPair.first);
			Assert::IsTrue(iterator == hashmap.end());

			iterator = hashmap.Find(thirdPair.first);
			Assert::IsTrue(iterator == hashmap.end());

			//Inserting all the 3 items again
			hashmap.Insert(firstPair);
			iterator = hashmap.Find(firstPair.first);
			Assert::IsTrue((*iterator).second == firstPair.second);

			hashmap.Insert(secondPair);
			iterator = hashmap.Find(secondPair.first);
			Assert::IsTrue((*iterator).second == secondPair.second);

			hashmap.Insert(thirdPair);
			iterator = hashmap.Find(thirdPair.first);
			Assert::IsTrue((*iterator).second == thirdPair.second);

			//Checking for the first element again
			iterator = hashmap.Find(firstPair.first);
			Assert::IsTrue((*iterator).second == firstPair.second);
		}

		TEST_METHOD(HashmapFind)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapFind(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapFind(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapFind(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapFind(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapFind(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapSubscriptOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{			
			secondPair;
			thirdPair;

			uint32_t currentSize = 0;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;
			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap[firstPair.first] == firstPair.second);

			hashmap.Insert(secondPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap[secondPair.first] == secondPair.second);

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap[thirdPair.first] == thirdPair.second);

			//Finding all the inserted items again
			Assert::IsTrue(hashmap[firstPair.first] == firstPair.second);
			Assert::IsTrue(hashmap[secondPair.first] == secondPair.second);
			Assert::IsTrue(hashmap[thirdPair.first] == thirdPair.second);

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;

			//Inserting all the 3 items again
			TValue subscriptInsertedValue = hashmap[firstPair.first];
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsFalse(subscriptInsertedValue == firstPair.second);

			subscriptInsertedValue = hashmap[secondPair.first];
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsFalse(subscriptInsertedValue == secondPair.second);

			subscriptInsertedValue = hashmap[thirdPair.first];
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsFalse(subscriptInsertedValue == thirdPair.second);

			//Checking for the first element again
			subscriptInsertedValue = hashmap[firstPair.first];
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsFalse(subscriptInsertedValue == firstPair.second);
		}

		TEST_METHOD(HashmapSubscriptOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapSubscriptOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapSubscriptOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapSubscriptOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapSubscriptOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapSubscriptOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapConstSubscriptOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			uint32_t currentSize = 0;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;
			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			++currentSize;

			const Hashmap<TKey, TValue, HashFunctor> constHashMapOne = hashmap;
			Assert::AreEqual(constHashMapOne.Size(), currentSize);
			Assert::IsTrue(constHashMapOne[firstPair.first] == firstPair.second);

			hashmap.Insert(secondPair);
			++currentSize;
			const Hashmap<TKey, TValue, HashFunctor> constHashMapTwo = hashmap;
			Assert::AreEqual(constHashMapTwo.Size(), currentSize);
			Assert::IsTrue(constHashMapTwo[secondPair.first] == secondPair.second);

			hashmap.Insert(thirdPair);
			++currentSize;
			const Hashmap<TKey, TValue, HashFunctor> constHashMapThree = hashmap;
			Assert::AreEqual(constHashMapThree.Size(), currentSize);
			Assert::IsTrue(constHashMapThree[thirdPair.first] == thirdPair.second);

			//Finding all the inserted items again
			Assert::IsTrue(constHashMapThree[firstPair.first] == firstPair.second);
			Assert::IsTrue(constHashMapThree[secondPair.first] == secondPair.second);
			Assert::IsTrue(constHashMapThree[thirdPair.first] == thirdPair.second);

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;

			const Hashmap<TKey, TValue, HashFunctor> constHashMapFour = hashmap;
			Assert::ExpectException<exception>([&] { constHashMapFour[firstPair.first]; });
			Assert::AreEqual(constHashMapFour.Size(), currentSize);

			Assert::ExpectException<exception>([&] { constHashMapFour[secondPair.first]; });
			Assert::AreEqual(constHashMapFour.Size(), currentSize);

			Assert::ExpectException<exception>([&] { constHashMapFour[thirdPair.first]; });
			Assert::AreEqual(constHashMapFour.Size(), currentSize);

			//Checking for the first element again
			TValue subscriptInsertedValue = hashmap[firstPair.first];
			++currentSize;
			const Hashmap<TKey, TValue, HashFunctor> constHashMapFive = hashmap;
			Assert::AreEqual(constHashMapFive.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsTrue(subscriptInsertedValue == constHashMapFive[firstPair.first]);
		}

		TEST_METHOD(HashmapConstSubscriptOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapConstSubscriptOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapConstSubscriptOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapConstSubscriptOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapConstSubscriptOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapConstSubscriptOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapAt(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			uint32_t currentSize = 0;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;
			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			++currentSize;

			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap.At(firstPair.first) == firstPair.second);

			hashmap.Insert(secondPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap.At(secondPair.first) == secondPair.second);

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(hashmap.At(thirdPair.first) == thirdPair.second);

			//Finding all the inserted items again
			Assert::IsTrue(hashmap.At(firstPair.first) == firstPair.second);
			Assert::IsTrue(hashmap.At(secondPair.first) == secondPair.second);
			Assert::IsTrue(hashmap.At(thirdPair.first) == thirdPair.second);

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;

			Assert::ExpectException<exception>([&] { hashmap.At(firstPair.first); });
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::ExpectException<exception>([&] { hashmap.At(secondPair.first); });
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::ExpectException<exception>([&] { hashmap.At(thirdPair.first); });
			Assert::AreEqual(hashmap.Size(), currentSize);

			//Checking for the first element again			
			TValue subscriptInsertedValue = hashmap[firstPair.first];
			++currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsTrue(subscriptInsertedValue == TValue());
			Assert::IsTrue(subscriptInsertedValue == hashmap.At(firstPair.first));
		}

		TEST_METHOD(HashmapAt)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapAt(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapAt(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapAt(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapAt(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapAt(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapRemove(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			//Checking Remove() before inserting anything
			uint32_t currentSize = 0;
			Assert::IsFalse(hashmap.Remove(firstPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::IsFalse(hashmap.Remove(secondPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::IsFalse(hashmap.Remove(thirdPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);

			//Inserting firstpair. Removing firstpair
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::IsTrue(hashmap.Remove(firstPair.first));
			--currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			//Inserting firstpair, secondpair, thirdpair. Removing secondpair, thirdpair
			hashmap.Insert(firstPair);
			++currentSize;
			hashmap.Insert(secondPair);
			++currentSize;
			Assert::IsTrue(hashmap.Remove(secondPair.first));
			--currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);
			Assert::IsFalse(hashmap.Remove(secondPair.first));
			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::IsTrue(hashmap.Remove(thirdPair.first));
			--currentSize;
			Assert::AreEqual(hashmap.Size(), currentSize);

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;
			Assert::IsFalse(hashmap.Remove(firstPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::IsFalse(hashmap.Remove(secondPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);

			Assert::IsFalse(hashmap.Remove(thirdPair.first));
			Assert::AreEqual(hashmap.Size(), currentSize);
		}

		TEST_METHOD(HashmapRemove)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapRemove(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapRemove(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapRemove(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapRemove(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapRemove(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapSize(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			uint32_t currentSize = 0;
			Assert::AreEqual(currentSize, hashmap.Size());

			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap[secondPair.first];
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;
			Assert::AreEqual(currentSize, hashmap.Size());

			//Inserting the first item and removing it
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap.Remove(firstPair.first);
			--currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap[secondPair.first];
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap.Insert(thirdPair);
			++currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the second item and then the third item
			hashmap.Remove(secondPair.first);
			--currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());

			hashmap.Remove(thirdPair.first);
			--currentSize;
			Assert::AreEqual(currentSize, hashmap.Size());
		}

		TEST_METHOD(HashmapSize)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapSize(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapSize(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapSize(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapSize(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapSize(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapContainsKey(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;
			//Inserting all the 3 items
			hashmap.Insert(firstPair);
			Assert::IsTrue(hashmap.ContainsKey(firstPair.first));

			hashmap.Insert(secondPair);
			Assert::IsTrue(hashmap.ContainsKey(secondPair.first));

			hashmap.Insert(thirdPair);
			Assert::IsTrue(hashmap.ContainsKey(thirdPair.first));

			//Finding all the inserted items again
			Assert::IsTrue(hashmap.ContainsKey(firstPair.first));
			Assert::IsTrue(hashmap.ContainsKey(secondPair.first));
			Assert::IsTrue(hashmap.ContainsKey(thirdPair.first));

			//Clearing the hashmap
			hashmap.Clear();
			Assert::IsFalse(hashmap.ContainsKey(firstPair.first));
			Assert::IsFalse(hashmap.ContainsKey(secondPair.first));
			Assert::IsFalse(hashmap.ContainsKey(thirdPair.first));

			//Inserting all the 3 items again
			hashmap.Insert(firstPair);
			Assert::IsTrue(hashmap.ContainsKey(firstPair.first));

			hashmap.Insert(secondPair);
			Assert::IsTrue(hashmap.ContainsKey(secondPair.first));

			hashmap.Insert(thirdPair);
			Assert::IsTrue(hashmap.ContainsKey(thirdPair.first));

			//Checking for the first element again			
			Assert::IsTrue(hashmap.ContainsKey(firstPair.first));
		}

		TEST_METHOD(HashmapContainsKey)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapContainsKey(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapContainsKey(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapContainsKey(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapContainsKey(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapContainsKey(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void Hashmapbegin(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			//Checking while the hashmap is empty
			Assert::IsTrue(hashmap.begin() == hashmap.end());

			//Inserting the firstpair and the secondpair
			hashmap.Insert(firstPair);
			Assert::IsFalse(hashmap.begin() == hashmap.end());
			Assert::AreEqual((*hashmap.begin()).second, firstPair.second);

			hashmap.Insert(secondPair);
			Assert::IsFalse(hashmap.begin() == hashmap.end());

			//Removing the firstpair and the secondpair
			hashmap.Remove(secondPair.first);
			Assert::IsFalse(hashmap.begin() == hashmap.end());
			hashmap.Remove(firstPair.first);
			Assert::IsTrue(hashmap.begin() == hashmap.end());

			//Inserting the secondpair, firstpair and the thirdpair
			hashmap.Insert(secondPair);
			Assert::IsFalse(hashmap.begin() == hashmap.end());
			Assert::AreEqual((*hashmap.begin()).second, secondPair.second);

			hashmap.Insert(firstPair);
			hashmap.Insert(thirdPair);

			//Clearing the hashmap
			hashmap.Clear();
			Assert::IsTrue(hashmap.begin() == hashmap.end());
		}

		TEST_METHOD(Hashmapbegin)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			Hashmapbegin(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			Hashmapbegin(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			Hashmapbegin(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			Hashmapbegin(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			Hashmapbegin(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void Hashmapend(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			//Checking the empty hashmap
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			//Inserting items and checking for the results
			hashmap.Insert(firstPair);
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			hashmap.Insert(secondPair);
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			hashmap.Insert(thirdPair);
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			//Clearing the hashmap
			hashmap.Clear();
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			//Inserting an item and checking
			hashmap.Insert(firstPair);
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });

			//Removing an item and checking
			hashmap.Remove(firstPair.first);
			Assert::ExpectException<out_of_range>([&] { *(hashmap.end()); });
		}

		TEST_METHOD(Hashmapend)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			Hashmapend(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			Hashmapend(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			Hashmapend(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			Hashmapend(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			Hashmapend(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapClear(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			uint32_t currentSize = 0;
			Assert::IsTrue(currentSize == hashmap.Size());

			hashmap.Clear();
			Assert::IsTrue(currentSize == hashmap.Size());

			//Inserting one item and clearing
			hashmap.Insert(firstPair);
			++currentSize;
			hashmap.Clear();
			currentSize = 0;
			Assert::IsTrue(currentSize == hashmap.Size());
			Assert::IsFalse(hashmap.ContainsKey(firstPair.first));

			//Inserting 3 items and clearing
			hashmap.Insert(firstPair);
			++currentSize;
			hashmap.Insert(secondPair);
			++currentSize;
			hashmap.Insert(thirdPair);
			++currentSize;

			hashmap.Clear();
			currentSize = 0;
			Assert::IsTrue(currentSize == hashmap.Size());
			Assert::IsFalse(hashmap.ContainsKey(firstPair.first));
			Assert::IsFalse(hashmap.ContainsKey(secondPair.first));
			Assert::IsFalse(hashmap.ContainsKey(thirdPair.first));
		}

		TEST_METHOD(HashmapClear)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapClear(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapClear(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapClear(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapClear(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapClear(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorPrefixIncrement(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;

			Assert::ExpectException<exception>([&] { ++iterator; });

			/************************************************************************/
			//Checking with the empty Hashmap
			iterator = hashmap.begin();
			Assert::ExpectException<exception>([&] { ++iterator; });

			iterator = hashmap.end();
			Assert::ExpectException<exception>([&] { ++iterator; });
			/************************************************************************/

			/************************************************************************/
			//Inserting the first item
			hashmap.Insert(firstPair);
			iterator = hashmap.begin();
			Assert::IsTrue((*iterator).second == firstPair.second);

			++iterator;
			Assert::IsTrue(iterator == hashmap.end());

			//Inserting the second and the third item
			hashmap.Insert(secondPair);
			hashmap.Insert(thirdPair);

			uint32_t currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Clearing the hashmap
			hashmap.Clear();
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());
			/************************************************************************/

			/************************************************************************/
			//Inserting all the 3 items again
			hashmap.Insert(firstPair);
			hashmap[secondPair.first];
			hashmap.Insert(thirdPair);

			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the second item
			hashmap.Remove(secondPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the third item
			hashmap.Remove(thirdPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the first item
			hashmap.Remove(firstPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());
			/************************************************************************/

			/************************************************************************/
			//Inserting all the 3 items
			hashmap[firstPair.first];
			hashmap.Insert(secondPair);
			hashmap[thirdPair.first];

			//Removing the first item
			hashmap.Remove(firstPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the second item
			hashmap.Remove(secondPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());

			//Removing the third item
			hashmap.Remove(thirdPair.first);
			currentSize = 0;
			for (auto& value : hashmap)
			{
				value;
				++currentSize;
			}
			Assert::AreEqual(currentSize, hashmap.Size());
			Assert::AreEqual(0u, hashmap.Size());
			/************************************************************************/
		}

		TEST_METHOD(HashmapIteratorPrefixIncrement)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorPrefixIncrement(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorPrefixIncrement(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorPrefixIncrement(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorPrefixIncrement(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorPrefixIncrement(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorPostfixIncrement(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;

			secondPair;
			thirdPair;
			Assert::ExpectException<exception>([&] { iterator++; });

			/************************************************************************/
			//Checking with the empty hashmap
			iterator = hashmap.begin();
			Assert::ExpectException<exception>([&] { iterator++; });

			iterator = hashmap.end();
			Assert::ExpectException<exception>([&] { iterator++; });
			/************************************************************************/

			/************************************************************************/
			//Inserting the first item
			hashmap.Insert(firstPair);
			iterator = hashmap.begin();
			Assert::IsTrue((*iterator).second == firstPair.second);

			iterator++;
			Assert::IsTrue(iterator == hashmap.end());
			/************************************************************************/
		}

		TEST_METHOD(HashmapIteratorPostfixIncrement)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorPostfixIncrement(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorPostfixIncrement(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorPostfixIncrement(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorPostfixIncrement(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorPostfixIncrement(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorDeferenceOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;

			Assert::ExpectException<exception>([&] { *iterator; });

			//Checking with the empty Hashmap
			iterator = hashmap.begin();
			Assert::ExpectException<exception>([&] { *iterator; });

			iterator = hashmap.end();
			Assert::ExpectException<exception>([&] { *iterator; });

			//Inserting all the items
			hashmap.Insert(firstPair);
			iterator = hashmap.begin();

			Assert::AreEqual((*iterator).first, firstPair.first);

			hashmap.Insert(secondPair);
			hashmap.Insert(thirdPair);

			bool areAllKeysPresent = true;
			for (auto& value : hashmap)
			{
				if (!(value.second == firstPair.second || value.second == secondPair.second || value.second == thirdPair.second))
				{
					areAllKeysPresent = false;
					break;
				}
			}
			Assert::IsTrue(areAllKeysPresent);
		}

		TEST_METHOD(HashmapIteratorDeferenceOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorDeferenceOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorDeferenceOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorDeferenceOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorDeferenceOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorDeferenceOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorConstDeferenceOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			//Checking with the empty Hashmap
			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorOne = hashmap.begin();
			Assert::ExpectException<exception>([&] { *iteratorOne; });

			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorTwo = hashmap.end();
			Assert::ExpectException<exception>([&] { *iteratorTwo; });

			//Inserting all the items
			hashmap.Insert(firstPair);
			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorThree = hashmap.begin();

			Assert::AreEqual((*iteratorThree).first, firstPair.first);

			hashmap.Insert(secondPair);
			hashmap.Insert(thirdPair);

			bool areAllKeysPresent = true;
			for (const auto& value : hashmap)
			{
				if (!(value.second == firstPair.second || value.second == secondPair.second || value.second == thirdPair.second))
				{
					areAllKeysPresent = false;
					break;
				}
			}
			Assert::IsTrue(areAllKeysPresent);
		}

		TEST_METHOD(HashmapIteratorConstDeferenceOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorConstDeferenceOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorConstDeferenceOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorConstDeferenceOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorConstDeferenceOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorConstDeferenceOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorArrowOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			firstPair;
			secondPair;
			thirdPair;			

			Hashmap<TKey, TValue, HashFunctor>::Iterator iterator;

			//Checking with the empty Hashmap
			iterator = hashmap.begin();
			Assert::ExpectException<exception>([&] { iterator->first; });

			iterator = hashmap.end();
			Assert::ExpectException<exception>([&] { iterator->first; });

			hashmap.Insert(firstPair);
			hashmap.Insert(secondPair);
			hashmap.Insert(thirdPair);

			iterator = hashmap.begin();
			bool isFound = false;
			if (iterator->second == firstPair.second || iterator->second == secondPair.second || iterator->second == thirdPair.second)
			{
				isFound = true;
			}
			Assert::IsTrue(isFound);
		}

		TEST_METHOD(HashmapIteratorArrowOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorArrowOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorArrowOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorArrowOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorArrowOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorArrowOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorConstArrowOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			firstPair;
			secondPair;
			thirdPair;

			//Checking with the empty Hashmap
			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorOne = hashmap.begin();
			Assert::ExpectException<exception>([&] { iteratorOne->first; });

			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorTwo = hashmap.end();
			Assert::ExpectException<exception>([&] { iteratorTwo->first; });

			hashmap.Insert(firstPair);
			hashmap.Insert(secondPair);
			hashmap.Insert(thirdPair);

			const Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorThree = hashmap.begin();
			bool isFound = false;
			if (iteratorThree->second == firstPair.second || iteratorThree->second == secondPair.second || iteratorThree->second == thirdPair.second)
			{
				isFound = true;
			}
			Assert::IsTrue(isFound);
		}

		TEST_METHOD(HashmapIteratorConstArrowOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorConstArrowOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorConstArrowOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorConstArrowOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorConstArrowOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorConstArrowOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorEqualityOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			secondPair;
			thirdPair;

			Hashmap<TKey, TValue, HashFunctor> hashmapTemp;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorOne;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorTwo;

			Assert::IsTrue(iteratorOne == iteratorTwo);

			Assert::IsFalse(iteratorOne == hashmap.begin());
			Assert::IsFalse(iteratorOne == hashmap.end());

			iteratorTwo = hashmapTemp.begin();
			Assert::IsFalse(iteratorOne == iteratorTwo);

			iteratorOne = hashmap.begin();
			Assert::IsTrue(iteratorOne == hashmap.begin());

			Assert::IsTrue(iteratorOne == hashmap.end());

			//Inserting values in two different hash maps and checking if they are true
			hashmap.Insert(firstPair);
			iteratorOne = hashmap.begin();

			hashmapTemp.Insert(firstPair);
			iteratorTwo = hashmapTemp.begin();

			Assert::IsFalse(iteratorOne == iteratorTwo);
			Assert::IsFalse(hashmap.end() == hashmapTemp.end());
		}

		TEST_METHOD(HashmapIteratorEqualityOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorEqualityOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorEqualityOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorEqualityOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorEqualityOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorEqualityOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}

		template <class TKey, class TValue, typename HashFunctor>
		void HashmapIteratorInEqualityOperator(Hashmap<TKey, TValue, HashFunctor>& hashmap, pair<TKey, TValue>& firstPair, pair<TKey, TValue>& secondPair, pair<TKey, TValue>& thirdPair)
		{
			secondPair;
			thirdPair;

			Hashmap<TKey, TValue, HashFunctor> hashmapTemp;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorOne;
			Hashmap<TKey, TValue, HashFunctor>::Iterator iteratorTwo;

			Assert::IsFalse(iteratorOne != iteratorTwo);

			Assert::IsTrue(iteratorOne != hashmap.begin());
			Assert::IsTrue(iteratorOne != hashmap.end());

			iteratorTwo = hashmapTemp.begin();
			Assert::IsTrue(iteratorOne != iteratorTwo);

			iteratorOne = hashmap.begin();
			Assert::IsFalse(iteratorOne != hashmap.begin());

			Assert::IsFalse(iteratorOne != hashmap.end());

			//Inserting values in two different hash maps and checking if they are true
			hashmap.Insert(firstPair);
			iteratorOne = hashmap.begin();

			hashmapTemp.Insert(firstPair);
			iteratorTwo = hashmapTemp.begin();

			Assert::IsTrue(iteratorOne != iteratorTwo);
			Assert::IsTrue(hashmap.end() != hashmapTemp.end());
		}

		TEST_METHOD(HashmapIteratorInEqualityOperator)
		{
			//Key - int32_t
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);
			HashmapIteratorInEqualityOperator(hashmapInt, firstPairInt, secondPairInt, thirdPairInt);

			int32_t firstInteger = 10;
			int32_t secondInteger = 20;
			int32_t thirdInteger = 30;

			//Key - int32_t*
			Hashmap<int32_t*, int32_t> hashmapIntPointer(100);
			pair<int32_t*, int32_t> firstPairIntPointer = make_pair(&firstInteger, 100);
			pair<int32_t*, int32_t> secondPairIntPointer = make_pair(&secondInteger, 200);
			pair<int32_t*, int32_t> thirdPairIntPointer = make_pair(&thirdInteger, 300);
			HashmapIteratorInEqualityOperator(hashmapIntPointer, firstPairIntPointer, secondPairIntPointer, thirdPairIntPointer);

			char* firstCharArray = "HelloOne";
			char* secondCharArray = "HelloTwo";
			char* thirdCharArray = "HelloThree";

			Foo firstFoo(firstInteger);
			Foo secondFoo(secondInteger);
			Foo thirdFoo(thirdInteger);

			//Key - char*
			Hashmap<char*, Foo> hashmapCharArray(100);
			pair<char*, Foo> firstPairCharArray = make_pair(firstCharArray, firstFoo);
			pair<char*, Foo> secondPairCharArray = make_pair(secondCharArray, secondFoo);
			pair<char*, Foo> thirdPairCharArray = make_pair(thirdCharArray, thirdFoo);
			HashmapIteratorInEqualityOperator(hashmapCharArray, firstPairCharArray, secondPairCharArray, thirdPairCharArray);

			string firstString = "HelloOne";
			string secondString = "HelloTwo";
			string thirdString = "HelloThree";

			//Key - string
			Hashmap<string, int32_t> hashmapString(100);
			pair<string, int32_t> firstPairString = make_pair(firstString, 100);
			pair<string, int32_t> secondPairString = make_pair(secondString, 200);
			pair<string, int32_t> thirdPairString = make_pair(thirdString, 300);
			HashmapIteratorInEqualityOperator(hashmapString, firstPairString, secondPairString, thirdPairString);

			//Key - Foo
			Hashmap<Foo, int32_t, FooHashFunctor> hashmapFoo(100);
			pair<Foo, int32_t> firstPairFoo = make_pair(firstFoo, 100);
			pair<Foo, int32_t> secondPairFoo = make_pair(secondFoo, 200);
			pair<Foo, int32_t> thirdPairFoo = make_pair(thirdFoo, 300);
			HashmapIteratorInEqualityOperator(hashmapFoo, firstPairFoo, secondPairFoo, thirdPairFoo);
		}


		TEST_METHOD(HashmapMoveSemantics)
		{
			Hashmap<int32_t, int32_t> hashmapInt(100);
			pair<int32_t, int32_t> firstPairInt = make_pair(10, 100);
			pair<int32_t, int32_t> secondPairInt = make_pair(20, 200);
			pair<int32_t, int32_t> thirdPairInt = make_pair(30, 300);

			Hashmap<int32_t, int32_t> hashmaptemp = move(hashmapInt);
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState HashmapTest::sStartMemState;
}