#include "Pch.h"
#include "CppUnitTest.h"
#include "Datum.h"

using namespace GameEngineLibrary;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace glm;

namespace LibraryDesktopTest
{
	TEST_CLASS(DatumTest)
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

		void DatumSizeAndTypeTest(Datum& datum, uint32_t currentSize, DatumType datumType)
		{
			Assert::AreEqual(currentSize, datum.Size());
			Assert::IsTrue(datum.Type() == datumType);
		}

		TEST_METHOD(DatumDefaultConstructor)
		{
			Datum datum;
			uint32_t currentSize = 0;

			DatumSizeAndTypeTest(datum, currentSize, DatumType::UNASSIGNED);
			Assert::ExpectException<exception>([&] {datum.SetSize(0); });
		}

		TEST_METHOD(DatumSetTypeAndType)
		{
			Datum datum;

			datum.SetType(DatumType::UNASSIGNED);
			Assert::IsTrue(datum.Type() == DatumType::UNASSIGNED);

			datum.SetType(DatumType::INT32_T);
			Assert::IsTrue(datum.Type() == DatumType::INT32_T);

			Assert::ExpectException<exception>([&] {datum.SetType(DatumType::FLOAT); });
			Assert::ExpectException<exception>([&] {datum.SetType(DatumType::UNASSIGNED); });
			datum.SetType(DatumType::INT32_T);

			Assert::IsTrue(datum.Type() == DatumType::INT32_T);
		}

