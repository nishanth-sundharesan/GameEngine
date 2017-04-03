#include "Pch.h"
#include "CppUnitTest.h"
#include "XmlParseHelperWSE.h"
#include "MonsterEntity.h"
#include "Glm/vec4.hpp"
#include "Glm/mat4x4.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;

namespace LibraryDesktopTest
{
	TEST_CLASS(XmlParseHelperWSETest)
	{
	public:
		//Tests for Valid Xml
		const string xmlFolder = "..\\..\\..\\TestXmlFiles\\WorldSectorEntity\\";

		const string xmlFileEmptyWSE = xmlFolder + "TestEmptyWSE.xml";

		const string xmlFileWSEPrimitives = xmlFolder + "TestEmptyWSEWithPrimitives.xml";

		const string xmlFileWSEPrimitivesArrays = xmlFolder + "TestEmptyWSEWithPrimitivesArray.xml";

		const string xmlFileWSEPrimitivesArraysMixed = xmlFolder + "TestEmptyWSEWithPrimitivesArraysMixed.xml";

		//Tests for Invalid Xml
		const string xmlFileInvalidEntity = xmlFolder + "TestInvalidEntity.xml";

		const string xmlFileInvalidPrimitive = xmlFolder + "TestInvalidPrimitive.xml";

		const string xmlFileInvalidSector = xmlFolder + "TestInvalidSector.xml";

		const string xmlFileInvalidTwoWorlds = xmlFolder + "TestInvalidTwoWorlds.xml";

		const string xmlFileInvalidWSE = xmlFolder + "TestInvalidWSE.xml";

		const string xmlFileInvalidSectorWithoutName = xmlFolder + "TestInvalidSectorWithoutName.xml";

		const string xmlFileInvalidEntityWithoutName = xmlFolder + "TestInvalidEntityWithoutName.xml";

		const string xmlFileInvalidWSEWithData = xmlFolder + "TestInvalidWSEWithData.xml";

		const string xmlFileWSEInvalidPrimitive = xmlFolder + "TestWSEInvalidPrimitive.xml";

		const string xmlFileTestInvalidWorldWithoutName = xmlFolder + "TestInvalidWorldWithoutName.xml";

		const string xmlFileTestWSEInvalidGlmPrimitive = xmlFolder + "TestWSEInvalidGlmPrimitive.xml";

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

		//Tests for Valid Xml
		TEST_METHOD(TestXmlParseHelperEmptyWSE)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileEmptyWSE);

