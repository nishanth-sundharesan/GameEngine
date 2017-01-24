#include "Pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"
#include "ToStringTemplates.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;

namespace LibraryDesktopTest
{
	TEST_CLASS(SListIteratorTest)
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

		/************************************************************************/
		/* BEGIN		Unit testing for the "Overloaded == and != Operator".	*/
		/************************************************************************/
		TEST_METHOD(SListIteratorEqualAndNotEqualOperator)
		{
			SList<int32_t>::Iterator iteratorOne;
			SList<int32_t>::Iterator iteratorTwo;

			Assert::IsTrue(iteratorOne == iteratorTwo);
			Assert::IsFalse(iteratorOne != iteratorTwo);

			int32_t firstPushedVariable = 10;
			SList<int32_t> listOne;
			listOne.PushBack(firstPushedVariable);

			iteratorOne = listOne.begin();
			iteratorTwo = listOne.begin();

			Assert::IsTrue(iteratorOne == iteratorTwo);
			Assert::IsFalse(iteratorOne != iteratorTwo);

			++iteratorOne;
			Assert::IsFalse(iteratorOne == iteratorTwo);
			Assert::IsTrue(iteratorOne != iteratorTwo);

			++iteratorTwo;
			Assert::IsTrue(iteratorOne == iteratorTwo);
			Assert::IsFalse(iteratorOne != iteratorTwo);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the "Overloaded ++ Operator".			*/
		/************************************************************************/
		TEST_METHOD(SListIteratorIntIncrementOperator)
		{
			SList<int32_t>::Iterator iterator;

			//Checking the empty Iterator with Prefix ++ operator
			auto functionPrefixIncrementEmpty = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEmpty);

			//Checking the empty Iterator with Postfix ++ operator
			auto functionPostfixIncrementEmpty = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEmpty);

			SList<int32_t> list;
			iterator = list.end();

