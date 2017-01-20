#include "Pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace std;

namespace Microsoft
{
	namespace VisualStudio
	{
		namespace CppUnitTestFramework
		{
			template<> 
			inline std::wstring ToString<Foo>(const Foo&)
			{
				return L"";
			}
		}
	}
}

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

#pragma warning ( push )
#pragma warning ( disable: 4189 )	//Suppressing the warning "local variable is initialized but never used"			

			//Checking the empty list with Front();
			auto functionFront = [&list] {T tempData = list.Front(); };
			Assert::ExpectException<exception>(functionFront);

			//Checking the empty list with const T& Front();
			auto functionConstFront = [&list] {const T TempData = list.Front(); };
			Assert::ExpectException<exception>(functionConstFront);

			//Checking the empty list with Back();			
			auto functionBack = [&list] {T tempData = list.Back(); };
			Assert::ExpectException<exception>(functionBack);

			//Checking the empty list with const T& Front();
			auto functionConstBack = [&list] {const T TempData = list.Back(); };
			Assert::ExpectException<exception>(functionConstBack);

			//Checking the empty list with PopFront();			
			auto functionPopFront = [&list] {list.PopFront(); };
			Assert::ExpectException<exception>(functionPopFront);

			//Checking the empty list with PopFront(T&);
			auto functionConstPopFront = [&list] { T tempData; list.PopFront(tempData); };
			Assert::ExpectException<exception>(functionPopFront);
#pragma warning ( pop )

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
			int32_t currentSize = 0;

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
			SList<int> list;
			SList<int> copiedList = list;

			Assert::AreEqual(copiedList.Size(), 0);

			TestExceptionSlist(list);
		}

		TEST_METHOD(SListIntCopyConstructor)
		{
			SList<int> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(firstPush);
			list.PushBack(secondPush);
			list.PushBack(thirdPush);

			SList<int> copiedList = list;

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
			SList<int*> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(&firstPush);
			list.PushBack(&secondPush);
			list.PushBack(&thirdPush);

			SList<int*> copiedList = list;

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

			f3.setIntegerDataAndPointer(300);

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
			SList<int> list;
			SList<int> copiedList = list;
			copiedList = list;

			Assert::AreEqual(copiedList.Size(), 0);

			TestExceptionSlist(list);
		}

		TEST_METHOD(SListIntAssignmentOperator)
		{
			SList<int> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(firstPush);
			list.PushBack(secondPush);
			list.PushBack(thirdPush);

			SList<int> copiedList = list;
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
			SList<int*> list;
			int32_t firstPush = 10;
			int32_t secondPush = 20;
			int32_t thirdPush = 30;

			list.PushBack(&firstPush);
			list.PushBack(&secondPush);
			list.PushBack(&thirdPush);

			SList<int*> copiedList = list;
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

			f3.setIntegerDataAndPointer(300);

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
		/* BEGIN		Unit testing for the User Defined Type(Mixed case).	*/
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

	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState SListTest::sStartMemState;
}