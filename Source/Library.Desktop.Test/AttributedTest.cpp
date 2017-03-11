#include "Pch.h"
#include "CppUnitTest.h"
#include "AttributedFooChild.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;

namespace LibraryDesktopTest
{
	TEST_CLASS(AttributedTest)
	{
	public:
#pragma region AttributedFoo
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

		void AttributedIsPrescribed(AttributedFoo& attributedFoo)
		{
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalInteger"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalFloat"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalString"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalVector4"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalMatrix4x4"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalStringArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalVector4Array"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mInternalMatrix4x4Array"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalInteger"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalFloat"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalString"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalVector4"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalMatrix4x4"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("AttributedFoo"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalStringArray"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalVector4Array"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("mExternalMatrix4x4Array"));

			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("NestedScope"));
			Assert::IsTrue(attributedFoo.IsPrescribedAttribute("AdoptedScope"));

			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("Temp"));
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute("Ola"));
		}

		TEST_METHOD(AttributedTestIsPrescribed)
		{
			AttributedFoo attributedFoo;
			AttributedIsPrescribed(attributedFoo);
		}

		void AttributedIsAttribute(AttributedFoo& attributedFoo)
		{
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalInteger"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalFloat"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalString"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalVector4"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalMatrix4x4"));

			Assert::IsTrue(attributedFoo.IsAttribute("mInternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalStringArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalVector4Array"));
			Assert::IsTrue(attributedFoo.IsAttribute("mInternalMatrix4x4Array"));

			Assert::IsTrue(attributedFoo.IsAttribute("mExternalInteger"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalFloat"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalString"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalVector4"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalMatrix4x4"));

			Assert::IsTrue(attributedFoo.IsAttribute("AttributedFoo"));

			Assert::IsTrue(attributedFoo.IsAttribute("mExternalIntegerArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalFloatArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalStringArray"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalVector4Array"));
			Assert::IsTrue(attributedFoo.IsAttribute("mExternalMatrix4x4Array"));

			Assert::IsTrue(attributedFoo.IsAttribute("NestedScope"));
			Assert::IsTrue(attributedFoo.IsAttribute("AdoptedScope"));

			Assert::IsFalse(attributedFoo.IsAttribute("Temp"));
			Assert::IsFalse(attributedFoo.IsAttribute("Ola"));
			Assert::IsFalse(attributedFoo.IsAttribute("SunShine"));
		}

		TEST_METHOD(AttributedTestIsAttribute)
		{
			AttributedFoo attributedFoo;
			AttributedIsAttribute(attributedFoo);
		}

		void AttributedTestAuxillaryAttribute(AttributedFoo& attributedFoo, string nameOne, string nameTwo, string nameThree)
		{
			Assert::IsTrue(attributedFoo.IsAttribute(nameOne));
			Assert::IsTrue(attributedFoo.IsAttribute(nameTwo));
			Assert::IsTrue(attributedFoo.IsAttribute(nameThree));

			Assert::IsTrue(attributedFoo.IsAuxiliaryAttribute(nameOne));
			Assert::IsTrue(attributedFoo.IsAuxiliaryAttribute(nameTwo));
			Assert::IsTrue(attributedFoo.IsAuxiliaryAttribute(nameThree));

			Assert::IsFalse(attributedFoo.IsPrescribedAttribute(nameOne));
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute(nameTwo));
			Assert::IsFalse(attributedFoo.IsPrescribedAttribute(nameThree));

			Assert::IsFalse(attributedFoo.IsAuxiliaryAttribute("mInternalInteger"));
			Assert::IsFalse(attributedFoo.IsAuxiliaryAttribute("mInternalFloat"));
			Assert::IsFalse(attributedFoo.IsAuxiliaryAttribute("mInternalString"));
			Assert::IsFalse(attributedFoo.IsAuxiliaryAttribute("mInternalVector4"));
			Assert::IsFalse(attributedFoo.IsAuxiliaryAttribute("mInternalMatrix4x4"));

			Assert::ExpectException<exception>([&] {attributedFoo.AppendAuxiliaryAttribute("mInternalInteger"); });
			Assert::ExpectException<exception>([&] {attributedFoo.AppendAuxiliaryAttribute("mInternalFloat"); });
		}

		TEST_METHOD(AttributedTestAuxillaryAttribute)
		{
			AttributedFoo attributedFoo;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFoo.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFoo.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFoo.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedTestAuxillaryAttribute(attributedFoo, nameOne, nameTwo, nameThree);
		}

		TEST_METHOD(AttributeAuxillaryBegin)
		{
			AttributedFoo attributedFoo;
			Assert::AreEqual(attributedFoo.AuxiliaryBegin(), 24U);
		}

		void AttributeMisc(AttributedFoo& attributedFoo)
		{
			Assert::AreEqual(attributedFoo["mInternalIntegerArray"].Get<int32_t>(0), attributedFoo.mInternalIntegerArray[0]);
			Assert::AreEqual(attributedFoo["mInternalIntegerArray"].Get<int32_t>(1), attributedFoo.mInternalIntegerArray[1]);

			Assert::AreEqual(attributedFoo["mExternalIntegerArray"].Get<int32_t>(0), attributedFoo.mExternalIntegerArray[0]);
			Assert::AreEqual(attributedFoo["mExternalIntegerArray"].Get<int32_t>(1), attributedFoo.mExternalIntegerArray[1]);

			Assert::AreEqual(attributedFoo["mExternalInteger"].Get<int32_t>(0), attributedFoo.mExternalInteger);
			attributedFoo.mExternalInteger = 50;
			Assert::AreEqual(attributedFoo["mExternalInteger"].Get<int32_t>(0), 50);
			attributedFoo["mExternalInteger"].Set(100);
			Assert::AreEqual(attributedFoo.mExternalInteger, 100);
			Assert::AreEqual(attributedFoo["mExternalInteger"].Get<int32_t>(0), attributedFoo.mExternalInteger);

			Datum* datum = attributedFoo.Find("mExternalFloat");
			Assert::AreEqual(datum->Size(), 1U);
			Assert::AreEqual(&datum->Get<std::float_t>(), &attributedFoo.mExternalFloat);

			float tempFloat = 50.0f;
			attributedFoo["mExternalFloat"] = tempFloat;
			Assert::AreEqual(tempFloat, attributedFoo.mExternalFloat);
		}

		TEST_METHOD(AttributeMisc)
		{
			AttributedFoo attributedFoo;
			AttributeMisc(attributedFoo);
		}

		TEST_METHOD(AttributeCopyConstructor)
		{
			AttributedFoo attributedFoo;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFoo.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFoo.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFoo.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFoo newAttributedFoo(attributedFoo);

			AttributedIsPrescribed(newAttributedFoo);
			AttributedIsAttribute(newAttributedFoo);
			AttributedTestAuxillaryAttribute(newAttributedFoo, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFoo.AuxiliaryBegin(), 24U);
			AttributeMisc(newAttributedFoo);
		}

		TEST_METHOD(AttributeAssignmentOperator)
		{
			AttributedFoo attributedFoo;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFoo.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFoo.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFoo.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFoo newAttributedFoo;
			newAttributedFoo = attributedFoo;

			AttributedIsPrescribed(newAttributedFoo);
			AttributedIsAttribute(newAttributedFoo);
			AttributedTestAuxillaryAttribute(newAttributedFoo, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFoo.AuxiliaryBegin(), 24U);
			AttributeMisc(newAttributedFoo);
		}

		AttributedFoo CreateTemporaryAttribute(string nameOne, string nameTwo, string nameThree)
		{
			AttributedFoo attributedFoo;

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFoo.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFoo.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFoo.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			return attributedFoo;
		}

		TEST_METHOD(AttributeMoveConstructor)
		{
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			AttributedFoo newAttributedFoo = CreateTemporaryAttribute(nameOne, nameTwo, nameThree);
			newAttributedFoo.IsPrescribedAttribute("mInternalInteger");
			Assert::IsTrue(newAttributedFoo.IsPrescribedAttribute("mInternalInteger"));
			AttributedIsPrescribed(newAttributedFoo);
			AttributedIsAttribute(newAttributedFoo);
			AttributedTestAuxillaryAttribute(newAttributedFoo, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFoo.AuxiliaryBegin(), 24U);
			AttributeMisc(newAttributedFoo);
		}

		TEST_METHOD(AttributeMoveAssignmentOperator)
		{
			AttributedFoo attributedFoo;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFoo.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFoo.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFoo.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFoo newAttributedFoo;
			newAttributedFoo = move(attributedFoo);

			AttributedIsPrescribed(newAttributedFoo);
			AttributedIsAttribute(newAttributedFoo);
			AttributedTestAuxillaryAttribute(newAttributedFoo, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFoo.AuxiliaryBegin(), 24U);
			AttributeMisc(newAttributedFoo);
		}
#pragma endregion
		
#pragma region Tests With AttributedFooChild
		void AttributedFooChildIsPrescribed(AttributedFooChild& attributedFooChild)
		{
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mExternalIntegerChild"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mExternalFloatChild"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalIntegerChild"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalFloatChild"));

			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalInteger"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalFloat"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalString"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalVector4"));
			Assert::IsTrue(attributedFooChild.IsPrescribedAttribute("mInternalMatrix4x4"));

			Assert::IsFalse(attributedFooChild.IsPrescribedAttribute("Temp"));
			Assert::IsFalse(attributedFooChild.IsPrescribedAttribute("Ola"));
		}

		TEST_METHOD(AttributeFooChildIsPrescribedTest)
		{
			AttributedFooChild fooChild;
			AttributedFooChildIsPrescribed(fooChild);
		}

		TEST_METHOD(AttributeFooChildCopyConstructor)
		{
			AttributedFooChild attributedFooChild;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFooChild.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFooChild.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFooChild.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFooChild newAttributedFooChild(attributedFooChild);

			AttributedIsPrescribed(newAttributedFooChild);
			AttributedIsAttribute(newAttributedFooChild);
			AttributedTestAuxillaryAttribute(newAttributedFooChild, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFooChild.AuxiliaryBegin(), 28U);
			AttributeMisc(newAttributedFooChild);
		}

		TEST_METHOD(AttributeFooChildAssignmentOperator)
		{
			AttributedFooChild attributedFooChild;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFooChild.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFooChild.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFooChild.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFooChild newAttributedFooChild;
			newAttributedFooChild = attributedFooChild;

			AttributedIsPrescribed(newAttributedFooChild);
			AttributedIsAttribute(newAttributedFooChild);
			AttributedTestAuxillaryAttribute(newAttributedFooChild, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFooChild.AuxiliaryBegin(), 28U);
			AttributeMisc(newAttributedFooChild);
		}

		AttributedFooChild CreateTemporaryAttributeFooChild(string nameOne, string nameTwo, string nameThree)
		{
			AttributedFooChild attributedFooChild;

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFooChild.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFooChild.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFooChild.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			return attributedFooChild;
		}

		TEST_METHOD(AttributeFooChildMoveConstructor)
		{
			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			AttributedFooChild newAttributedFooChild = CreateTemporaryAttributeFooChild(nameOne, nameTwo, nameThree);

			newAttributedFooChild.IsPrescribedAttribute("mInternalInteger");
			Assert::IsTrue(newAttributedFooChild.IsPrescribedAttribute("mInternalInteger"));
			AttributedIsPrescribed(newAttributedFooChild);
			AttributedIsAttribute(newAttributedFooChild);
			AttributedTestAuxillaryAttribute(newAttributedFooChild, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFooChild.AuxiliaryBegin(), 28U);
			AttributeMisc(newAttributedFooChild);
		}
		
		TEST_METHOD(AttributeFooChildMoveAssignmentOperator)
		{
			AttributedFooChild attributedFooChild;

			string nameOne = "Hello";
			string nameTwo = "World";
			string nameThree = "Now";

			int32_t firstPushedInt = 10;
			string firstPushedString = "Hello";
			Scope *firstPushedScope = new Scope();

			Datum& firstAppend = attributedFooChild.AppendAuxiliaryAttribute(nameOne);
			firstAppend.SetType(DatumType::INT32_T);
			firstAppend.PushBack(firstPushedInt);

			Datum& secondAppend = attributedFooChild.AppendAuxiliaryAttribute(nameTwo);
			secondAppend.SetType(DatumType::STRING);
			secondAppend.PushBack(firstPushedString);

			Datum& thirdAppend = attributedFooChild.AppendAuxiliaryAttribute(nameThree);
			thirdAppend.SetType(DatumType::TABLE);
			thirdAppend.PushBack(firstPushedScope);

			AttributedFooChild newAttributedFooChild;
			newAttributedFooChild = move(attributedFooChild);

			AttributedIsPrescribed(newAttributedFooChild);
			AttributedIsAttribute(newAttributedFooChild);
			AttributedTestAuxillaryAttribute(newAttributedFooChild, nameOne, nameTwo, nameThree);
			Assert::AreEqual(newAttributedFooChild.AuxiliaryBegin(), 28U);
			AttributeMisc(newAttributedFooChild);
		}
#pragma endregion
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState AttributedTest::sStartMemState;
}