			//Checking the Iterator pointing to the end of the list with Prefix ++ operator
			auto functionPrefixIncrementEnd = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEnd);

			//Checking the Iterator pointing to the end of the list with Postfix ++ operator
			auto functionPostfixIncrementEnd = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEnd);


			//Checking if the pushed value matches the value from the Iterator after using the increment operator on the Iterator
			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			iterator = list.begin();
			++iterator;
			Assert::AreEqual(*iterator, secondPushedVariable);

			iterator = list.begin();
			iterator++;
			Assert::AreEqual(*iterator, secondPushedVariable);

			//Incrementing the Iterator to the end and checking if it matches the end() iterator
			++iterator;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { ++iterator; });

			iterator = list.begin();
			iterator++;
			iterator++;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { iterator++; });
		}

		TEST_METHOD(SListIteratorIntPointerIncrementOperator)
		{
			SList<int32_t*>::Iterator iterator;

			//Checking the empty Iterator with Prefix ++ operator
			auto functionPrefixIncrementEmpty = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEmpty);

			//Checking the empty Iterator with Postfix ++ operator
			auto functionPostfixIncrementEmpty = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEmpty);

			SList<int32_t*> list;
			iterator = list.end();

			//Checking the Iterator pointing to the end of the list with Prefix ++ operator
			auto functionPrefixIncrementEnd = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEnd);

			//Checking the Iterator pointing to the end of the list with Postfix ++ operator
			auto functionPostfixIncrementEnd = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEnd);


			//Checking if the pushed value matches the value from the Iterator after using the increment operator on the Iterator
			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			list.PushBack(&firstPushedVariable);
			list.PushBack(&secondPushedVariable);

			iterator = list.begin();
			++iterator;
			Assert::AreEqual(*(*iterator), secondPushedVariable);

			iterator = list.begin();
			iterator++;
			Assert::AreEqual(*(*iterator), secondPushedVariable);

			//Incrementing the Iterator to the end and checking if it matches the end() iterator
			++iterator;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { ++iterator; });

			iterator = list.begin();
			iterator++;
			iterator++;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { iterator++; });
		}

		TEST_METHOD(SListIteratorUserDefinedTypeIncrementOperator)
		{
			SList<Foo>::Iterator iterator;

			//Checking the empty Iterator with Prefix ++ operator
			auto functionPrefixIncrementEmpty = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEmpty);

			//Checking the empty Iterator with Postfix ++ operator
			auto functionPostfixIncrementEmpty = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEmpty);

			SList<Foo> list;
			iterator = list.end();

			//Checking the Iterator pointing to the end of the list with Prefix ++ operator
			auto functionPrefixIncrementEnd = [&iterator] { ++iterator; };
			Assert::ExpectException<exception>(functionPrefixIncrementEnd);

			//Checking the Iterator pointing to the end of the list with Postfix ++ operator
			auto functionPostfixIncrementEnd = [&iterator] { iterator++; };
			Assert::ExpectException<exception>(functionPostfixIncrementEnd);


			//Checking if the pushed value matches the value from the Iterator after using the increment operator on the Iterator
			Foo firstPushedVariable(10);
			Foo secondPushedVariable(20);
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			iterator = list.begin();
			++iterator;
			Assert::AreEqual(*iterator, secondPushedVariable);

			iterator = list.begin();
			iterator++;
			Assert::AreEqual(*iterator, secondPushedVariable);

			//Incrementing the Iterator to the end and checking if it matches the end() iterator
			++iterator;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { ++iterator; });

			iterator = list.begin();
			iterator++;
			iterator++;
			Assert::IsTrue(iterator == list.end());
			Assert::ExpectException<exception>([&iterator] { iterator++; });
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/

		
		/************************************************************************/
		/* BEGIN		Unit testing for the "Overloaded * Operator".			*/
		/************************************************************************/
		TEST_METHOD(SListIteratorIntDereferenceOperator)
		{
			//Testing the non const (*)Dereference operator.
			SList<int32_t>::Iterator iterator;
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			SList<int32_t> list;
			iterator = list.begin();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			iterator = list.end();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			iterator = list.begin();
			Assert::AreEqual(*iterator, firstPushedVariable);
			++iterator;
			Assert::AreEqual(*iterator, secondPushedVariable);
			list.Clear();


			//Testing the const (*)Dereference operator.
			const SList<int32_t>::Iterator iteratorConstEmpty;
			Assert::ExpectException<exception>([&iteratorConstEmpty] { *iteratorConstEmpty; });

			const SList<int32_t>::Iterator iteratorConstBegin = list.begin();
			Assert::ExpectException<exception>([&iteratorConstBegin] { *iteratorConstBegin; });

			const SList<int32_t>::Iterator iteratorConstEnd = list.end();
			Assert::ExpectException<exception>([&iteratorConstEnd] { *iteratorConstEnd; });

			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			const SList<int32_t>::Iterator iteratorConstFirstItem = list.begin();
			Assert::AreEqual(*iteratorConstFirstItem, firstPushedVariable);

			iterator = list.begin();
			iterator++;
			const SList<int32_t>::Iterator iteratorConstSecondItem = iterator;
			Assert::AreEqual(*iteratorConstSecondItem, secondPushedVariable);
		}

		TEST_METHOD(SListIteratorIntPointerDereferenceOperator)
		{
			//Testing the non const (*)Dereference operator.
			SList<int32_t*>::Iterator iterator;
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			SList<int32_t*> list;
			iterator = list.begin();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			iterator = list.end();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			list.PushBack(&firstPushedVariable);
			list.PushBack(&secondPushedVariable);

			iterator = list.begin();
			Assert::AreEqual(*(*iterator), firstPushedVariable);
			++iterator;
			Assert::AreEqual(*(*iterator), secondPushedVariable);
			list.Clear();


			//Testing the const (*)Dereference operator.
			const SList<int32_t*>::Iterator iteratorConstEmpty;
			Assert::ExpectException<exception>([&iteratorConstEmpty] { *iteratorConstEmpty; });

			const SList<int32_t*>::Iterator iteratorConstBegin = list.begin();
			Assert::ExpectException<exception>([&iteratorConstBegin] { *iteratorConstBegin; });

			const SList<int32_t*>::Iterator iteratorConstEnd = list.end();
			Assert::ExpectException<exception>([&iteratorConstEnd] { *iteratorConstEnd; });

			list.PushBack(&firstPushedVariable);
			list.PushBack(&secondPushedVariable);

			const SList<int32_t*>::Iterator iteratorConstFirstItem = list.begin();
			Assert::AreEqual(*(*iteratorConstFirstItem), firstPushedVariable);

			iterator = list.begin();
			iterator++;
			const SList<int32_t*>::Iterator iteratorConstSecondItem = iterator;
			Assert::AreEqual(*(*iteratorConstSecondItem), secondPushedVariable);
		}

		TEST_METHOD(SListIteratorUserDefinedTypeDereferenceOperator)
		{
			//Testing the non const (*)Dereference operator.
			SList<Foo>::Iterator iterator;
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			SList<Foo> list;
			iterator = list.begin();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			iterator = list.end();
			Assert::ExpectException<exception>([&iterator] { *iterator; });

			Foo firstPushedVariable(10);
			Foo secondPushedVariable(20);
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			iterator = list.begin();
			Assert::AreEqual(*iterator, firstPushedVariable);
			++iterator;
			Assert::AreEqual(*iterator, secondPushedVariable);
			list.Clear();


			//Testing the const (*)Dereference operator.
			const SList<Foo>::Iterator iteratorConstEmpty;
			Assert::ExpectException<exception>([&iteratorConstEmpty] { *iteratorConstEmpty; });

			const SList<Foo>::Iterator iteratorConstBegin = list.begin();
			Assert::ExpectException<exception>([&iteratorConstBegin] { *iteratorConstBegin; });

			const SList<Foo>::Iterator iteratorConstEnd = list.end();
			Assert::ExpectException<exception>([&iteratorConstEnd] { *iteratorConstEnd; });

			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);

			const SList<Foo>::Iterator iteratorConstFirstItem = list.begin();
			Assert::AreEqual(*iteratorConstFirstItem, firstPushedVariable);

			iterator = list.begin();
			iterator++;
			const SList<Foo>::Iterator iteratorConstSecondItem = iterator;
			Assert::AreEqual(*iteratorConstSecondItem, secondPushedVariable);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListIteratorTest::sStartMemState;
}