			World* world = sharedDataWSE.GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");
			Datum& sectorDatum = world->Sectors();
			Assert::IsTrue(sectorDatum.Size() == 1U);

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");
			Datum& entityDatum = sectorScope->Entities();
			Assert::IsTrue(entityDatum.Size() == 1U);

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");
		}

		TEST_METHOD(TestXmlParseHelperWSEPrimitives)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileWSEPrimitives);

			//World
			World* world = sharedDataWSE.GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");
			Datum& sectorDatum = world->Sectors();
			Assert::IsTrue(sectorDatum.Size() == 1U);

			Datum* datum = world->Find("FirstInteger");
			Assert::IsTrue(datum->Get<int32_t>() == 10);

			datum = world->Find("FirstFloat");
			Assert::IsTrue(datum->Get<float_t>() == 20.25);

			datum = world->Find("FirstString");
			Assert::IsTrue(datum->Get<string>() == "HelloWorld");

			datum = world->Find("FirstVector");
			Assert::IsTrue(datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));

			glm::vec4 firstVector(1034.77632, 91238.1202, 0890.012, 1.960);
			glm::vec4 secondVector(12.2, 6.2, 2.6, 17.77);
			glm::vec4 thirdVector(77.77, 19.21, 99.6912, 69.96);
			glm::vec4 fourthVector(41.44, 32.201, 05.0, 69.00);

			datum = world->Find("FirstMatrix");
			Assert::IsTrue(datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));

			//Sector
			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");
			Datum& entityDatum = sectorScope->Entities();
			Assert::IsTrue(entityDatum.Size() == 1U);

			datum = sectorScope->Find("FirstInteger");
			Assert::IsTrue(datum->Get<int32_t>() == 10);

			datum = sectorScope->Find("FirstFloat");
			Assert::IsTrue(datum->Get<float_t>() == 20.25);

			datum = sectorScope->Find("FirstString");
			Assert::IsTrue(datum->Get<string>() == "HelloWorld");

			datum = sectorScope->Find("FirstVector");
			Assert::IsTrue(datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));

			datum = sectorScope->Find("FirstMatrix");
			Assert::IsTrue(datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));

			//Entity
			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");

			datum = entityScope->Find("FirstInteger");
			Assert::IsTrue(datum->Get<int32_t>() == 10);

			datum = entityScope->Find("FirstFloat");
			Assert::IsTrue(datum->Get<float_t>() == 20.25);

			datum = entityScope->Find("FirstString");
			Assert::IsTrue(datum->Get<string>() == "HelloWorld");

			datum = entityScope->Find("FirstVector");
			Assert::IsTrue(datum->Get<glm::vec4>() == glm::vec4(12.2, 6.2, 2.6, 17.77));

			datum = entityScope->Find("FirstMatrix");
			Assert::IsTrue(datum->Get<glm::mat4x4>() == glm::mat4x4(firstVector, secondVector, thirdVector, fourthVector));
		}

		TEST_METHOD(TestXmlParseHelperWSEPrimitivesArrays)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileWSEPrimitivesArrays);

			//World
			World* world = sharedDataWSE.GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");

			Datum* datum = world->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Sector
			Sector* sectorScope = static_cast<Sector*>(world->Sectors().Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");

			datum = sectorScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Entity
			Entity* entityScope = static_cast<Entity*>(sectorScope->Entities().Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");

			datum = entityScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);
		}

		TEST_METHOD(TestXmlParseHelperWSEPrimitivesArraysMixed)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileWSEPrimitivesArraysMixed);

			//World
			World* world = sharedDataWSE.GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");

			Datum* datum = world->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Sector
			Sector* sectorScope = static_cast<Sector*>(world->Sectors().Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");

			datum = sectorScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Entity
			Entity* entityScope = static_cast<Entity*>(sectorScope->Entities().Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");

			datum = entityScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);
		}

		TEST_METHOD(TestSharedDataWSEClone)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			//Cloning SharedDataWSE
			SharedDataWSE* clonedSharedDataWSE = static_cast<SharedDataWSE*>(sharedDataWSE.Clone());
			xmlParseMaster.SetSharedData(*clonedSharedDataWSE);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileWSEPrimitivesArraysMixed);

			//World
			World* world = clonedSharedDataWSE->GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");

			Datum* datum = world->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Sector
			Sector* sectorScope = static_cast<Sector*>(world->Sectors().Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");

			datum = sectorScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Entity
			Entity* entityScope = static_cast<Entity*>(sectorScope->Entities().Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");

			datum = entityScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			delete clonedSharedDataWSE;
		}

		TEST_METHOD(TestXmlParseHelperWSEClone)
		{
			MonsterEntityFactory monsterFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);		

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			//Cloning XmlParseHelperWSE*
			XmlParseHelperWSE* clonedWseXmlParseHelper = static_cast<XmlParseHelperWSE*>(wseXmlParseHelper.Clone());
			xmlParseMaster.RemoveHelper(wseXmlParseHelper);
			xmlParseMaster.AddHelper(*clonedWseXmlParseHelper);

			xmlParseMaster.ParseFromFile(xmlFileWSEPrimitivesArraysMixed);

			//World
			World* world = sharedDataWSE.GetWorld();
			Assert::IsTrue(world->Name() == "FIEA");

			Datum* datum = world->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Sector
			Sector* sectorScope = static_cast<Sector*>(world->Sectors().Get<Scope*>(0));
			Assert::IsTrue(sectorScope->Name() == "Prog2");

			datum = sectorScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			//Entity
			Entity* entityScope = static_cast<Entity*>(sectorScope->Entities().Get<Scope*>(0));
			Assert::IsTrue(entityScope->Name() == "Monster");

			datum = entityScope->Find("FirstInteger");
			Assert::AreEqual(datum->Size(), 3U);

			Assert::IsTrue(datum->Get<int32_t>(0) == 10);
			Assert::IsTrue(datum->Get<int32_t>(1) == 20);
			Assert::IsTrue(datum->Get<int32_t>(2) == 30);

			delete clonedWseXmlParseHelper;
		}

		//Tests for Invalid Xml
		TEST_METHOD(TestXmlFileInvalidEntity)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidEntity); });			
		}

		TEST_METHOD(TestXmlFileInvalidPrimitive)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidPrimitive); });
		}

		TEST_METHOD(TestXmlFileInvalidSector)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidSector); });
		}

		TEST_METHOD(TestXmlFileInvalidTwoWorlds)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidTwoWorlds); });
		}

		TEST_METHOD(TestXmlFileInvalidWSE)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidWSE); });
		}

		TEST_METHOD(TestXmlFileInvalidSectorWithoutName)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidSectorWithoutName); });
		}

		TEST_METHOD(TestXmlFileInvalidEntityWithoutName)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidEntityWithoutName); });
		}

		TEST_METHOD(TestXmlFileInvalidWSEWithData)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileInvalidWSEWithData); });
		}

		TEST_METHOD(TestXmlFileWSEInvalidPrimitive)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileWSEInvalidPrimitive); });
		}

		TEST_METHOD(TestXmlFileTestInvalidWorldWithoutName)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileTestInvalidWorldWithoutName); });
		}

		TEST_METHOD(TestXmlFileTestWSEInvalidGlmPrimitive)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] { xmlParseMaster.ParseFromFile(xmlFileTestWSEInvalidGlmPrimitive); });
		}

		TEST_METHOD(SharedDataWSERTTITest)
		{
			SharedDataWSE* sharedDataWSE = new SharedDataWSE();

			Assert::IsTrue(sharedDataWSE->Is("SharedDataWSE"));
			Assert::IsFalse(sharedDataWSE->Is("SharedDat"));

			RTTI* sharedDataWseRTTI = sharedDataWSE->QueryInterface(sharedDataWSE->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(sharedDataWseRTTI) == sharedDataWSE);
			Assert::IsTrue(sharedDataWSE->Equals(sharedDataWseRTTI));

			sharedDataWseRTTI = sharedDataWSE->QueryInterface(XmlParseMaster::SharedData::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(sharedDataWseRTTI) == sharedDataWSE);
			Assert::IsTrue(sharedDataWSE->Equals(sharedDataWseRTTI));

			sharedDataWseRTTI = sharedDataWSE->As<XmlParseMaster::SharedData>();
			Assert::IsTrue(sharedDataWSE->Equals(sharedDataWseRTTI));

			Assert::IsTrue(sharedDataWseRTTI->Is(sharedDataWSE->TypeIdClass()));
			Assert::IsTrue(sharedDataWseRTTI->Is(SharedDataWSE::TypeIdClass()));

			Assert::IsTrue(sharedDataWseRTTI->TypeIdInstance() == SharedDataWSE::TypeIdClass());
			Assert::IsTrue(sharedDataWseRTTI->TypeIdInstance() != XmlParseMaster::SharedData::TypeIdClass());

			delete sharedDataWSE;
		}

		TEST_METHOD(XmlParseHelperRTTITest)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			XmlParseHelperWSE* xmlParseHelperWSE = new XmlParseHelperWSE(xmlParseMaster);

			Assert::IsTrue(xmlParseHelperWSE->Is("XmlParseHelperWSE"));
			Assert::IsFalse(xmlParseHelperWSE->Is("XmlParseHelper"));

			RTTI* xmlParseHelperWseRTTI = xmlParseHelperWSE->QueryInterface(xmlParseHelperWSE->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperWseRTTI) == xmlParseHelperWSE);
			Assert::IsTrue(xmlParseHelperWSE->Equals(xmlParseHelperWseRTTI));

			xmlParseHelperWseRTTI = xmlParseHelperWSE->QueryInterface(IXmlParseHelper::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperWseRTTI) == xmlParseHelperWSE);
			Assert::IsTrue(xmlParseHelperWSE->Equals(xmlParseHelperWseRTTI));

			xmlParseHelperWseRTTI = xmlParseHelperWSE->As<IXmlParseHelper>();
			Assert::IsTrue(xmlParseHelperWSE->Equals(xmlParseHelperWseRTTI));

			Assert::IsTrue(xmlParseHelperWseRTTI->Is(xmlParseHelperWSE->TypeIdClass()));
			Assert::IsTrue(xmlParseHelperWseRTTI->Is(XmlParseHelperWSE::TypeIdClass()));

			Assert::IsTrue(xmlParseHelperWseRTTI->TypeIdInstance() == XmlParseHelperWSE::TypeIdClass());
			Assert::IsTrue(xmlParseHelperWseRTTI->TypeIdInstance() != IXmlParseHelper::TypeIdClass());

			delete xmlParseHelperWSE;
		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState XmlParseHelperWSETest::sStartMemState;
}