		TEST_METHOD(DatumSize)
		{
			Datum datum;
			uint32_t currentSize = 0;

			int32_t firstPushedValue = 10;
			int32_t secondPushedValue = 10;
			int32_t thirdPushedValue = 10;

			datum.SetType(DatumType::INT32_T);

			datum.PushBack(firstPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			datum.PushBack(secondPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			datum.PushBack(thirdPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			datum.Clear();
			currentSize = 0;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			datum.PushBack(firstPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			datum.PushBack(secondPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);
		}

		TEST_METHOD(DatumSetSize)
		{
			Datum datum;
			uint32_t currentSize = 0;
			Assert::ExpectException<exception>([&] {datum.SetSize(currentSize); });

			datum.SetType(DatumType::FLOAT);
			Assert::ExpectException<exception>([&] {datum.SetSize(currentSize); });

			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);

			float firstPushedValue = 10.0f;
			float secondPushedValue = 20.0f;
			float thirdPushedValue = 30.0f;
			datum.PushBack(firstPushedValue);
			++currentSize;

			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);

			datum.SetSize(currentSize);
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);

			Assert::AreEqual(datum.Get<std::float_t>(currentSize - 1), firstPushedValue);
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(currentSize); });
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(currentSize + 1); });

			datum.SetSize(currentSize + 1);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);
			Assert::AreEqual(datum.Get<std::float_t>(currentSize - 1), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(currentSize - 2), firstPushedValue);

			datum.SetSize(100);
			currentSize = 100;
			Assert::AreEqual(datum.Get<std::float_t>(0), firstPushedValue);
			Assert::AreEqual(datum.Get<std::float_t>(1), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(2), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(5), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(20), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(50), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(80), std::float_t());
			Assert::AreEqual(datum.Get<std::float_t>(99), std::float_t());
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(100); });
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);

			datum.Clear();
			currentSize = 0;

			datum.PushBack(firstPushedValue);
			++currentSize;

			datum.PushBack(secondPushedValue);
			++currentSize;

			datum.PushBack(thirdPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);

			datum.SetSize(2);
			currentSize = 2;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);
			Assert::AreEqual(datum.Get<std::float_t>(0), firstPushedValue);
			Assert::AreEqual(datum.Get<std::float_t>(1), secondPushedValue);
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(2); });

			datum.SetSize(1);
			currentSize = 1;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::FLOAT);
			Assert::AreEqual(datum.Get<std::float_t>(0), firstPushedValue);
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(1); });
			Assert::ExpectException<exception>([&] {datum.Get<std::float_t>(2); });
		}

		TEST_METHOD(DatumClear)
		{
			Datum datum;
			uint32_t currentSize = 0;

			DatumSizeAndTypeTest(datum, currentSize, DatumType::UNASSIGNED);
			datum.Clear();
			DatumSizeAndTypeTest(datum, currentSize, DatumType::UNASSIGNED);

			datum.SetType(DatumType::GLM_VECTOR4);
			vec4 vector4;
			vector4.w = 10.0f;
			vector4.x = 20.0f;
			vector4.y = 30.0f;
			vector4.z = 40.0f;

			datum.PushBack(vector4);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::GLM_VECTOR4);

			datum.PushBack(vector4);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::GLM_VECTOR4);

			datum.PushBack(vector4);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::GLM_VECTOR4);

			datum.Clear();
			currentSize = 0;
			DatumSizeAndTypeTest(datum, currentSize, DatumType::GLM_VECTOR4);

			//Assigning external storage and testing Clear()
			Datum externalDatum;
			externalDatum.SetType(DatumType::GLM_MATRIX4X4);

			mat4x4 matrix4x4;
			matrix4x4[0].w = 10.0f;
			matrix4x4[0].x = 20.0f;
			matrix4x4[0].y = 30.0f;
			matrix4x4[0].z = 40.0f;

			matrix4x4[1].w = 10.0f;
			matrix4x4[1].x = 20.0f;
			matrix4x4[1].y = 30.0f;
			matrix4x4[1].z = 40.0f;

			currentSize = 1;
			externalDatum.SetStorage(&matrix4x4, currentSize);
			DatumSizeAndTypeTest(externalDatum, currentSize, DatumType::GLM_MATRIX4X4);

			externalDatum.Clear();
			DatumSizeAndTypeTest(externalDatum, currentSize, DatumType::GLM_MATRIX4X4);
		}

		template<typename T1, typename T2>
		void DatumSetStorage(Datum& datum, T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType, DatumType falseType)
		{
			uint32_t currentSize = 1;
			Assert::ExpectException<exception>([&] {datum.SetStorage(&firstPushedValue, currentSize); });

			datum.SetType(datumType);

			datum.SetStorage(&firstPushedValue, currentSize);
			DatumSizeAndTypeTest(datum, currentSize, datumType);

			datum.SetStorage(&secondPushedValue, currentSize);
			DatumSizeAndTypeTest(datum, currentSize, datumType);

			datum.SetStorage(&thirdPushedValue, currentSize);
			DatumSizeAndTypeTest(datum, currentSize, datumType);

			datum.Clear();
			DatumSizeAndTypeTest(datum, currentSize, datumType);

			Assert::ExpectException<exception>([&] {datum.SetStorage(&falseValue, currentSize); });

			datum.SetStorage(&firstPushedValue, currentSize);
			DatumSizeAndTypeTest(datum, currentSize, datumType);

			Datum internalDatum1;
			internalDatum1.SetType(datumType);
			internalDatum1.PushBack(firstPushedValue);

			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&firstPushedValue, currentSize); });
			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&falseValue, currentSize); });

			currentSize = 1;
			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&secondPushedValue, currentSize); });
			DatumSizeAndTypeTest(internalDatum1, currentSize, datumType);

			Datum internalDatum2;
			internalDatum2.SetType(falseType);
			internalDatum2.PushBack(falseValue);

			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&firstPushedValue, currentSize); });
			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&falseValue, currentSize); });

			currentSize = 1;
			Assert::ExpectException<exception>([&] {internalDatum1.SetStorage(&secondPushedValue, currentSize); });
			DatumSizeAndTypeTest(internalDatum1, currentSize, datumType);
		}

		TEST_METHOD(DatumSetStorage)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.0f;
			std::float_t thirdPushedFloat = 30.0f;

			string firstPushedString = "Hello";
			string secondPushedString = "How";
			string thirdPushedString = "Ummm";

			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);
			vec4 thirdPushedVec4(90, 100, 110, 120);

			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);
			mat4x4 thirdPushedMat4x4(thirdPushedVec4, fourthVec4, firstPushedVec4, secondPushedVec4);

			Datum intDatum;
			DatumSetStorage(intDatum, firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T, DatumType::FLOAT);

			Datum floatDatum;
			DatumSetStorage(floatDatum, firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedInt, DatumType::FLOAT, DatumType::INT32_T);

			Datum stringDatum;
			DatumSetStorage(stringDatum, firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING, DatumType::GLM_VECTOR4);

			Datum vec4Datum;
			DatumSetStorage(vec4Datum, firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4, DatumType::GLM_MATRIX4X4);

			Datum mat4x4Datum;
			DatumSetStorage(mat4x4Datum, firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedString, DatumType::GLM_MATRIX4X4, DatumType::STRING);
		}

		template<typename T1, typename T2>
		void DatumSetStorage(Datum& datum, T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType)
		{
			datum.SetType(datumType);

			uint32_t currentSize = 0;
			datum.PushBack(firstPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			datum.PushBack(secondPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == secondPushedValue);

			datum.PushBack(thirdPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == thirdPushedValue);

			Assert::ExpectException<exception>([&] {datum.PushBack(falseValue); });

			datum.Clear();
			currentSize = 0;

			datum.PushBack(firstPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			datum.PushBack(secondPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == secondPushedValue);

			datum.PushBack(thirdPushedValue);
			++currentSize;
			DatumSizeAndTypeTest(datum, currentSize, datumType);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == thirdPushedValue);

			Assert::ExpectException<exception>([&] {datum.PushBack(falseValue); });

			datum.Clear();

			Datum externalDatum;
			externalDatum.SetType(datumType);
			externalDatum.SetStorage(&firstPushedValue, 1);
			externalDatum.SetStorage(&secondPushedValue, 1);

			Assert::ExpectException<exception>([&] {externalDatum.PushBack(thirdPushedValue); });
		}

		TEST_METHOD(DatumPushBack)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.0f;
			std::float_t thirdPushedFloat = 30.0f;

			string firstPushedString = "Hello";
			string secondPushedString = "How";
			string thirdPushedString = "Ummm";

			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);
			vec4 thirdPushedVec4(90, 100, 110, 120);

			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);
			mat4x4 thirdPushedMat4x4(thirdPushedVec4, fourthVec4, firstPushedVec4, secondPushedVec4);

			Datum intDatum;
			DatumSetStorage(intDatum, firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T);

			Datum floatDatum;
			DatumSetStorage(floatDatum, firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedString, DatumType::FLOAT);

			Datum stringDatum;
			DatumSetStorage(stringDatum, firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING);

			Datum vec4Datum;
			DatumSetStorage(vec4Datum, firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4);

			Datum mat4x4Datum;
			DatumSetStorage(mat4x4Datum, firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedInt, DatumType::GLM_MATRIX4X4);
		}

		template<typename T1, typename T2>
		void DatumOverloadedEqualityOperator(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType, DatumType falseDatumType)
		{
			Datum firstDatum;
			Datum secondDatum;

			Assert::IsTrue(firstDatum == secondDatum);

			//For Internal Storage
			firstDatum.SetType(datumType);
			secondDatum.SetType(datumType);

			uint32_t currentSize = 0;
			firstDatum.PushBack(firstPushedValue);
			secondDatum.PushBack(firstPushedValue);
			++currentSize;

			Assert::IsTrue(firstDatum == secondDatum);

			firstDatum.PushBack(secondPushedValue);
			secondDatum.PushBack(secondPushedValue);
			++currentSize;

			Assert::IsTrue(firstDatum == secondDatum);

			firstDatum.PushBack(thirdPushedValue);
			secondDatum.PushBack(firstPushedValue);
			++currentSize;
			Assert::IsFalse(firstDatum == secondDatum);

			secondDatum.Set(thirdPushedValue, currentSize - 1);

			Assert::IsTrue(firstDatum == secondDatum);

			firstDatum.PushBack(thirdPushedValue);
			Assert::IsFalse(firstDatum == secondDatum);

			Datum thirdDatum;
			thirdDatum.SetType(falseDatumType);
			Assert::IsFalse(firstDatum == thirdDatum);

			thirdDatum.PushBack(falseValue);
			Assert::IsFalse(firstDatum == thirdDatum);

			//For External Storage
			Datum externalDatum;
			externalDatum.SetType(datumType);
			Assert::IsFalse(firstDatum == externalDatum);
			externalDatum.SetStorage(&firstPushedValue, 1);

			firstDatum = externalDatum;
			Assert::IsTrue(firstDatum == externalDatum);

			externalDatum = firstDatum;
			Assert::IsTrue(externalDatum == firstDatum);

			secondDatum.Clear();
			secondDatum.PushBack(firstPushedValue);

			Assert::IsTrue(externalDatum == secondDatum);
		}

		TEST_METHOD(DatumOverloadedEqualityOperator)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.0f;
			std::float_t thirdPushedFloat = 30.0f;

			string firstPushedString = "Hello";
			string secondPushedString = "How";
			string thirdPushedString = "Ummm";

			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);
			vec4 thirdPushedVec4(90, 100, 110, 120);

			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);
			mat4x4 thirdPushedMat4x4(thirdPushedVec4, fourthVec4, firstPushedVec4, secondPushedVec4);

			DatumOverloadedEqualityOperator(firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T, DatumType::FLOAT);

			DatumOverloadedEqualityOperator(firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedString, DatumType::FLOAT, DatumType::STRING);

			DatumOverloadedEqualityOperator(firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING, DatumType::GLM_VECTOR4);

			DatumOverloadedEqualityOperator(firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4, DatumType::GLM_MATRIX4X4);

			DatumOverloadedEqualityOperator(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedInt, DatumType::GLM_MATRIX4X4, DatumType::INT32_T);
		}

		TEST_METHOD(DatumOverloadedIntEqualityOperator)
		{
			Datum datum;
			datum.SetType(DatumType::INT32_T);

			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;			

			std::float_t firstPushedFloat = 10.0f;

			Assert::IsFalse(datum == firstPushedInt);

			datum.PushBack(firstPushedInt);
			Assert::IsTrue(datum == firstPushedInt);

			datum.PushBack(firstPushedInt);
			Assert::IsFalse(datum == firstPushedInt);

			//Clearing Datum and checking again
			datum.Clear();
			Assert::IsFalse(datum == firstPushedInt);

			datum.PushBack(firstPushedInt);
			Assert::IsTrue(datum == firstPushedInt);

			Assert::IsFalse(datum == secondPushedInt);
			Assert::IsFalse(datum == firstPushedFloat);

			Assert::IsTrue(datum == firstPushedInt);

			datum.PushBack(firstPushedInt);
			Assert::IsFalse(datum == firstPushedInt);
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState DatumTest::sStartMemState;
}