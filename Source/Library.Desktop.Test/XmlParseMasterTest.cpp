#include "Pch.h"
#include "CppUnitTest.h"
#include "SharedDataFoo.h"
#include "XmlParseHelperStudent.h"
#include "XmlParseHelperTracer.h"
#include <fstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;

namespace LibraryDesktopTest
{
	TEST_CLASS(XmlParseMasterTest)
	{
	public:
		const string xmlFileEmptyTable = "..\\..\\..\\TestXmlFiles\\XmlParseMaster\\TestStudent.xml";

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

		TEST_METHOD(TestXmlParseMasterCharacterData)
		{
			string testXml = "<Student><Name>Nishanth</Name></Student>";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "Nishanth");
			Assert::AreEqual(0U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(2, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);
			Assert::AreEqual(0, sharedDataFoo.Depth());
		}

		TEST_METHOD(TestXmlParseMasterAttributesOne)
		{
			string testXml = "<Student Ola='Hello'></Student>";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "");
			Assert::AreEqual(1U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(1, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(0, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);
			Assert::IsTrue("Hello" == sharedDataFoo.mKeyValuePairs["ola"]);
			Assert::AreEqual(0, sharedDataFoo.Depth());
		}

		TEST_METHOD(TestXmlParseMasterAttributesThree)
		{
			string testXml = "<Student Ola='Hello'><Name Hello='World' Welp='Me' Paul='Rocks'>Nishanth</Name></Student>";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "Nishanth");
			Assert::AreEqual(3U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(2, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);

			Assert::IsTrue("World" == sharedDataFoo.mKeyValuePairs["hello"]);
			Assert::IsTrue("Me" == sharedDataFoo.mKeyValuePairs["welp"]);
			Assert::IsTrue("Rocks" == sharedDataFoo.mKeyValuePairs["paul"]);
			Assert::AreEqual(0, sharedDataFoo.Depth());
		}

		TEST_METHOD(TestXmlParseMasterSetSharedData)
		{
			string testXml = "<Student Ola='Hello'><Name Hello='World' Welp='Me' Paul='Rocks'>Nishanth</Name></Student>";

			SharedDataFoo sharedDataFooTemp;
			XmlParseMaster xmlParseMaster(sharedDataFooTemp);
			sharedDataFooTemp.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			SharedDataFoo sharedDataFoo;
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			xmlParseMaster.SetSharedData(sharedDataFoo);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "Nishanth");
			Assert::AreEqual(3U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(2, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);

			Assert::IsTrue("World" == sharedDataFoo.mKeyValuePairs["hello"]);
			Assert::IsTrue("Me" == sharedDataFoo.mKeyValuePairs["welp"]);
			Assert::IsTrue("Rocks" == sharedDataFoo.mKeyValuePairs["paul"]);
			Assert::AreEqual(0, sharedDataFoo.Depth());
		}

		TEST_METHOD(TestXmlParseMasterMaxDepth)
		{
			string testXml = "<Student><World><Country><USA><Name>Nishanth</Name></USA></Country></World></Student>";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "Nishanth");
			Assert::AreEqual(0U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(5, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(5, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(5, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);
			Assert::AreEqual(0, sharedDataFoo.Depth());
		}

		TEST_METHOD(TextXmlParseMasterDepth)
		{
			string testXml = "<Student><World><Country><USA><Name>Nishanth";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), false);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "Nishanth");
			Assert::AreEqual(0U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(5, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(0, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(5, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);
			Assert::AreEqual(5, sharedDataFoo.Depth());
		}

		TEST_METHOD(TextXmlParseMasterParseFromFile)
		{
			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileEmptyTable);
			Assert::IsTrue(sharedDataFoo.mReadCharacterData == "3216669295");
			Assert::AreEqual(0U, sharedDataFoo.mKeyValuePairs.Size());
			Assert::AreEqual(13, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(13, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(9, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(4, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);
			Assert::AreEqual(0, sharedDataFoo.Depth());
			Assert::IsTrue(xmlParseMaster.GetFileName() == xmlFileEmptyTable);
		}

		TEST_METHOD(TestXmlParseMasterCloneParse)
		{
			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			string testXml = "<Student Ola='Hello'><Name Hello='World' Welp='Me' Paul='Rocks'>Nishanth</Name></Student>";
			XmlParseMaster *clonedXmlParseMaster = xmlParseMaster.Clone();
			clonedXmlParseMaster->Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			SharedDataFoo *clonedSharedDataFoo = static_cast<SharedDataFoo*>(clonedXmlParseMaster->GetSharedData());

			Assert::IsFalse(clonedXmlParseMaster->AddHelper(studentXmlParser));
			Assert::IsFalse(clonedXmlParseMaster->SetSharedData(sharedDataFoo));
			Assert::IsTrue(clonedSharedDataFoo->mReadCharacterData == "Nishanth");
			Assert::AreEqual(3U, clonedSharedDataFoo->mKeyValuePairs.Size());
			Assert::AreEqual(0, clonedSharedDataFoo->Depth());
			Assert::IsTrue(clonedXmlParseMaster->GetFileName() == "");

			delete clonedXmlParseMaster;
		}

		TEST_METHOD(TestXmlParseMasterCloneParseWithFile)
		{
			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);
			
			XmlParseMaster *clonedXmlParseMaster = xmlParseMaster.Clone();
			clonedXmlParseMaster->ParseFromFile(xmlFileEmptyTable);
			SharedDataFoo *clonedSharedDataFoo = static_cast<SharedDataFoo*>(clonedXmlParseMaster->GetSharedData());

			Assert::IsTrue(clonedSharedDataFoo->mReadCharacterData == "3216669295");
			Assert::AreEqual(0U, clonedSharedDataFoo->mKeyValuePairs.Size());
			Assert::AreEqual(0, clonedSharedDataFoo->Depth());
			Assert::IsTrue(clonedXmlParseMaster->GetFileName() == xmlFileEmptyTable);

			delete clonedXmlParseMaster;
		}

		TEST_METHOD(TestXmlParseMasterParseException)
		{
			string testXml = "<Student><Name>Nishanth";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true); });
		}

		TEST_METHOD(TestXmlParseMasterInvalidFileNameException)
		{
			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			string invalidFileName = "..\\..\\..\\TestXmlFiles\\Dummy.xml";
			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(invalidFileName); });
		}

