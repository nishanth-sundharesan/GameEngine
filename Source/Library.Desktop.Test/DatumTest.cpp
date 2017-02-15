#include "Pch.h"
#include "CppUnitTest.h"
#include "Datum.h"

#pragma warning ( push )
#pragma warning ( disable: 4201 )							//Suppressing the warning message "nonstandard extension used : nameless struct / union" in the file"
#include <Glm/gtx/string_cast.hpp>
#pragma warning ( pop )

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

			Assert::ExpectException<exception>([&] {datum.SetType(DatumType::UNASSIGNED); });
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

		template<typename T1, typename T2>
		void DatumOverloadedEqualityOperatorEachType(T1 firstPushedValue, T1 secondPushedValue, T2 falseValue, DatumType datumType)
		{
			Datum datum;
			datum.SetType(datumType);

			Assert::IsFalse(datum == firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsTrue(datum == firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsFalse(datum == firstPushedValue);

			//Clearing Datum and checking again
			datum.Clear();
			Assert::IsFalse(datum == firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsTrue(datum == firstPushedValue);

			Assert::IsFalse(datum == secondPushedValue);
			Assert::IsFalse(datum == falseValue);

			Assert::IsTrue(datum == firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsFalse(datum == firstPushedValue);
		}

		TEST_METHOD(DatumOverloadedEqualityOperatorEachType)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.0f;

			string firstPushedString = "Hello";
			string secondPushedString = "How";

			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);

			vec4 thirdPushedVec4(90, 100, 110, 120);
			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);

			DatumOverloadedEqualityOperatorEachType(firstPushedInt, secondPushedInt, firstPushedFloat, DatumType::INT32_T);

			DatumOverloadedEqualityOperatorEachType(firstPushedFloat, secondPushedFloat, firstPushedString, DatumType::FLOAT);

			DatumOverloadedEqualityOperatorEachType(firstPushedString, secondPushedString, firstPushedVec4, DatumType::STRING);

			DatumOverloadedEqualityOperatorEachType(firstPushedVec4, secondPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4);

			DatumOverloadedEqualityOperatorEachType(firstPushedMat4x4, secondPushedMat4x4, secondPushedString, DatumType::GLM_MATRIX4X4);
		}

		template<typename T1, typename T2>
		void DatumOverloadedInEqualityOperator(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType, DatumType falseDatumType)
		{
			Datum firstDatum;
			Datum secondDatum;

			Assert::IsFalse(firstDatum != secondDatum);

			//For Internal Storage
			firstDatum.SetType(datumType);
			secondDatum.SetType(datumType);

			uint32_t currentSize = 0;
			firstDatum.PushBack(firstPushedValue);
			secondDatum.PushBack(firstPushedValue);
			++currentSize;

			Assert::IsFalse(firstDatum != secondDatum);

			firstDatum.PushBack(secondPushedValue);
			secondDatum.PushBack(secondPushedValue);
			++currentSize;

			Assert::IsFalse(firstDatum != secondDatum);

			firstDatum.PushBack(thirdPushedValue);
			secondDatum.PushBack(firstPushedValue);
			++currentSize;
			Assert::IsTrue(firstDatum != secondDatum);

			secondDatum.Set(thirdPushedValue, currentSize - 1);

			Assert::IsFalse(firstDatum != secondDatum);

			firstDatum.PushBack(thirdPushedValue);
			Assert::IsTrue(firstDatum != secondDatum);

			Datum thirdDatum;
			thirdDatum.SetType(falseDatumType);
			Assert::IsTrue(firstDatum != thirdDatum);

			thirdDatum.PushBack(falseValue);
			Assert::IsTrue(firstDatum != thirdDatum);

			//For External Storage
			Datum externalDatum;
			externalDatum.SetType(datumType);
			Assert::IsTrue(firstDatum != externalDatum);
			externalDatum.SetStorage(&firstPushedValue, 1);

			firstDatum = externalDatum;
			Assert::IsFalse(firstDatum != externalDatum);

			externalDatum = firstDatum;
			Assert::IsFalse(externalDatum != firstDatum);

			secondDatum.Clear();
			secondDatum.PushBack(firstPushedValue);

			Assert::IsFalse(externalDatum != secondDatum);
		}

		TEST_METHOD(DatumOverloadedInEqualityOperator)
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

			DatumOverloadedInEqualityOperator(firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T, DatumType::FLOAT);

			DatumOverloadedInEqualityOperator(firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedString, DatumType::FLOAT, DatumType::STRING);

			DatumOverloadedInEqualityOperator(firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING, DatumType::GLM_VECTOR4);

			DatumOverloadedInEqualityOperator(firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4, DatumType::GLM_MATRIX4X4);

			DatumOverloadedInEqualityOperator(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedInt, DatumType::GLM_MATRIX4X4, DatumType::INT32_T);
		}

		template<typename T1, typename T2>
		void DatumOverloadedInEqualityOperatorEachType(T1 firstPushedValue, T1 secondPushedValue, T2 falseValue, DatumType datumType)
		{
			Datum datum;
			datum.SetType(datumType);

			Assert::IsTrue(datum != firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsFalse(datum != firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsTrue(datum != firstPushedValue);

			//Clearing Datum and checking again
			datum.Clear();
			Assert::IsTrue(datum != firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsFalse(datum != firstPushedValue);

			Assert::IsTrue(datum != secondPushedValue);
			Assert::IsTrue(datum != falseValue);

			Assert::IsFalse(datum != firstPushedValue);

			datum.PushBack(firstPushedValue);
			Assert::IsTrue(datum != firstPushedValue);
		}

		TEST_METHOD(DatumOverloadedInEqualityOperatorEachType)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.0f;

			string firstPushedString = "Hello";
			string secondPushedString = "How";

			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);

			vec4 thirdPushedVec4(90, 100, 110, 120);
			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);

			DatumOverloadedInEqualityOperatorEachType(firstPushedInt, secondPushedInt, firstPushedFloat, DatumType::INT32_T);

			DatumOverloadedInEqualityOperatorEachType(firstPushedFloat, secondPushedFloat, firstPushedString, DatumType::FLOAT);

			DatumOverloadedInEqualityOperatorEachType(firstPushedString, secondPushedString, firstPushedVec4, DatumType::STRING);

			DatumOverloadedInEqualityOperatorEachType(firstPushedVec4, secondPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4);

			DatumOverloadedInEqualityOperatorEachType(firstPushedMat4x4, secondPushedMat4x4, secondPushedString, DatumType::GLM_MATRIX4X4);
		}

		template<typename T1, typename T2>
		void DatumSet(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType, DatumType falseDatumType)
		{
			Datum datum;
			uint32_t currentSize = 0;

			Assert::ExpectException<exception>([&] {datum.Set(firstPushedValue); });

			datum.SetType(datumType);
			Assert::ExpectException<exception>([&] {datum.Set(firstPushedValue); });

			datum.PushBack(firstPushedValue);
			++currentSize;

			datum.Set(firstPushedValue);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			datum.Set(firstPushedValue, currentSize - 1);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			datum.Set(firstPushedValue);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			Assert::ExpectException<exception>([&] {datum.Set(firstPushedValue, currentSize); });
			Assert::ExpectException<exception>([&] {datum.Set(firstPushedValue, currentSize + 10); });

			datum.PushBack(secondPushedValue);
			++currentSize;
			datum.PushBack(thirdPushedValue);
			++currentSize;

			datum.Set(secondPushedValue, currentSize - 2);
			Assert::IsTrue(datum.Get<T1>(currentSize - 2) == secondPushedValue);

			datum.Set(thirdPushedValue, currentSize - 1);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == thirdPushedValue);
			datum.Clear();
			currentSize = 0;

			//Checking for external Datum
			Datum externalDatum;
			externalDatum.SetType(datumType);
			currentSize = 1;
			externalDatum.SetStorage(&firstPushedValue, currentSize);

			externalDatum.Set(secondPushedValue, currentSize - 1);
			Assert::IsTrue(externalDatum.Get<T1>(currentSize - 1) == secondPushedValue);

			//Trying to Set Datum of different type
			Datum falseDatum;
			falseDatum.SetType(falseDatumType);
			falseDatum.PushBack(falseValue);

			Assert::ExpectException<exception>([&] {falseDatum.Set(firstPushedValue); });
		}

		TEST_METHOD(DatumSet)
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

			DatumSet(firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T, DatumType::FLOAT);

			DatumSet(firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedString, DatumType::FLOAT, DatumType::STRING);

			DatumSet(firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING, DatumType::GLM_VECTOR4);

			DatumSet(firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4, DatumType::GLM_MATRIX4X4);

			DatumSet(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedInt, DatumType::GLM_MATRIX4X4, DatumType::INT32_T);
		}

		template<typename T1, typename T2>
		void DatumGet(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, T2 falseValue, DatumType datumType, DatumType falseDatumType)
		{
			Datum datum;
			uint32_t currentSize = 0;
			Assert::ExpectException<exception>([&] {datum.Get<T1>(); });
			Assert::ExpectException<exception>([&] {datum.Get<T1>(2); });

			datum.SetType(datumType);
			Assert::ExpectException<exception>([&] {datum.Get<T1>(0); });

			datum.PushBack(firstPushedValue);
			++currentSize;
			Assert::IsTrue(datum.Get<T1>() == firstPushedValue);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == firstPushedValue);

			datum.PushBack(secondPushedValue);
			++currentSize;
			datum.PushBack(thirdPushedValue);
			++currentSize;
			Assert::IsTrue(datum.Get<T1>() == firstPushedValue);
			Assert::IsTrue(datum.Get<T1>(currentSize - 2) == secondPushedValue);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == thirdPushedValue);

			Assert::ExpectException<exception>([&] {datum.Get<T1>(currentSize); });
			Assert::ExpectException<exception>([&] {datum.Get<T1>(currentSize + 1); });
			Assert::ExpectException<exception>([&] {datum.Get<T1>(currentSize + 10); });

			Datum externalDatum;
			externalDatum.SetType(datumType);
			Assert::ExpectException<exception>([&] {externalDatum.Get<T1>(0); });

			externalDatum.SetStorage(&firstPushedValue, 1);
			currentSize = 1;
			Assert::IsTrue(externalDatum.Get<T1>(currentSize - 1) == firstPushedValue);

			Datum falseDatum;
			currentSize = 0;
			falseDatum.SetType(falseDatumType);
			Assert::ExpectException<exception>([&] {datum.Get<T2>(currentSize); });

			falseDatum.PushBack(falseValue);
			++currentSize;
			Assert::IsTrue(falseDatum.Get<T2>(currentSize - 1) == falseValue);
			Assert::ExpectException<exception>([&] {falseDatum.Get<T1>(currentSize - 1); });
		}

		TEST_METHOD(DatumGet)
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

			DatumGet(firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedFloat, DatumType::INT32_T, DatumType::FLOAT);

			DatumGet(firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedString, DatumType::FLOAT, DatumType::STRING);

			DatumGet(firstPushedString, secondPushedString, thirdPushedString, firstPushedVec4, DatumType::STRING, DatumType::GLM_VECTOR4);

			DatumGet(firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedMat4x4, DatumType::GLM_VECTOR4, DatumType::GLM_MATRIX4X4);

			DatumGet(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedInt, DatumType::GLM_MATRIX4X4, DatumType::INT32_T);
		}

		template<typename T1>
		void DatumToStringIntAndFloat(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, DatumType datumType)
		{
			uint32_t currentSize = 0;
			Datum datum;
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.SetType(datumType);
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.PushBack(firstPushedValue);
			++currentSize;
			Assert::IsTrue(datum.ToString() == std::to_string(firstPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 1) == std::to_string(firstPushedValue));

			datum.PushBack(secondPushedValue);
			++currentSize;
			datum.PushBack(thirdPushedValue);
			++currentSize;

			Assert::IsTrue(datum.ToString() == std::to_string(firstPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 2) == std::to_string(secondPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 1) == std::to_string(thirdPushedValue));

			Assert::ExpectException<exception>([&] {datum.ToString(currentSize); });
			Assert::ExpectException<exception>([&] {datum.ToString(currentSize + 1); });

			datum.Clear();
			Assert::ExpectException<exception>([&] {datum.ToString(); });
		}

		TEST_METHOD(DatumToStringIntAndFloat)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			DatumToStringIntAndFloat(firstPushedInt, secondPushedInt, thirdPushedInt, DatumType::INT32_T);

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.22f;
			std::float_t thirdPushedFloat = 30.31f;

			DatumToStringIntAndFloat(firstPushedFloat, secondPushedFloat, thirdPushedFloat, DatumType::FLOAT);
		}

		TEST_METHOD(DatumToStringString)
		{
			string firstPushedString = "Hello";
			string secondPushedString = "How";
			string thirdPushedString = "Ummm";

			uint32_t currentSize = 0;
			Datum datum;
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.SetType(DatumType::STRING);
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.PushBack(firstPushedString);
			++currentSize;
			Assert::IsTrue(datum.ToString() == firstPushedString);
			Assert::IsTrue(datum.ToString(currentSize - 1) == firstPushedString);

			datum.PushBack(secondPushedString);
			++currentSize;
			datum.PushBack(thirdPushedString);
			++currentSize;

			Assert::IsTrue(datum.ToString() == firstPushedString);
			Assert::IsTrue(datum.ToString(currentSize - 2) == secondPushedString);
			Assert::IsTrue(datum.ToString(currentSize - 1) == thirdPushedString);

			Assert::ExpectException<exception>([&] {datum.ToString(currentSize); });
			Assert::ExpectException<exception>([&] {datum.ToString(currentSize + 1); });

			datum.Clear();
			Assert::ExpectException<exception>([&] {datum.ToString(); });
		}

		template<typename T1>
		void DatumToStringGlmVec4AndMat4x4(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, DatumType datumType)
		{
			uint32_t currentSize = 0;
			Datum datum;
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.SetType(datumType);
			Assert::ExpectException<exception>([&] {datum.ToString(); });

			datum.PushBack(firstPushedValue);
			++currentSize;
			Assert::IsTrue(datum.ToString() == glm::to_string(firstPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 1) == glm::to_string(firstPushedValue));

			datum.PushBack(secondPushedValue);
			++currentSize;
			datum.PushBack(thirdPushedValue);
			++currentSize;

			Assert::IsTrue(datum.ToString() == glm::to_string(firstPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 2) == glm::to_string(secondPushedValue));
			Assert::IsTrue(datum.ToString(currentSize - 1) == glm::to_string(thirdPushedValue));

			Assert::ExpectException<exception>([&] {datum.ToString(currentSize); });
			Assert::ExpectException<exception>([&] {datum.ToString(currentSize + 1); });

			datum.Clear();
			Assert::ExpectException<exception>([&] {datum.ToString(); });
		}

		TEST_METHOD(DatumToStringGlmVec4AndMat4x4)
		{
			vec4 firstPushedVec4(10, 20, 30, 40);
			vec4 secondPushedVec4(50, 60, 70, 80);
			vec4 thirdPushedVec4(90, 100, 110, 120);

			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);
			mat4x4 thirdPushedMat4x4(thirdPushedVec4, fourthVec4, firstPushedVec4, secondPushedVec4);

			DatumToStringGlmVec4AndMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, DatumType::GLM_VECTOR4);

			DatumToStringGlmVec4AndMat4x4(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, DatumType::GLM_MATRIX4X4);
		}

		template<typename T1>
		void DatumSetFromString(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, string firstPushedString, string secondPushedString, string thirdPushedString, DatumType datumType)
		{
			Datum datum;
			uint32_t currentSize = 0;
			Assert::ExpectException<exception>([&] {datum.SetFromString(firstPushedString); });
			datum.SetType(datumType);
			Assert::ExpectException<exception>([&] {datum.SetFromString(firstPushedString); });

			datum.PushBack(thirdPushedValue);
			++currentSize;
			Assert::IsTrue(datum.Get<T1>() == thirdPushedValue);

			datum.SetFromString(firstPushedString);
			Assert::IsTrue(datum.Get<T1>() == firstPushedValue);

			Assert::ExpectException<exception>([&] {datum.SetFromString(secondPushedString, currentSize); });

			datum.SetFromString(secondPushedString);
			Assert::IsTrue(datum.Get<T1>() == secondPushedValue);

			datum.SetFromString(firstPushedString);
			Assert::IsTrue(datum.Get<T1>() == firstPushedValue);

			datum.PushBack(firstPushedValue);
			++currentSize;
			datum.PushBack(firstPushedValue);
			++currentSize;

			datum.SetFromString(secondPushedString, currentSize - 2);
			Assert::IsTrue(datum.Get<T1>(currentSize - 2) == secondPushedValue);

			datum.SetFromString(thirdPushedString, currentSize - 1);
			Assert::IsTrue(datum.Get<T1>(currentSize - 1) == thirdPushedValue);

			Assert::ExpectException<exception>([&] {datum.SetFromString(secondPushedString, currentSize); });
			Assert::ExpectException<exception>([&] {datum.SetFromString(secondPushedString, currentSize + 1); });
			Assert::ExpectException<exception>([&] {datum.SetFromString(secondPushedString, currentSize + 10); });

			datum.Clear();
			currentSize = 0;
			Assert::ExpectException<exception>([&] {datum.SetFromString(firstPushedString); });
			Assert::ExpectException<exception>([&] {datum.Get<int32_t>(currentSize); });
			Assert::ExpectException<exception>([&] {datum.Get<int32_t>(currentSize + 1); });
			Assert::ExpectException<exception>([&] {datum.Get<int32_t>(currentSize - 1); });

			//Checking for the external Datum
			Datum externalDatum;
			externalDatum.SetType(datumType);
			externalDatum.SetStorage(&firstPushedValue, 1);

			externalDatum.SetFromString(firstPushedString);
			Assert::IsTrue(externalDatum.Get<T1>() == firstPushedValue);
		}

		TEST_METHOD(DatumSetFromString)
		{
			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			string firstPushedIntString = std::to_string(firstPushedInt);
			string secondPushedIntString = std::to_string(secondPushedInt);
			string thirdPushedIntString = std::to_string(thirdPushedInt);

			DatumSetFromString(firstPushedInt, secondPushedInt, thirdPushedInt, firstPushedIntString, secondPushedIntString, thirdPushedIntString, DatumType::INT32_T);

			std::float_t firstPushedFloat = 10.0f;
			std::float_t secondPushedFloat = 20.22f;
			std::float_t thirdPushedFloat = 30.31f;

			string firstPushedFloatString = std::to_string(firstPushedFloat);
			string secondPushedFloatString = std::to_string(secondPushedFloat);
			string thirdPushedFloatString = std::to_string(thirdPushedFloat);

			DatumSetFromString(firstPushedFloat, secondPushedFloat, thirdPushedFloat, firstPushedFloatString, secondPushedFloatString, thirdPushedFloatString, DatumType::FLOAT);

			string firstPushedString = "Hello";
			string secondPushedString = "How";
			string thirdPushedString = "Ummm";

			DatumSetFromString(firstPushedString, secondPushedString, thirdPushedString, firstPushedString, secondPushedString, thirdPushedString, DatumType::STRING);

			vec4 firstPushedVec4(10.2f, 20.9f, 30.6f, 40.7f);
			vec4 secondPushedVec4(50.2f, 60.1f, 79.9f, 80.0f);
			vec4 thirdPushedVec4(90.22f, 100, 110, 120);

			string firstPushedVec4String = glm::to_string(firstPushedVec4);
			string secondPushedVec4String = glm::to_string(secondPushedVec4);
			string thirdPushedVec4String = glm::to_string(thirdPushedVec4);

			DatumSetFromString(firstPushedVec4, secondPushedVec4, thirdPushedVec4, firstPushedVec4String, secondPushedVec4String, thirdPushedVec4String, DatumType::GLM_VECTOR4);

			vec4 fourthVec4(90, 100, 110, 120);
			mat4x4 firstPushedMat4x4(firstPushedVec4, secondPushedVec4, thirdPushedVec4, fourthVec4);
			mat4x4 secondPushedMat4x4(secondPushedVec4, thirdPushedVec4, fourthVec4, firstPushedVec4);
			mat4x4 thirdPushedMat4x4(thirdPushedVec4, fourthVec4, firstPushedVec4, secondPushedVec4);

			string firstPushedMat4x4String = glm::to_string(firstPushedMat4x4);
			string secondPushedMat4x4String = glm::to_string(secondPushedMat4x4);
			string thirdPushedMat4x4String = glm::to_string(thirdPushedMat4x4);

			DatumSetFromString(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, firstPushedMat4x4String, secondPushedMat4x4String, thirdPushedMat4x4String, DatumType::GLM_MATRIX4X4);
		}

		TEST_METHOD(DatumShrinkToFit)
		{
			Datum datum;
			uint32_t currentSize = 0;
			datum.ShrinkToFit();
			datum.SetType(DatumType::INT32_T);

			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			datum.PushBack(firstPushedInt);
			++currentSize;
			datum.PushBack(secondPushedInt);
			++currentSize;
			datum.PushBack(thirdPushedInt);
			++currentSize;

			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);
			datum.ShrinkToFit();
			DatumSizeAndTypeTest(datum, currentSize, DatumType::INT32_T);

			//Checking for the external Datum
			Datum externalDatum;
			currentSize = 1;
			externalDatum.SetType(DatumType::INT32_T);
			externalDatum.SetStorage(&firstPushedInt, currentSize);
			DatumSizeAndTypeTest(externalDatum, currentSize, DatumType::INT32_T);
			externalDatum.ShrinkToFit();
			DatumSizeAndTypeTest(externalDatum, currentSize, DatumType::INT32_T);
		}

		template<typename T1>
		void DatumCopyConstructor(T1 firstPushedValue, T1 secondPushedValue, T1 thirdPushedValue, DatumType datumType)
		{
			Datum datum;
			datum.SetType(datumType);

			Datum datumCopiedOne(datum);
			Assert::IsTrue(datum == datumCopiedOne);

			datum.PushBack(firstPushedValue);
			datum.PushBack(secondPushedValue);
			datum.PushBack(thirdPushedValue);

			Datum datumCopiedTwo(datum);
			Assert::IsTrue(datum == datumCopiedTwo);

			datum.Clear();
			datumCopiedTwo.Clear();
			Assert::IsTrue(datum == datumCopiedTwo);

			datum.PushBack(firstPushedValue);
			datum.PushBack(secondPushedValue);

			Datum datumCopiedThree(datum);
			Assert::IsTrue(datum == datumCopiedThree);
		}

		TEST_METHOD(DatumCopyConstructor)
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

			DatumCopyConstructor(firstPushedInt, secondPushedInt, thirdPushedInt, DatumType::INT32_T);

			DatumCopyConstructor(firstPushedFloat, secondPushedFloat, thirdPushedFloat, DatumType::FLOAT);

			DatumCopyConstructor(firstPushedString, secondPushedString, thirdPushedString, DatumType::STRING);

			DatumCopyConstructor(firstPushedVec4, secondPushedVec4, thirdPushedVec4, DatumType::GLM_VECTOR4);

			DatumCopyConstructor(firstPushedMat4x4, secondPushedMat4x4, thirdPushedMat4x4, DatumType::GLM_MATRIX4X4);
		}

		TEST_METHOD(DatumAssignmentOperator)
		{
			Datum datum;

			Datum datumOne;
			datumOne = datum;
			Assert::IsTrue(datum == datumOne);

			int32_t firstPushedInt = 10;
			int32_t secondPushedInt = 20;
			int32_t thirdPushedInt = 30;

			datum.SetType(DatumType::INT32_T);

			datum.PushBack(firstPushedInt);
			datum.PushBack(secondPushedInt);
			datum.PushBack(thirdPushedInt);

			Datum datumtwo;
			datumtwo = datum;
			Assert::IsTrue(datum == datumtwo);

			Datum externalDatum;
			externalDatum.SetType(DatumType::INT32_T);
			externalDatum.SetStorage(&firstPushedInt, 1);

			Datum datumThree;
			datumThree = externalDatum;
			Assert::IsTrue(datumThree == externalDatum);
			
			Datum externalDatumTwo;
			externalDatumTwo.SetType(DatumType::INT32_T);
			externalDatumTwo.SetStorage(&secondPushedInt, 1);
			
			externalDatum = externalDatumTwo;
			Assert::IsTrue(externalDatum == externalDatumTwo);

			externalDatum = datum;
			Assert::IsTrue(externalDatum == datum);
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState DatumTest::sStartMemState;
}