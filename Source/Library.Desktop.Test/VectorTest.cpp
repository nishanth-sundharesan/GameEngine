#include "Pch.h"
#include "CppUnitTest.h"
#include "Vector.h"
#include "Foo.h"
#include "ToStringTemplates.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;

namespace LibraryDesktopTest
{
	TEST_CLASS(VectorTest)
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

		template<typename T>
		void TestEmptyVector(Vector<T>& vector, T& tempValue, uint32_t allocatedCapacity)
		{
			uint32_t currentSize = 0;

			//Testing of the non-const versions						
			Assert::ExpectException<out_of_range>([&] { vector[0]; });
			Assert::ExpectException<out_of_range>([&] { vector[1]; });

			Assert::ExpectException<out_of_range>([&] { vector.At(0); });
			Assert::ExpectException<out_of_range>([&] { vector.At(1); });

			Assert::ExpectException<out_of_range>([&] { vector.Front(); });
			Assert::ExpectException<out_of_range>([&] { vector.Back(); });

			//Testing of the const versions
			const Vector<T>& vectorconst = vector;
			Assert::ExpectException<out_of_range>([&] { vectorconst[0]; });
			Assert::ExpectException<out_of_range>([&] { vectorconst[1]; });

			Assert::ExpectException<out_of_range>([&] { vectorconst.At(0); });
			Assert::ExpectException<out_of_range>([&] { vectorconst.At(1); });

			Assert::ExpectException<out_of_range>([&] { vectorconst.Front(); });
			Assert::ExpectException<out_of_range>([&] { vectorconst.Back(); });

			//Testing of other functions which only have single versions
			Assert::IsTrue(vector.IsEmpty());
			Assert::AreEqual(vector.Size(), currentSize);
			Assert::AreEqual(vector.Capacity(), allocatedCapacity);

			Assert::IsTrue(vector.Find(tempValue) == vector.end());
			Assert::IsFalse(vector.Remove(tempValue));
			Assert::IsFalse(vector.PopBack());
			Assert::IsFalse(vector.PopBack(tempValue));
		}

		template<typename T>
		void TestVectorDefaultConstructor(Vector<T>& vector, T& tempValue, uint32_t allocatedCapacity)
		{
			TestEmptyVector(vector, tempValue, allocatedCapacity);

			vector.ShrinkToFit();
			vector.Reserve(0);

			allocatedCapacity = 0;
			TestEmptyVector(vector, tempValue, 0);
		}

		TEST_METHOD(TestVectorDefaultConstructor)
		{
			Vector<int32_t> intVector;
			int32_t tempInt = 10;
			uint32_t capacity = 29;
			TestVectorDefaultConstructor(intVector, tempInt, capacity);

			Vector<Foo> fooVector;
			Foo tempFoo(10);
			TestVectorDefaultConstructor(fooVector, tempFoo, capacity);
		}

		template<typename T>
		void TestVectorConstructorWithCapacity(Vector<T>& vector, T& tempValue, uint32_t allocatedCapacity)
		{
			TestEmptyVector(vector, tempValue, allocatedCapacity);

			vector.ShrinkToFit();
			vector.Reserve(0);

			TestEmptyVector(vector, tempValue, 0);
		}

		TEST_METHOD(TestVectorConstructorWithCapacity)
		{
			uint32_t capacity = 10;

			int32_t tempInt = 10;
			Vector<int32_t> intVector(capacity);
			TestVectorConstructorWithCapacity(intVector, tempInt, capacity);

			Foo tempFoo(10);
			Vector<Foo> fooVector(capacity);
			TestVectorConstructorWithCapacity(fooVector, tempFoo, capacity);
		}
		