		TEST_METHOD(TestSharedDataClone)
		{
			string testXml = "<Tracer Ola='Hello'><Name Hello='World' Welp='Me' Paul='Rocks'>Nishanth</Name></Tracer>";

			XmlParseMaster::SharedData sharedData;
			XmlParseMaster xmlParseMaster(sharedData);
			sharedData.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);
			XmlParseHelperTracer tracerXmlParser(xmlParseMaster);

			XmlParseMaster::SharedData* clonedSharedData = sharedData.Clone();
			clonedSharedData->Initialize();
			clonedSharedData->SetXmlParseMaster(xmlParseMaster);
			xmlParseMaster.SetSharedData(*clonedSharedData);
			Assert::IsTrue(clonedSharedData->GetXmlParseMaster() == &xmlParseMaster);

			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::AreEqual(2, tracerXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(2, tracerXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, tracerXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(2, tracerXmlParser.mMaxDepth);
			Assert::AreEqual(true, tracerXmlParser.mIsInitialized);
			Assert::AreEqual(0, clonedSharedData->Depth());

			delete clonedSharedData;
		}

		TEST_METHOD(TestSharedDataFooClone)
		{
			string testXml = "<Student Ola='Hello'><Name Hello='World' Welp='Me' Paul='Rocks'>Nishanth</Name></Student>";

			SharedDataFoo sharedDataFoo;
			XmlParseMaster xmlParseMaster(sharedDataFoo);
			sharedDataFoo.SetXmlParseMaster(xmlParseMaster);
			XmlParseHelperStudent studentXmlParser(xmlParseMaster);

			SharedDataFoo* clonedSharedDataFoo = static_cast<SharedDataFoo*>(sharedDataFoo.Clone());
			clonedSharedDataFoo->SetXmlParseMaster(xmlParseMaster);
			xmlParseMaster.SetSharedData(*clonedSharedDataFoo);
			Assert::IsTrue(clonedSharedDataFoo->GetXmlParseMaster() == &xmlParseMaster);


			xmlParseMaster.Parse(testXml, static_cast<uint32_t>(testXml.length()), true);
			Assert::IsTrue(clonedSharedDataFoo->mReadCharacterData == "Nishanth");
			Assert::AreEqual(3U, clonedSharedDataFoo->mKeyValuePairs.Size());
			Assert::AreEqual(2, studentXmlParser.mStartElementHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mEndElementHandlerCount);
			Assert::AreEqual(1, studentXmlParser.mCharDataHandlerCount);
			Assert::AreEqual(2, studentXmlParser.mMaxDepth);
			Assert::AreEqual(true, studentXmlParser.mIsInitialized);

			Assert::IsTrue("World" == clonedSharedDataFoo->mKeyValuePairs["hello"]);
			Assert::IsTrue("Me" == clonedSharedDataFoo->mKeyValuePairs["welp"]);
			Assert::IsTrue("Rocks" == clonedSharedDataFoo->mKeyValuePairs["paul"]);
			Assert::AreEqual(0, clonedSharedDataFoo->Depth());

			delete clonedSharedDataFoo;
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState XmlParseMasterTest::sStartMemState;
}