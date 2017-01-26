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
	TEST_CLASS(SListTest)
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

		/** Test for all the exceptions.
		*	Exceptions are occurred when the list is empty and certain functions are called.
		*/
		template <typename T>
		void TestExceptionSlist(SList<T>& list)
		{
			//Checking the empty list with Front();
			auto functionFront = [&list] {list.Front(); };
			Assert::ExpectException<exception>(functionFront);

			//Checking the empty list with const T& Front();
			auto functionConstFront = [&list] {list.Front(); };
			Assert::ExpectException<exception>(functionConstFront);

			//Checking the empty list with Back();			
			auto functionBack = [&list] {list.Back(); };
			Assert::ExpectException<exception>(functionBack);

			//Checking the empty list with const T& Front();
			auto functionConstBack = [&list] {list.Back(); };
			Assert::ExpectException<exception>(functionConstBack);

			//Checking the empty list with PopFront();			
			auto functionPopFront = [&list] {list.PopFront(); };
			Assert::ExpectException<exception>(functionPopFront);

			//Checking the empty list with PopFront(T&);
			auto functionConstPopFront = [&list] { T tempData; list.PopFront(tempData); };
			Assert::ExpectException<exception>(functionPopFront);
		}

		/** Checks if the functions Size(), IsEmpty(), Front() & Back() return the same results for two lists.
		*/
		template <typename T>
		void TestSListSizeIsEmptyFrontBack(SList<T>& list, SList<T>& copiedList)
		{
			Assert::AreEqual(list.Size(), copiedList.Size());
			Assert::AreEqual(list.IsEmpty(), copiedList.IsEmpty());

			Assert::AreEqual(list.Front(), copiedList.Front());
			Assert::AreEqual(list.Back(), copiedList.Back());
		}

		/************************************************************************/
		/* BEGIN		Unit testing for the "Default Constructor".				*/
		/************************************************************************/
		TEST_METHOD(SListDefaultConstructor)
		{
			SList<int32_t> list;
			TestExceptionSlist(list);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function IsEmpty()					*/
		/************************************************************************/
		TEST_METHOD(SListIsEmptyWithDefaultConstructor)
		{
			SList<int32_t> list;

			//Checking the empty list with IsEmpty();
			bool isEmpty = list.IsEmpty();
			Assert::IsTrue(isEmpty);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmpty = list.IsEmpty();
			Assert::IsTrue(IsEmpty);
		}

		TEST_METHOD(SListIsEmptyWithClear)
		{
			SList<int32_t> list;
			list.Clear();

			//Checking the empty list with IsEmpty();
			bool isEmpty = list.IsEmpty();
			Assert::IsTrue(isEmpty);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmpty = list.IsEmpty();
			Assert::IsTrue(IsEmpty);
		}

		TEST_METHOD(SListIsEmptyWithPushAndClear)
		{
			SList<int32_t> list;
			list.PushFront(10);
			list.Clear();

			//Checking the empty list with IsEmpty();
			bool isEmpty = list.IsEmpty();
			Assert::IsTrue(isEmpty);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmpty = list.IsEmpty();
			Assert::IsTrue(IsEmpty);
		}

		TEST_METHOD(SListIsEmptyWithPushFrontPopFront)
		{
			SList<int32_t> list;
			list.PushFront(10);

			//Checking the empty list with IsEmpty();
			bool isEmpty = list.IsEmpty();
			Assert::IsFalse(isEmpty);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmpty = list.IsEmpty();
			Assert::IsFalse(IsEmpty);

			list.PopFront();

			//Checking the empty list with IsEmpty();
			bool isEmptyAfterPop = list.IsEmpty();
			Assert::IsTrue(isEmptyAfterPop);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmptyAfterPop = list.IsEmpty();
			Assert::IsTrue(IsEmptyAfterPop);
		}

		TEST_METHOD(SListIsEmptyWithPushBackPopFront)
		{
			SList<int32_t> list;
			list.PushBack(10);

			//Checking the empty list with IsEmpty();
			bool isEmpty = list.IsEmpty();
			Assert::IsFalse(isEmpty);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmpty = list.IsEmpty();
			Assert::IsFalse(IsEmpty);

			int32_t poppedValue;
			list.PopFront(poppedValue);

			//Checking the empty list with IsEmpty();
			bool isEmptyAfterPop = list.IsEmpty();
			Assert::IsTrue(isEmptyAfterPop);

			//Checking the empty list with const bool IsEmpty();
			const bool IsEmptyAfterPop = list.IsEmpty();
			Assert::IsTrue(IsEmptyAfterPop);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function Size()					*/
		/************************************************************************/
		TEST_METHOD(SListSize)
		{
			SList<int32_t> list;
			uint32_t currentSize = 0;

			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushFront(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushFront(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushBack(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Popping Data from SList
			list.PopFront();
			currentSize--;
			Assert::AreEqual(list.Size(), currentSize);

			//Clearing SList
			list.Clear();
			currentSize = 0;
			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushBack(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushBack(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Pushing Data into SList
			list.PushFront(10);
			currentSize++;
			Assert::AreEqual(list.Size(), currentSize);

			//Popping Data from SList
			int32_t tempVariable = 10;
			list.PopFront(tempVariable);
			currentSize--;
			Assert::AreEqual(list.Size(), currentSize);

			//Clearing SList
			list.Clear();
			currentSize = 0;
			Assert::AreEqual(list.Size(), currentSize);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function Front()					*/
		/************************************************************************/
		TEST_METHOD(SListFront)
		{
			SList<int32_t> list;

			int32_t firstPush = 10;
			list.PushFront(firstPush);
			Assert::AreEqual(firstPush, list.Front());

			int32_t secondPush = 20;
			list.PushFront(secondPush);
			Assert::AreEqual(secondPush, list.Front());

			int32_t thirdPush = 30;
			list.PushBack(thirdPush);
			Assert::AreEqual(secondPush, list.Front());

			list.PopFront();
			Assert::AreEqual(firstPush, list.Front());

			int32_t poppedValue;
			list.PopFront(poppedValue);
			Assert::AreEqual(poppedValue, firstPush);
			Assert::AreEqual(thirdPush, list.Front());

			list.PopFront();										//List gets cleared here
			list.PushBack(firstPush);
			Assert::AreEqual(firstPush, list.Front());

			list.PopFront(poppedValue);								//List gets cleared here
			Assert::AreEqual(poppedValue, firstPush);

			list.PushFront(firstPush);
			Assert::AreEqual(firstPush, list.Front());

			list.PushBack(secondPush);
			Assert::AreEqual(firstPush, list.Front());
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function Back()					*/
		/************************************************************************/
		TEST_METHOD(SListBack)
		{
			SList<int32_t> list;

			int32_t firstPush = 10;
			list.PushBack(firstPush);
			Assert::AreEqual(firstPush, list.Back());

			int32_t secondPush = 20;
			list.PushBack(secondPush);
			Assert::AreEqual(secondPush, list.Back());

			int32_t thirdPush = 30;
			list.PushFront(thirdPush);
			Assert::AreEqual(secondPush, list.Back());

			list.PopFront();
			Assert::AreEqual(secondPush, list.Back());

			int32_t poppedValue;
			list.PopFront(poppedValue);
			Assert::AreEqual(poppedValue, firstPush);
			Assert::AreEqual(secondPush, list.Back());

			list.PopFront();										//List gets cleared here
			list.PushBack(firstPush);
			Assert::AreEqual(firstPush, list.Back());

			list.PopFront(poppedValue);								//List gets cleared here
			Assert::AreEqual(poppedValue, firstPush);

			list.PushFront(firstPush);
			Assert::AreEqual(firstPush, list.Back());

			list.PushBack(secondPush);
			Assert::AreEqual(secondPush, list.Back());
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the "Copy Constructor".				*/
		/************************************************************************/
		TEST_METHOD(SListEmptyCopyConstructor)
		{
			SList<int32_t> list;
			SList<int32_t> copiedList = list;

			uint32_t size = 0;
			Assert::AreEqual(copiedList.Size(), size);

			TestExceptionSlist(list);
		}

		TEST_METHOD(SListIntCopyConstructor)
		{
			SList<int32_t> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(firstPush);
			list.PushBack(secondPush);
			list.PushBack(thirdPush);

			SList<int32_t> copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t listPoppedValue;
			int32_t copiedListPoppedValue;

			list.PopFront(listPoppedValue);
			copiedList.PopFront(copiedListPoppedValue);

			Assert::AreEqual(listPoppedValue, copiedListPoppedValue);

			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}

		TEST_METHOD(SListIntPointerCopyConstructor)
		{
			SList<int32_t*> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(&firstPush);
			list.PushBack(&secondPush);
			list.PushBack(&thirdPush);

			SList<int32_t*> copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t* listPoppedValue;
			int32_t* copiedListPoppedValue;

			list.PopFront(listPoppedValue);
			copiedList.PopFront(copiedListPoppedValue);

			Assert::AreEqual(listPoppedValue, copiedListPoppedValue);
			Assert::AreEqual(*listPoppedValue, *copiedListPoppedValue);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t newValue = 100;
			*(list.Front()) = newValue;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			thirdPush = 300;

			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}

		TEST_METHOD(SListUserDefinedTypeCopyConstructor)
		{
			SList<Foo> list;

			Foo f1(100);
			Foo f2(200);
			Foo f3(300);

			list.PushBack(f1);
			list.PushBack(f2);
			list.PushBack(f3);

			SList<Foo> copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			Foo listPoppedFoo;
			Foo copiedListPoppedFoo;

			list.PopFront(listPoppedFoo);
			copiedList.PopFront(copiedListPoppedFoo);

			Assert::AreEqual(listPoppedFoo, copiedListPoppedFoo);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			Foo newFoo = 100;
			list.Front() = newFoo;
			copiedList.Front() = newFoo;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			f3.SetIntegerDataAndPointer(300);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Front() = copiedList.Front();
			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Back() = copiedList.Back();
			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the "Overloaded Assignment Operator".	*/
		/************************************************************************/
		TEST_METHOD(SListEmptyAssignmentOperator)
		{
			SList<int32_t> list;
			SList<int32_t> copiedList = list;
			copiedList = list;

			uint32_t size = 0;
			Assert::AreEqual(copiedList.Size(), size);

			TestExceptionSlist(list);
		}

		TEST_METHOD(SListIntAssignmentOperator)
		{
			SList<int32_t> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(firstPush);
			list.PushBack(secondPush);
			list.PushBack(thirdPush);

			SList<int32_t> copiedList = list;
			copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t listPoppedValue;
			int32_t copiedListPoppedValue;

			list.PopFront(listPoppedValue);
			copiedList.PopFront(copiedListPoppedValue);

			Assert::AreEqual(listPoppedValue, copiedListPoppedValue);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Front() = copiedList.Front();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Back() = copiedList.Back();

			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}

		TEST_METHOD(SListIntPointerAssignmentOperator)
		{
			SList<int32_t*> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(&firstPush);
			list.PushBack(&secondPush);
			list.PushBack(&thirdPush);

			SList<int32_t*> copiedList = list;
			copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t* listPoppedValue;
			int32_t* copiedListPoppedValue;

			list.PopFront(listPoppedValue);
			copiedList.PopFront(copiedListPoppedValue);

			Assert::AreEqual(listPoppedValue, copiedListPoppedValue);
			Assert::AreEqual(*listPoppedValue, *copiedListPoppedValue);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			int32_t newValue = 100;
			*(list.Front()) = newValue;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			thirdPush = 300;
			TestSListSizeIsEmptyFrontBack(list, copiedList);

			*(list.Front()) = *(copiedList.Front());
			TestSListSizeIsEmptyFrontBack(list, copiedList);

			*(list.Back()) = *(copiedList.Back());
			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}

		TEST_METHOD(SListUserDefinedTypeAssignmentOperator)
		{
			SList<Foo> list;

			Foo f1(100);
			Foo f2(200);
			Foo f3(300);

			list.PushBack(f1);
			list.PushBack(f2);
			list.PushBack(f3);

			SList<Foo> copiedList = list;
			copiedList = list;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			Foo listPoppedFoo;
			Foo copiedListPoppedFoo;

			list.PopFront(listPoppedFoo);
			copiedList.PopFront(copiedListPoppedFoo);

			Assert::AreEqual(listPoppedFoo, copiedListPoppedFoo);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			Foo newFoo = 100;
			list.Front() = newFoo;
			copiedList.Front() = newFoo;

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			f3.SetIntegerDataAndPointer(300);

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Front() = copiedList.Front();
			TestSListSizeIsEmptyFrontBack(list, copiedList);

			list.Back() = copiedList.Back();
			TestSListSizeIsEmptyFrontBack(list, copiedList);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the User Defined Type(Mixed case).		*/
		/************************************************************************/
		TEST_METHOD(SListUserDefinedTypeSpecialCases)
		{
			SList<Foo> list;

			Foo f1(100);
			Foo f2(200);
			Foo f3(300);

			list.PushBack(f1);
			list.PushBack(f2);
			list.PushBack(f3);

			SList<Foo> copiedList = list;

			list.PopFront();

			Assert::AreNotEqual(list.Front(), copiedList.Front());
			Assert::AreEqual(list.Back(), copiedList.Back());

			Assert::AreNotEqual(list.Size(), copiedList.Size());

			list.PopFront();

			copiedList.PopFront();
			copiedList.PopFront();

			TestSListSizeIsEmptyFrontBack(list, copiedList);

			Foo listPoppedFoo;
			Foo copiedListPoppedFoo;

			list.PopFront(listPoppedFoo);
			copiedList.PopFront(copiedListPoppedFoo);

			Assert::AreEqual(listPoppedFoo, copiedListPoppedFoo);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function begin().					*/
		/************************************************************************/
		TEST_METHOD(SListbegin)
		{
			//For Integer list
			SList<int32_t> intList;
			SList<int32_t>::Iterator intListIterator = intList.begin();
			Assert::ExpectException<exception>([&intListIterator] { *intListIterator; });

			int32_t firstPushedIntVariable = 10;
			intList.PushBack(firstPushedIntVariable);
			intListIterator = intList.begin();
			Assert::AreEqual(*intListIterator, firstPushedIntVariable);

			int32_t secondPushedIntVariable = 20;
			intList.PushBack(secondPushedIntVariable);
			intListIterator = intList.begin();
			Assert::AreEqual(*intListIterator, firstPushedIntVariable);

			//For User defined list - Foo
			SList<Foo> fooList;
			SList<Foo>::Iterator fooListIterator = fooList.begin();
			Assert::ExpectException<exception>([&fooListIterator] { *fooListIterator; });

			Foo firstPushedFooVariable(10);
			fooList.PushBack(firstPushedFooVariable);
			fooListIterator = fooList.begin();
			Assert::AreEqual(*fooListIterator, firstPushedFooVariable);

			Foo secondPushedFooVariable(20);
			fooList.PushBack(secondPushedFooVariable);
			fooListIterator = fooList.begin();
			Assert::AreEqual(*fooListIterator, firstPushedFooVariable);

		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function end().					*/
		/************************************************************************/
		TEST_METHOD(SListend)
		{
			//For Integer list
			SList<int32_t> intList;
			SList<int32_t>::Iterator intListIterator = intList.end();
			Assert::ExpectException<exception>([&intListIterator] { *intListIterator; });

			int32_t firstPushedIntVariable = 10;
			intList.PushBack(firstPushedIntVariable);
			intListIterator = intList.end();
			Assert::ExpectException<exception>([&intListIterator] { *intListIterator; });

			int32_t secondPushedIntVariable = 20;
			intList.PushBack(secondPushedIntVariable);
			intListIterator = intList.end();
			Assert::ExpectException<exception>([&intListIterator] { *intListIterator; });

			//For User defined list - Foo
			SList<Foo> fooList;
			SList<Foo>::Iterator fooListIterator = fooList.end();
			Assert::ExpectException<exception>([&fooListIterator] { *fooListIterator; });

			Foo firstPushedFooVariable(10);
			fooList.PushBack(firstPushedFooVariable);
			fooListIterator = fooList.end();
			Assert::ExpectException<exception>([&fooListIterator] { *fooListIterator; });

			Foo secondPushedFooVariable(20);
			fooList.PushBack(secondPushedFooVariable);
			fooListIterator = fooList.end();
			Assert::ExpectException<exception>([&fooListIterator] { *fooListIterator; });

		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function InsertAfter().			*/
		/************************************************************************/
		TEST_METHOD(SListIntInsertAfter)
		{
			SList<int32_t> firstList;
			SList<int32_t>::Iterator iterator;

			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			int32_t thirdPushedVariable = 30;

			//Inserting data to the begin iterator of an empty SList
			iterator = firstList.begin();
			iterator = firstList.InsertAfter(firstPushedVariable, iterator);
			Assert::IsTrue(iterator == firstList.begin());
			Assert::IsTrue(*iterator == firstPushedVariable);

			iterator = firstList.begin();
			iterator = firstList.InsertAfter(secondPushedVariable, iterator);
			Assert::IsTrue(*iterator == secondPushedVariable);
			Assert::AreEqual(firstList.Back(), secondPushedVariable);


			//Trying to insert data to SList using the iterator that belongs to a different SList
			SList<int32_t> secondList;
			Assert::ExpectException<exception>([&] { secondList.InsertAfter(firstPushedVariable, iterator); });

			//Inserting data to the iterator that points one past the end of SList.
			iterator = firstList.InsertAfter(thirdPushedVariable, firstList.end());
			Assert::IsTrue(*iterator == thirdPushedVariable);
			Assert::AreEqual(firstList.Back(), thirdPushedVariable);

			//Combined testing of Iterator loop and InsertAfter()
			int32_t i = 0;
			for (iterator = firstList.begin(); iterator != firstList.end(); ++iterator, ++i)
			{
				if (i == 0)
				{
					Assert::AreEqual(*iterator, firstPushedVariable);
				}
				else if (i == 1)
				{
					Assert::AreEqual(*iterator, secondPushedVariable);
				}
				else
				{
					Assert::AreEqual(*iterator, thirdPushedVariable);
				}				
			}
			Assert::AreEqual(firstList.Front(), firstPushedVariable);
			Assert::AreEqual(firstList.Back(), thirdPushedVariable);
		}

		TEST_METHOD(SListIntPointerInsertAfter)
		{
			SList<int32_t*> firstList;
			SList<int32_t*>::Iterator iterator;

			int32_t firstPushedVariable = 10;
			int32_t secondPushedVariable = 20;
			int32_t thirdPushedVariable = 30;

			//Inserting data to the begin iterator of an empty SList
			iterator = firstList.begin();
			iterator = firstList.InsertAfter(&firstPushedVariable, iterator);
			Assert::IsTrue(iterator == firstList.begin());
			Assert::IsTrue(*(*iterator) == firstPushedVariable);

			iterator = firstList.begin();
			iterator = firstList.InsertAfter(&secondPushedVariable, iterator);
			Assert::IsTrue(*(*iterator) == secondPushedVariable);
			Assert::AreEqual(*firstList.Back(), secondPushedVariable);


			//Trying to insert data to SList using the iterator that belongs to a different SList
			SList<int32_t*> secondList;
			Assert::ExpectException<exception>([&] { secondList.InsertAfter(&firstPushedVariable, iterator); });

			//Inserting data to the iterator that points one past the end of SList.
			iterator = firstList.InsertAfter(&thirdPushedVariable, firstList.end());
			Assert::IsTrue(*(*iterator) == thirdPushedVariable);
			Assert::AreEqual(*firstList.Back(), thirdPushedVariable);

			//Combined testing of Iterator loop and InsertAfter()
			int32_t i = 0;
			for (iterator = firstList.begin(); iterator != firstList.end(); ++iterator, ++i)
			{
				if (i == 0)
				{
					Assert::AreEqual(*(*iterator), firstPushedVariable);
				}
				else if (i == 1)
				{
					Assert::AreEqual(*(*iterator), secondPushedVariable);
				}
				else
				{
					Assert::AreEqual(*(*iterator), thirdPushedVariable);
				}				
			}
			Assert::AreEqual(*firstList.Front(), firstPushedVariable);
			Assert::AreEqual(*firstList.Back(), thirdPushedVariable);
		}

		TEST_METHOD(SListUserDefinedTypeInsertAfter)
		{
			SList<Foo> firstList;
			SList<Foo>::Iterator iterator;

			Foo firstPushedVariable(10);
			Foo secondPushedVariable(20);
			Foo thirdPushedVariable(30);

			//Inserting data to the begin iterator of an empty SList
			iterator = firstList.begin();
			iterator = firstList.InsertAfter(firstPushedVariable, iterator);
			Assert::IsTrue(iterator == firstList.begin());
			Assert::IsTrue(*iterator == firstPushedVariable);

			iterator = firstList.begin();
			iterator = firstList.InsertAfter(secondPushedVariable, iterator);
			Assert::IsTrue(*iterator == secondPushedVariable);
			Assert::AreEqual(firstList.Back(), secondPushedVariable);


			//Trying to insert data to SList using the iterator that belongs to a different SList
			SList<Foo> secondList;
			Assert::ExpectException<exception>([&] { secondList.InsertAfter(firstPushedVariable, iterator); });

			//Inserting data to the iterator that points one past the end of SList.
			iterator = firstList.InsertAfter(thirdPushedVariable, firstList.end());
			Assert::IsTrue(*iterator == thirdPushedVariable);
			Assert::AreEqual(firstList.Back(), thirdPushedVariable);

			//Combined testing of Iterator loop and InsertAfter()
			int32_t i = 0;
			for (iterator = firstList.begin(); iterator != firstList.end(); ++iterator, ++i)
			{
				if (i == 0)
				{
					Assert::AreEqual(*iterator, firstPushedVariable);
				}
				else if (i == 1)
				{
					Assert::AreEqual(*iterator, secondPushedVariable);
				}
				else
				{
					Assert::AreEqual(*iterator, thirdPushedVariable);
				}				
			}
			Assert::AreEqual(firstList.Front(), firstPushedVariable);
			Assert::AreEqual(firstList.Back(), thirdPushedVariable);
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function Find().					*/
		/************************************************************************/
		TEST_METHOD(SListIntFind)
		{
			SList<int32_t> list;

			int32_t firstPushedVariable = 10;
			int32_t tempVariable = 100;
			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());

			list.PushBack(firstPushedVariable);
			//If the Find fails, Find returns an iterator that points one past the end of the list.
			Assert::IsTrue(list.Find(tempVariable) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());
			Assert::AreEqual(*(list.Find(firstPushedVariable)), firstPushedVariable);


			int32_t secondPushedVariable = 20;
			list.PushBack(secondPushedVariable);
			Assert::IsTrue(list.Find(tempVariable) == list.end());
			Assert::IsTrue(list.Find(secondPushedVariable) == ++list.begin());
			Assert::AreEqual(*(list.Find(secondPushedVariable)), secondPushedVariable);

			list.Clear();
			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());
		}

		TEST_METHOD(SListIntPointerFind)
		{
			SList<int32_t*> list;

			int32_t firstPushedVariable = 10;
			int32_t tempVariable = 100;
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.begin());

			list.PushBack(&firstPushedVariable);
			//If the Find fails, Find returns an iterator that points one past the end of the list.
			Assert::IsTrue(list.Find(&tempVariable) == list.end());
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.begin());
			Assert::AreEqual(*(*(list.Find(&firstPushedVariable))), firstPushedVariable);


			int32_t secondPushedVariable = 20;
			list.PushBack(&secondPushedVariable);
			Assert::IsTrue(list.Find(&tempVariable) == list.end());
			Assert::IsTrue(list.Find(&secondPushedVariable) == ++list.begin());
			Assert::AreEqual(*(*(list.Find(&secondPushedVariable))), secondPushedVariable);

			list.Clear();
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.begin());
		}

		TEST_METHOD(SListUserDefinedTypeFind)
		{
			SList<Foo> list;

			Foo firstPushedVariable(10);
			Foo temp(100);

			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());

			list.PushBack(firstPushedVariable);
			//If the Find fails, Find returns an iterator that points one past the end of the list.
			Assert::IsTrue(list.Find(temp) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());
			Assert::AreEqual(*(list.Find(firstPushedVariable)), firstPushedVariable);


			Foo secondPushedVariable(20);
			list.PushBack(secondPushedVariable);
			Assert::IsTrue(list.Find(temp) == list.end());
			Assert::IsTrue(list.Find(secondPushedVariable) == ++list.begin());
			Assert::AreEqual(*(list.Find(secondPushedVariable)), secondPushedVariable);

			list.Clear();
			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsTrue(list.Find(firstPushedVariable) == list.begin());
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/


		/************************************************************************/
		/* BEGIN		Unit testing for the function Remove().					*/
		/************************************************************************/
		TEST_METHOD(SListIntRemove)
		{
			SList<int32_t> list;
			int32_t firstPushedVariable = 10;

			//Trying to remove an item when the list is empty
			Assert::IsFalse(list.Remove(firstPushedVariable));

			uint32_t currentSize = 0;
			list.PushBack(firstPushedVariable);
			currentSize++;

			Assert::IsTrue(list.Remove(firstPushedVariable));
			currentSize--;
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {int32_t temp; list.PopFront(temp); });
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);

			int32_t secondPushedVariable = 20;
			int32_t thirdPushedVariable = 30;
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);
			list.PushBack(thirdPushedVariable);
			currentSize = 3;

			Assert::IsTrue(list.Remove(secondPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(secondPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(secondPushedVariable));

			Assert::IsTrue(list.Remove(thirdPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(thirdPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(thirdPushedVariable));

			Assert::IsTrue(list.Remove(firstPushedVariable));
			currentSize--;
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(firstPushedVariable));

			//Checking for exceptions after removing every element in the list
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {int32_t temp; list.PopFront(temp); });
		}

		TEST_METHOD(SListIntPointerRemove)
		{
			SList<int32_t*> list;
			int32_t firstPushedVariable = 10;

			//Trying to remove an item when the list is empty
			Assert::IsFalse(list.Remove(&firstPushedVariable));

			uint32_t currentSize = 0;
			list.PushBack(&firstPushedVariable);
			currentSize++;

			Assert::IsTrue(list.Remove(&firstPushedVariable));
			currentSize--;
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {int32_t *temp; list.PopFront(temp); });
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);

			int32_t secondPushedVariable = 20;
			int32_t thirdPushedVariable = 30;
			list.PushBack(&firstPushedVariable);
			list.PushBack(&secondPushedVariable);
			list.PushBack(&thirdPushedVariable);
			currentSize = 3;

			Assert::IsTrue(list.Remove(&secondPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(&secondPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(&secondPushedVariable));

			Assert::IsTrue(list.Remove(&thirdPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(&thirdPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(&thirdPushedVariable));

			Assert::IsTrue(list.Remove(&firstPushedVariable));
			currentSize--;
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(&firstPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(&firstPushedVariable));

			//Checking for exceptions after removing every element in the list
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {int32_t *temp; list.PopFront(temp); });
		}

		TEST_METHOD(SListUserDefinedTypeRemove)
		{
			SList<Foo> list;
			Foo firstPushedVariable(10);

			//Trying to remove an item when the list is empty
			Assert::IsFalse(list.Remove(firstPushedVariable));

			uint32_t currentSize = 0;
			list.PushBack(firstPushedVariable);
			currentSize++;

			Assert::IsTrue(list.Remove(firstPushedVariable));
			currentSize--;
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {Foo temp; list.PopFront(temp); });
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);

			Foo secondPushedVariable(20);
			Foo thirdPushedVariable(30);
			list.PushBack(firstPushedVariable);
			list.PushBack(secondPushedVariable);
			list.PushBack(thirdPushedVariable);
			currentSize = 3;

			Assert::IsTrue(list.Remove(secondPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(secondPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(secondPushedVariable));

			Assert::IsTrue(list.Remove(thirdPushedVariable));
			currentSize--;
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(thirdPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(thirdPushedVariable));

			Assert::IsTrue(list.Remove(firstPushedVariable));
			currentSize--;
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Size(), currentSize);
			Assert::IsTrue(list.Find(firstPushedVariable) == list.end());
			Assert::IsFalse(list.Remove(firstPushedVariable));

			//Checking for exceptions after removing every element in the list
			Assert::ExpectException<exception>([&list] {list.Front(); });
			Assert::ExpectException<exception>([&list] {list.Back(); });
			Assert::ExpectException<exception>([&list] {list.PopFront(); });
			Assert::ExpectException<exception>([&list] {Foo temp; list.PopFront(temp); });
		}
		/*----------------------------------------------------------------------*/
		/* END																	*/
		/*----------------------------------------------------------------------*/
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;
}