		template<typename T>
		void TestVectorIsEmpty(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Assert::IsTrue(vector.IsEmpty());

			vector.PushBack(firstPushedValue);
			Assert::IsFalse(vector.IsEmpty());

			vector.PushBack(secondPushedValue);
			Assert::IsFalse(vector.IsEmpty());

			vector.PushBack(thirdPushedValue);
			Assert::IsFalse(vector.IsEmpty());

			vector.PopBack();
			Assert::IsFalse(vector.IsEmpty());

			vector.Clear();
			Assert::IsTrue(vector.IsEmpty());

			vector.ShrinkToFit();
			Assert::IsTrue(vector.IsEmpty());

			vector.PushBack(firstPushedValue);
			Assert::IsFalse(vector.IsEmpty());

			vector.PopBack();
			Assert::IsTrue(vector.IsEmpty());

			vector.PushBack(firstPushedValue);
			Assert::IsFalse(vector.IsEmpty());

			vector.Remove(firstPushedValue);
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(TestVectorIsEmpty)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorIsEmpty(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorIsEmpty(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorSize(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			uint32_t currentSize = 0;

			Assert::AreEqual(vector.Size(), currentSize);

			vector.PushBack(firstPushedValue);
			++currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.PushBack(secondPushedValue);
			++currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.PushBack(thirdPushedValue);
			++currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.ShrinkToFit();
			Assert::AreEqual(vector.Size(), currentSize);

			vector.Clear();
			currentSize = 0;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.ShrinkToFit();
			Assert::AreEqual(vector.Size(), currentSize);

			vector.PushBack(firstPushedValue);
			++currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.PopBack();
			--currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.PushBack(firstPushedValue);
			++currentSize;
			Assert::AreEqual(vector.Size(), currentSize);

			vector.Remove(firstPushedValue);
			--currentSize;
			Assert::AreEqual(vector.Size(), currentSize);
		}

		TEST_METHOD(TestVectorSize)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorSize(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorSize(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorCapacity(Vector<T>& vector, T& firstPushedValue, uint32_t allocatedCapacity)
		{
			uint32_t zeroCapacity = 0;
			Assert::AreEqual(allocatedCapacity, vector.Capacity());

			vector.ShrinkToFit();
			Assert::AreEqual(vector.Size(), vector.Capacity());

			allocatedCapacity = vector.Capacity();
			vector.PushBack(firstPushedValue);
			Assert::AreNotEqual(vector.Capacity(), allocatedCapacity);

			vector.Clear();
			vector.ShrinkToFit();
			Assert::AreEqual(vector.Size(), vector.Capacity());
			Assert::AreEqual(vector.Capacity(), zeroCapacity);

			vector.PushBack(firstPushedValue);
			Assert::AreNotEqual(vector.Capacity(), zeroCapacity);
		}

		TEST_METHOD(TestVectorCapacity)
		{
			Vector<int32_t> intVector;
			uint32_t capacity = 29;
			int32_t firstPushedIntValue = 10;
			TestVectorCapacity(intVector, firstPushedIntValue, capacity);

			capacity = 10;
			Vector<int32_t> intVectorWithCapacity(capacity);
			TestVectorCapacity(intVectorWithCapacity, firstPushedIntValue, capacity);

			capacity = 29;
			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			TestVectorCapacity(fooVector, firstPushedFooValue, capacity);

			capacity = 10;
			Vector<Foo> fooVectorWithCapacity(capacity);
			TestVectorCapacity(fooVectorWithCapacity, firstPushedFooValue, capacity);
		}

		template<typename T>
		void TestVectorPushBack(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Vector<T>::Iterator iterator;
			uint32_t currentSize = 0;

			//Pushing the first value
			iterator = vector.PushBack(firstPushedValue);
			++currentSize;

			Assert::AreEqual(*iterator, firstPushedValue);
			Assert::IsTrue(iterator == vector.begin());
			Assert::AreEqual(vector.Size(), currentSize);

			//Pushing the second value
			iterator = vector.PushBack(secondPushedValue);
			++currentSize;

			Assert::AreEqual(*iterator, secondPushedValue);
			Assert::AreEqual(vector.Size(), currentSize);

			//Pushing the third value
			iterator = vector.PushBack(thirdPushedValue);
			++currentSize;

			Assert::AreEqual(*iterator, thirdPushedValue);
			Assert::AreEqual(vector.Size(), currentSize);

			//Clearing the list
			vector.Clear();
			currentSize = 0;

			iterator = vector.PushBack(firstPushedValue);
			++currentSize;

			Assert::AreEqual(*iterator, firstPushedValue);
			Assert::AreEqual(vector.Size(), currentSize);

			//Clearing the list and shrinking to size
			vector.Clear();
			vector.ShrinkToFit();
			currentSize = 0;

			iterator = vector.PushBack(firstPushedValue);
			++currentSize;

			Assert::AreEqual(*iterator, firstPushedValue);
			Assert::AreEqual(vector.Size(), currentSize);
		}

		TEST_METHOD(TestVectorPushBack)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorPushBack(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorPushBack(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorIndexOfOperator(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Assert::ExpectException<out_of_range>([&] { vector[0]; });
			Assert::ExpectException<out_of_range>([&] { vector[1]; });

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorOne = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorOne[0]; });
			Assert::ExpectException<out_of_range>([&] { constVectorOne[1]; });
			/************************************************************************/

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector[0], firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector[10]; });

			vector.PushBack(secondPushedValue);
			Assert::AreEqual(vector[1], secondPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector[10]; });

			vector.PushBack(thirdPushedValue);
			Assert::AreEqual(vector[2], thirdPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector[10]; });

			Assert::AreEqual(vector[0], firstPushedValue);
			Assert::AreEqual(vector[1], secondPushedValue);
			Assert::AreEqual(vector[2], thirdPushedValue);

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorTwo = vector;
			Assert::AreEqual(constVectorTwo[0], firstPushedValue);
			Assert::AreEqual(constVectorTwo[1], secondPushedValue);
			Assert::AreEqual(constVectorTwo[2], thirdPushedValue);
			/************************************************************************/

			vector.Clear();
			Assert::ExpectException<out_of_range>([&] { vector[0]; });
			Assert::ExpectException<out_of_range>([&] { vector[1]; });
			Assert::ExpectException<out_of_range>([&] { vector[10]; });

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorThree = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorThree[0]; });
			Assert::ExpectException<out_of_range>([&] { constVectorThree[1]; });
			Assert::ExpectException<out_of_range>([&] { constVectorThree[10]; });
			/************************************************************************/

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector[0], firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector[10]; });

			const Vector<T> constVectorFour = vector;
			Assert::AreEqual(constVectorFour[0], firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { constVectorFour[10]; });
		}

		TEST_METHOD(TestVectorIndexOfOperator)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorIndexOfOperator(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorIndexOfOperator(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorAt(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Assert::ExpectException<out_of_range>([&] { vector.At(0); });
			Assert::ExpectException<out_of_range>([&] { vector.At(1); });

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorOne = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorOne.At(0); });
			Assert::ExpectException<out_of_range>([&] { constVectorOne.At(1); });
			/************************************************************************/

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.At(0), firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector.At(10); });

			vector.PushBack(secondPushedValue);
			Assert::AreEqual(vector.At(1), secondPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector.At(10); });

			vector.PushBack(thirdPushedValue);
			Assert::AreEqual(vector.At(2), thirdPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector.At(10); });

			Assert::AreEqual(vector.At(0), firstPushedValue);
			Assert::AreEqual(vector.At(1), secondPushedValue);
			Assert::AreEqual(vector.At(2), thirdPushedValue);

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorTwo = vector;
			Assert::AreEqual(constVectorTwo.At(0), firstPushedValue);
			Assert::AreEqual(constVectorTwo.At(1), secondPushedValue);
			Assert::AreEqual(constVectorTwo.At(2), thirdPushedValue);
			/************************************************************************/

			vector.Clear();
			Assert::ExpectException<out_of_range>([&] { vector.At(0); });
			Assert::ExpectException<out_of_range>([&] { vector.At(1); });
			Assert::ExpectException<out_of_range>([&] { vector.At(10); });

			/************************************************************************/
			//Checking of the const version
			const Vector<T> constVectorThree = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorThree.At(0); });
			Assert::ExpectException<out_of_range>([&] { constVectorThree.At(1); });
			Assert::ExpectException<out_of_range>([&] { constVectorThree.At(10); });
			/************************************************************************/

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.At(0), firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { vector.At(10); });

			const Vector<T> constVectorFour = vector;
			Assert::AreEqual(constVectorFour.At(0), firstPushedValue);
			Assert::ExpectException<out_of_range>([&] { constVectorFour.At(10); });
		}

		TEST_METHOD(TestVectorAt)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorAt(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorAt(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorFront(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Assert::ExpectException<out_of_range>([&] { vector.Front(); });

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PushBack(thirdPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PopBack();
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PopBack(secondPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PopBack();
			Assert::ExpectException<out_of_range>([&] { vector.Front(); });

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.Clear();
			Assert::ExpectException<out_of_range>([&] { vector.Front(); });


			//Testing Const version of Front()
			const Vector<T> constVectorOne = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorOne.Front(); });

			vector.PushBack(firstPushedValue);
			const Vector<T> constVectorTwo = vector;
			Assert::AreEqual(constVectorTwo.Front(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			const Vector<T> constVectorThree = vector;
			Assert::AreEqual(constVectorThree.Front(), firstPushedValue);

			vector.PushBack(thirdPushedValue);
			const Vector<T> constVectorFour = vector;
			Assert::AreEqual(constVectorFour.Front(), firstPushedValue);

			vector.PopBack();
			const Vector<T> constVectorFive = vector;
			Assert::AreEqual(constVectorFive.Front(), firstPushedValue);

			vector.PopBack(secondPushedValue);
			const Vector<T> constVectorSix = vector;
			Assert::AreEqual(constVectorSix.Front(), firstPushedValue);

			vector.PopBack();
			const Vector<T> constVectorSeven = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorSeven.Front(); });

			vector.PushBack(firstPushedValue);
			const Vector<T> constVectorEight = vector;
			Assert::AreEqual(constVectorEight.Front(), firstPushedValue);

			vector.Clear();
			const Vector<T> constVectorNine = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorNine.Front(); });
		}

		TEST_METHOD(TestVectorFront)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorFront(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorFront(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorBack(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Assert::ExpectException<out_of_range>([&] { vector.Back(); });

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.Back(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			Assert::AreEqual(vector.Back(), secondPushedValue);

			vector.PushBack(thirdPushedValue);
			Assert::AreEqual(vector.Back(), thirdPushedValue);

			vector.PopBack();
			Assert::AreEqual(vector.Back(), secondPushedValue);

			vector.PopBack(secondPushedValue);
			Assert::AreEqual(vector.Front(), firstPushedValue);

			vector.PopBack();
			Assert::ExpectException<out_of_range>([&] { vector.Back(); });

			vector.PushBack(firstPushedValue);
			Assert::AreEqual(vector.Back(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			Assert::AreEqual(vector.Back(), secondPushedValue);

			vector.Clear();
			Assert::ExpectException<out_of_range>([&] { vector.Back(); });


			//Testing Const version of Front()
			const Vector<T> constVectorOne = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorOne.Back(); });

			vector.PushBack(firstPushedValue);
			const Vector<T> constVectorTwo = vector;
			Assert::AreEqual(constVectorTwo.Back(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			const Vector<T> constVectorThree = vector;
			Assert::AreEqual(constVectorThree.Back(), secondPushedValue);

			vector.PushBack(thirdPushedValue);
			const Vector<T> constVectorFour = vector;
			Assert::AreEqual(constVectorFour.Back(), thirdPushedValue);

			vector.PopBack();
			const Vector<T> constVectorFive = vector;
			Assert::AreEqual(constVectorFive.Back(), secondPushedValue);

			vector.PopBack(secondPushedValue);
			const Vector<T> constVectorSix = vector;
			Assert::AreEqual(constVectorSix.Back(), firstPushedValue);

			vector.PopBack();
			const Vector<T> constVectorSeven = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorSeven.Back(); });

			vector.PushBack(firstPushedValue);
			const Vector<T> constVectorEight = vector;
			Assert::AreEqual(constVectorEight.Back(), firstPushedValue);

			vector.PushBack(secondPushedValue);
			const Vector<T> constVectorNine = vector;
			Assert::AreEqual(constVectorNine.Back(), secondPushedValue);

			vector.Clear();
			const Vector<T> constVectorTen = vector;
			Assert::ExpectException<out_of_range>([&] { constVectorTen.Back(); });
		}

		TEST_METHOD(TestVectorBack)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorBack(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorBack(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorFind(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Vector<T>::Iterator iterator;
			iterator = vector.Find(thirdPushedValue);
			Assert::IsTrue(iterator == vector.end());

			vector.PushBack(firstPushedValue);
			iterator = vector.Find(thirdPushedValue);
			Assert::IsTrue(iterator == vector.end());

			//Finding the single item in the vector
			iterator = vector.Find(firstPushedValue);
			Assert::AreEqual(*iterator, firstPushedValue);

			//Finding all the two items in the vector
			vector.PushBack(secondPushedValue);
			iterator = vector.Find(firstPushedValue);
			Assert::AreEqual(*iterator, firstPushedValue);

			iterator = vector.Find(secondPushedValue);
			Assert::AreEqual(*iterator, secondPushedValue);

			//Finding all the three items in the vector
			vector.PushBack(thirdPushedValue);
			iterator = vector.Find(firstPushedValue);
			Assert::AreEqual(*iterator, firstPushedValue);

			iterator = vector.Find(secondPushedValue);
			Assert::AreEqual(*iterator, secondPushedValue);

			iterator = vector.Find(thirdPushedValue);
			Assert::AreEqual(*iterator, thirdPushedValue);

			//Clearing and testing of the vector again
			vector.Clear();
			vector.PushBack(firstPushedValue);
			iterator = vector.Find(thirdPushedValue);
			Assert::IsTrue(iterator == vector.end());

			iterator = vector.Find(firstPushedValue);
			Assert::AreEqual(*iterator, firstPushedValue);
		}

		TEST_METHOD(TestVectorFind)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorFind(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorFind(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorRemoveWithValue(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			uint32_t currentSize = 0;

			Assert::IsFalse(vector.Remove(firstPushedValue));

			vector.PushBack(firstPushedValue);
			Assert::IsFalse(vector.Remove(thirdPushedValue));
			Assert::IsTrue(vector.Remove(firstPushedValue));
			Assert::AreEqual(currentSize, vector.Size());

			//Removing order -> Third node, First node, Second node
			vector.PushBack(firstPushedValue);
			vector.PushBack(secondPushedValue);
			vector.PushBack(thirdPushedValue);
			currentSize = 3;

			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsTrue(vector.Remove(thirdPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsTrue(vector.Remove(firstPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsTrue(vector.Remove(secondPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsFalse(vector.Remove(thirdPushedValue));
			Assert::IsTrue(vector.IsEmpty());


			//Removing order -> Second node, First node, Third node
			vector.PushBack(firstPushedValue);
			vector.PushBack(secondPushedValue);
			vector.PushBack(thirdPushedValue);
			currentSize = 3;
			Assert::AreEqual(currentSize, vector.Size());


			Assert::IsTrue(vector.Remove(secondPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsTrue(vector.Remove(firstPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsTrue(vector.Remove(thirdPushedValue));
			--currentSize;
			Assert::AreEqual(currentSize, vector.Size());

			Assert::IsFalse(vector.Remove(thirdPushedValue));
			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(TestVectorRemoveWithValue)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorRemoveWithValue(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorRemoveWithValue(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorRemoveWithIterators(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			Vector<T> tempVector;
			Vector<T>::Iterator tempIterator = tempVector.begin();

			Vector<T>::Iterator iterator;
			iterator = vector.begin();

			Assert::ExpectException<exception>([&] { vector.Remove(iterator, tempIterator); });
			Assert::ExpectException<exception>([&] { vector.Remove(tempIterator, iterator); });

			Assert::ExpectException<exception>([&] { vector.Remove(iterator, iterator); });

			vector.PushBack(firstPushedValue);
			vector.PushBack(secondPushedValue);
			vector.PushBack(thirdPushedValue);

			iterator = vector.begin();
			Assert::ExpectException<exception>([&] { vector.Remove(iterator, iterator); });

			iterator = vector.end();
			Assert::ExpectException<exception>([&] { vector.Remove(iterator, iterator); });

			iterator = vector.begin();
			tempIterator = vector.begin();
			++tempIterator;

			Assert::ExpectException<exception>([&] { vector.Remove(tempIterator, iterator); });

			iterator = vector.begin();
			tempIterator = vector.end();

			vector.Remove(iterator, tempIterator);
			Assert::IsTrue(vector.Find(firstPushedValue) == vector.end());

			Assert::IsTrue(vector.IsEmpty());
		}

		TEST_METHOD(TestVectorRemoveWithIterators)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorRemoveWithIterators(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorRemoveWithIterators(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}

		template<typename T>
		void TestVectorIteratorContentOfOperator(Vector<T>& vector, T& firstPushedValue, T& secondPushedValue, T& thirdPushedValue)
		{
			secondPushedValue;
			thirdPushedValue;

			Vector<T>::Iterator iterator;
			Assert::ExpectException<exception>([&] { *iterator; });

			//Const version of the content of operator
			const Vector<T>::Iterator constIteratorOne = iterator;
			Assert::ExpectException<exception>([&] { *iterator; });

			vector.PushBack(firstPushedValue);
			iterator = vector.begin();
			Assert::AreEqual(*iterator, firstPushedValue);
			Assert::AreEqual(*iterator, *(vector.begin()));

			//Const version of the content of operator
			const Vector<T>::Iterator constIteratorTwo = iterator;
			Assert::AreEqual(*constIteratorTwo, firstPushedValue);
			Assert::AreEqual(*constIteratorTwo, *(vector.begin()));

			vector.PushBack(secondPushedValue);
			vector.PushBack(thirdPushedValue);

			iterator++;
			Assert::AreEqual(*iterator, secondPushedValue);

			//Const version of the content of operator
			const Vector<T>::Iterator constIteratorFour = iterator;
			Assert::AreEqual(*constIteratorFour, secondPushedValue);

			++iterator;
			Assert::AreEqual(*iterator, thirdPushedValue);

			//Const version of the content of operator
			const Vector<T>::Iterator constIteratorFive = iterator;
			Assert::AreEqual(*constIteratorFive, thirdPushedValue);

			vector.Clear();
			vector.PushBack(firstPushedValue);
			iterator = vector.begin();
			Assert::AreEqual(*iterator, firstPushedValue);
			Assert::AreEqual(*iterator, *(vector.begin()));

			//Const version of the content of operator
			const Vector<T>::Iterator constIteratorSix = iterator;
			Assert::AreEqual(*constIteratorSix, firstPushedValue);
			Assert::AreEqual(*constIteratorSix, *(vector.begin()));
		}

		TEST_METHOD(TestVectorIteratorContentOfOperator)
		{
			Vector<int32_t> intVector;
			int32_t firstPushedIntValue = 10;
			int32_t secondPushedIntValue = 20;
			int32_t thirdPushedIntValue = 30;
			TestVectorIteratorContentOfOperator(intVector, firstPushedIntValue, secondPushedIntValue, thirdPushedIntValue);

			Vector<Foo> fooVector;
			Foo firstPushedFooValue(10);
			Foo secondPushedFooValue(20);
			Foo thirdPushedFooValue(30);
			TestVectorIteratorContentOfOperator(fooVector, firstPushedFooValue, secondPushedFooValue, thirdPushedFooValue);
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState VectorTest::sStartMemState;
}