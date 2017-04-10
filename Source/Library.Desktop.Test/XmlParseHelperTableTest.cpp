#include "Pch.h"
#include "CppUnitTest.h"
#include "SharedDataTable.h"
#include "XmlParseHelperTable.h"
#include "XmlParseHelperPrimitives.h"
#include "Glm/vec4.hpp"
#include "Glm/mat4x4.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace std;

namespace LibraryDesktopTest
{
	TEST_CLASS(XmlParseHelperTableTest)
	{
	public:
		const string xmlFolder = "..\\..\\..\\TestXmlFiles\\XmlParseHelperTable\\";

		const string xmlFileEmpty = xmlFolder + "TestXmlParseHelperEmpty.xml";

		const string xmlFileEmptyTable = xmlFolder + "TestXmlParseHelperEmptyTable.xml";

		const string xmlFilePrimitiveTypes = xmlFolder + "TestXmlParseHelperPrimitiveTypes.xml";

		const string xmlFileInnerScopes = xmlFolder + "TestXmlParseHelperInnerScope.xml";

		const string xmlFileInnerScopesSiblings = xmlFolder + "TestXmlParseHelperInnerScopeSiblings.xml";

		const string xmlFilePrimitiveTypesArray = xmlFolder + "TestXmlParseHelperPrimitiveTypesArray.xml";

		const string xmlFilePrimitiveTypesFormat2 = xmlFolder + "TestXmlParseHelperPrimitiveTypesFormat2.xml";

		const string xmlFileErrorTableOne = xmlFolder + "TestXmlParseHelperErrorTableOne.xml";

		const string xmlFileErrorTableTwo = xmlFolder + "TestXmlParseHelperErrorTableTwo.xml";

		const string xmlFileErrorInvalidPrimitiveOne = xmlFolder + "TestXmlParseHelperInvalidPrimitiveOne.xml";

		const string xmlFileErrorInvalidPrimitiveTwo = xmlFolder + "TestXmlParseHelperInvalidPrimitiveTwo.xml";

		const string xmlFileErrorInvalidPrimitiveThree = xmlFolder + "TestXmlParseHelperInvalidPrimitiveThree.xml";

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

		TEST_METHOD(XmlParseHelperTableEmptyXmlFile)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileEmpty); });
		}

		TEST_METHOD(XmlParseHelperTableEmptyTable)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileEmptyTable);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);
		}

		TEST_METHOD(XmlParseHelperTablePrimitiveTypes)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFilePrimitiveTypes);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);
		}

		TEST_METHOD(XmlParseHelperTableInnerScopes)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileInnerScopes);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);

			Datum* scopeDatum = scope->Find("InnerTable");
			Assert::AreEqual(scopeDatum->Size(), 1U);

			Scope* childScope = scopeDatum->Get<Scope*>();

			Datum* innerIntegerDatum = childScope->Find("SecondInteger");
			Assert::AreEqual(innerIntegerDatum->Get<int32_t>(), 100);
			Assert::AreEqual(innerIntegerDatum->Size(), 1U);

			Datum* innerFloatDatum = childScope->Find("SecondFloat");
			Assert::AreEqual(innerFloatDatum->Get<float_t>(), 200.250f);
			Assert::AreEqual(innerFloatDatum->Size(), 1U);

			Datum* innerStringDatum = childScope->Find("SecondString");
			Assert::IsTrue(innerStringDatum->Get<string>() == "Linus Torvalds");
			Assert::AreEqual(innerStringDatum->Size(), 1U);
		}

		TEST_METHOD(XmlParseHelperTableInnerScopesSiblings)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileInnerScopesSiblings);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);

			//First Sibling
			Datum* firstSiblingScopeDatum = scope->Find("InnerTable");
			Assert::AreEqual(firstSiblingScopeDatum->Size(), 2U);

			Scope* thirdSiblingScope = firstSiblingScopeDatum->Get<Scope*>();

			Datum* firstSiblingIntegerDatum = thirdSiblingScope->Find("SecondInteger");
			Assert::AreEqual(firstSiblingIntegerDatum->Get<int32_t>(), 100);
			Assert::AreEqual(firstSiblingIntegerDatum->Size(), 1U);

			Datum* firstSiblingFloatDatum = thirdSiblingScope->Find("SecondFloat");
			Assert::AreEqual(firstSiblingFloatDatum->Get<float_t>(), 200.250f);
			Assert::AreEqual(firstSiblingFloatDatum->Size(), 1U);

			Datum* firstSiblingStringDatum = thirdSiblingScope->Find("SecondString");
			Assert::IsTrue(firstSiblingStringDatum->Get<string>() == "Linus Torvalds");
			Assert::AreEqual(firstSiblingStringDatum->Size(), 1U);

			//Second Sibling
			Datum* secondSiblingScopeDatum = scope->Find("InnerTable");
			Assert::AreEqual(secondSiblingScopeDatum->Size(), 2U);

			Scope* secondSiblingScope = secondSiblingScopeDatum->Get<Scope*>(1);

			Datum* secondSiblingIntegerDatum = secondSiblingScope->Find("ThirdInteger");
			Assert::AreEqual(secondSiblingIntegerDatum->Get<int32_t>(), 300);
			Assert::AreEqual(secondSiblingIntegerDatum->Size(), 1U);

			Datum* secondSiblingFloatDatum = secondSiblingScope->Find("ThirdFloat");
			Assert::AreEqual(secondSiblingFloatDatum->Get<float_t>(), 300.250f);
			Assert::AreEqual(secondSiblingFloatDatum->Size(), 1U);

			Datum* thirdSiblingStringDatum = secondSiblingScope->Find("ThirdString");
			Assert::IsTrue(thirdSiblingStringDatum->Get<string>() == "Paul Rocks");
			Assert::AreEqual(thirdSiblingStringDatum->Size(), 1U);
		}

		TEST_METHOD(XmlParseHelperTablePrimitiveTypesArrays)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFilePrimitiveTypesArray);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Get<int32_t>(0), 10);
			Assert::AreEqual(integerDatum->Get<int32_t>(1), 20);
			Assert::AreEqual(integerDatum->Get<int32_t>(2), 30);
			Assert::AreEqual(integerDatum->Get<int32_t>(3), 40);
			Assert::AreEqual(integerDatum->Get<int32_t>(4), 50);
			Assert::AreEqual(integerDatum->Size(), 5U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);

			Datum* scopeDatum = scope->Find("InnerTable");
			Assert::AreEqual(scopeDatum->Size(), 1U);

			Scope* childScope = scopeDatum->Get<Scope*>();

			Datum* innerIntegerDatum = childScope->Find("SecondInteger");
			Assert::AreEqual(innerIntegerDatum->Get<int32_t>(), 100);
			Assert::AreEqual(innerIntegerDatum->Size(), 1U);

			Datum* innerFloatDatum = childScope->Find("SecondFloat");
			Assert::AreEqual(innerFloatDatum->Get<float_t>(), 200.250f);
			Assert::AreEqual(innerFloatDatum->Get<float_t>(0), 200.250f);
			Assert::AreEqual(innerFloatDatum->Get<float_t>(1), 220.251f);
			Assert::AreEqual(innerFloatDatum->Size(), 2U);

			Datum* innerStringDatum = childScope->Find("SecondString");
			Assert::IsTrue(innerStringDatum->Get<string>() == "Linus Torvalds");
			Assert::AreEqual(innerStringDatum->Size(), 1U);
		}

		TEST_METHOD(XmlParseHelperTablePrimitivesClone)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			XmlParseHelperTable* clonedTableXmlParser = static_cast<XmlParseHelperTable*>(tableXmlParser.Clone());
			XmlParseHelperPrimitives* clonedPrimitivesParser = static_cast<XmlParseHelperPrimitives*>(primitivesParser.Clone());
			SharedDataTable* clonedSharedDataTable = static_cast<SharedDataTable*>(sharedDataTable.Clone());

			xmlParseMaster.RemoveHelper(tableXmlParser);
			xmlParseMaster.RemoveHelper(primitivesParser);
			xmlParseMaster.SetSharedData(*clonedSharedDataTable);

			clonedTableXmlParser;
			clonedPrimitivesParser;

			xmlParseMaster.ParseFromFile(xmlFileInnerScopes);

			Scope* scope = clonedSharedDataTable->GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);

			Datum* scopeDatum = scope->Find("InnerTable");
			Assert::AreEqual(scopeDatum->Size(), 1U);

			Scope* childScope = scopeDatum->Get<Scope*>();

			Datum* innerIntegerDatum = childScope->Find("SecondInteger");
			Assert::AreEqual(innerIntegerDatum->Get<int32_t>(), 100);
			Assert::AreEqual(innerIntegerDatum->Size(), 1U);

			Datum* innerFloatDatum = childScope->Find("SecondFloat");
			Assert::AreEqual(innerFloatDatum->Get<float_t>(), 200.250f);
			Assert::AreEqual(innerFloatDatum->Size(), 1U);

			Datum* innerStringDatum = childScope->Find("SecondString");
			Assert::IsTrue(innerStringDatum->Get<string>() == "Linus Torvalds");
			Assert::AreEqual(innerStringDatum->Size(), 1U);

			delete clonedTableXmlParser;
			delete clonedPrimitivesParser;
			delete clonedSharedDataTable;
		}

		TEST_METHOD(XmlParseHelperTablePrimitiveTypesFormat2)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFilePrimitiveTypesFormat2);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);
		}

		TEST_METHOD(XmlParseHelperErrorTableOne)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileErrorTableOne); });
		}

		TEST_METHOD(XmlParseHelperErrorTableTwo)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileErrorTableTwo); });
		}

		TEST_METHOD(XmlParseHelperErrorInvalidPrimitiveOne)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileErrorInvalidPrimitiveOne); });
		}

		TEST_METHOD(XmlParseHelperErrorInvalidPrimitiveTwo)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileErrorInvalidPrimitiveTwo); });
		}

		TEST_METHOD(XmlParseHelperErrorInvalidPrimitiveThree)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileErrorInvalidPrimitiveThree); });
		}

		TEST_METHOD(SharedDataClone)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			sharedDataTable.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperPrimitives primitivesParser(xmlParseMaster);
			XmlParseHelperTable tableXmlParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFilePrimitiveTypes);

			Scope* scope = sharedDataTable.GetScope();
			Assert::IsTrue(scope->GetParent() == nullptr);

			Datum* integerDatum = scope->Find("FirstInteger");
			Assert::AreEqual(integerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(integerDatum->Size(), 1U);

			Datum* floatDatum = scope->Find("FirstFloat");
			Assert::AreEqual(floatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(floatDatum->Size(), 1U);

			Datum* stringDatum = scope->Find("FirstString");
			Assert::IsTrue(stringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(stringDatum->Size(), 1U);

			Datum* vec4Datum = scope->Find("FirstVector");
			Assert::IsTrue(vec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(vec4Datum->Size(), 1U);

			Datum* mat4x4Datum = scope->Find("FirstMatrix");

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			Assert::IsTrue(mat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(mat4x4Datum->Size(), 1U);

			//Cloning Shared Data
			SharedDataTable* clonedSharedData = static_cast<SharedDataTable*>(sharedDataTable.Clone());

			Scope* clonedSharedDataScope = clonedSharedData->GetScope();
			Assert::IsTrue(clonedSharedDataScope->GetParent() == nullptr);

			Datum* clonedIntegerDatum = clonedSharedDataScope->Find("FirstInteger");
			Assert::AreEqual(clonedIntegerDatum->Get<int32_t>(), 10);
			Assert::AreEqual(clonedIntegerDatum->Size(), 1U);

			Datum* cloendFloatDatum = clonedSharedDataScope->Find("FirstFloat");
			Assert::AreEqual(cloendFloatDatum->Get<float_t>(), 20.25f);
			Assert::AreEqual(cloendFloatDatum->Size(), 1U);

			Datum* clonedStringDatum = clonedSharedDataScope->Find("FirstString");
			Assert::IsTrue(clonedStringDatum->Get<string>() == "HelloWorld");
			Assert::AreEqual(clonedStringDatum->Size(), 1U);

			Datum* clonedVec4Datum = clonedSharedDataScope->Find("FirstVector");
			Assert::IsTrue(clonedVec4Datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));
			Assert::AreEqual(clonedVec4Datum->Size(), 1U);

			Datum* clonedMat4x4Datum = clonedSharedDataScope->Find("FirstMatrix");

			Assert::IsTrue(clonedMat4x4Datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
			Assert::AreEqual(clonedMat4x4Datum->Size(), 1U);

			delete clonedSharedData;
		}

		TEST_METHOD(SharedDataRTTI)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			XmlParseHelperTable xmlParseHelperTable(xmlParseMaster);
			Assert::IsTrue(sharedDataTable.Is("SharedDataTable"));
			Assert::IsFalse(sharedDataTable.Is("SharedDataTa"));

			RTTI* sharedDataRTTI = sharedDataTable.QueryInterface(sharedDataTable.TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(sharedDataRTTI) == &sharedDataTable);
			Assert::IsTrue(sharedDataTable.Equals(sharedDataRTTI));
			Assert::IsFalse(sharedDataTable.Equals(&xmlParseHelperTable));

			XmlParseMaster::SharedData* newSharedData = sharedDataTable.As<XmlParseMaster::SharedData>();
			Assert::IsTrue(sharedDataTable.Equals(newSharedData));

			Assert::IsTrue(newSharedData->Is(sharedDataTable.TypeIdClass()));
			Assert::IsTrue(newSharedData->Is(SharedDataTable::TypeIdClass()));

			Assert::IsTrue(newSharedData->TypeIdInstance() == SharedDataTable::TypeIdClass());
			Assert::IsTrue(newSharedData->TypeIdInstance() != XmlParseMaster::SharedData::TypeIdClass());
		}

		TEST_METHOD(XmlParseHelperTableRTTI)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			XmlParseHelperTable xmlParseHelperTable(xmlParseMaster);

			Assert::IsTrue(xmlParseHelperTable.Is("XmlParseHelperTable"));
			Assert::IsFalse(xmlParseHelperTable.Is("XmlParseHelperTab"));

			RTTI* xmlParseHelperTableRTTI = xmlParseHelperTable.QueryInterface(xmlParseHelperTable.TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperTableRTTI) == &xmlParseHelperTable);
			Assert::IsTrue(xmlParseHelperTable.Equals(xmlParseHelperTableRTTI));
			Assert::IsFalse(xmlParseHelperTable.Equals(&sharedDataTable));

			IXmlParseHelper* newXmlParseHelperTable = xmlParseHelperTable.As<IXmlParseHelper>();
			Assert::IsTrue(xmlParseHelperTable.Equals(newXmlParseHelperTable));

			Assert::IsTrue(newXmlParseHelperTable->Is(xmlParseHelperTable.TypeIdClass()));
			Assert::IsTrue(newXmlParseHelperTable->Is(XmlParseHelperTable::TypeIdClass()));

			Assert::IsTrue(newXmlParseHelperTable->TypeIdInstance() == XmlParseHelperTable::TypeIdClass());
			Assert::IsTrue(newXmlParseHelperTable->TypeIdInstance() != IXmlParseHelper::TypeIdClass());
		}

		TEST_METHOD(XmlParseHelperPrimitiveRTTI)
		{
			SharedDataTable sharedDataTable;
			XmlParseMaster xmlParseMaster(sharedDataTable);
			XmlParseHelperPrimitives xmlParseHelperPrimitives(xmlParseMaster);

			Assert::IsTrue(xmlParseHelperPrimitives.Is("XmlParseHelperPrimitives"));
			Assert::IsFalse(xmlParseHelperPrimitives.Is("XmlParseHelperPrimiti"));

			RTTI* xmlParseHelperPrimitivesRTTI = xmlParseHelperPrimitives.QueryInterface(xmlParseHelperPrimitives.TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperPrimitivesRTTI) == &xmlParseHelperPrimitives);
			Assert::IsTrue(xmlParseHelperPrimitives.Equals(xmlParseHelperPrimitivesRTTI));
			Assert::IsFalse(xmlParseHelperPrimitives.Equals(&sharedDataTable));

			IXmlParseHelper* newXmlParseHelperPrimitives = xmlParseHelperPrimitives.As<IXmlParseHelper>();
			Assert::IsTrue(xmlParseHelperPrimitives.Equals(newXmlParseHelperPrimitives));

			Assert::IsTrue(newXmlParseHelperPrimitives->Is(xmlParseHelperPrimitives.TypeIdClass()));
			Assert::IsTrue(newXmlParseHelperPrimitives->Is(XmlParseHelperPrimitives::TypeIdClass()));

			Assert::IsTrue(newXmlParseHelperPrimitives->TypeIdInstance() == XmlParseHelperPrimitives::TypeIdClass());
			Assert::IsTrue(newXmlParseHelperPrimitives->TypeIdInstance() != IXmlParseHelper::TypeIdClass());
		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState XmlParseHelperTableTest::sStartMemState;
}