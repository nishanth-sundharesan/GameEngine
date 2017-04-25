#include "Pch.h"
#include "CppUnitTest.h"
#include "World.h"
#include "Sector.h"
#include "MonsterEntity.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;

namespace LibraryDesktopTest
{
	TEST_CLASS(WorldSectorEntityTest)
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

		TEST_METHOD(TestWorld)
		{
			GameTime gameTime;
			WorldState worldState(gameTime);
			string firstWorldName = "PaulzWorld";

			World* world = new World(firstWorldName);
			Assert::IsTrue(world->Name() == firstWorldName);

			string secondWorldName = "FIEA";
			world->SetName(secondWorldName);
			Assert::IsTrue(world->Name() == secondWorldName);

			Datum& sectorDatum = world->Sectors();
			Assert::IsTrue(sectorDatum.Size() == 0U);
			Assert::IsTrue(sectorDatum.Type() == DatumType::TABLE);

			//Testing update with empty world
			world->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentWorld());

			//Testing Adopt Sector
			string firstSectorName = "PaulzSector";
			Sector* firstSector = new Sector(firstSectorName);
			world->AdoptSector(*firstSector);

			sectorDatum = world->Sectors();
			Assert::IsTrue(sectorDatum.Size() == 1U);
			Assert::IsTrue(sectorDatum.Type() == DatumType::TABLE);

			//Testing update with 1 sector;
			world->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentWorld());

			//Testing Create Sector One
			string secondSectorName = "FIEA";
			world->CreateSector(secondSectorName);

			//Testing update with 2 sectors;
			world->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentWorld());

			//Testing Create Sector Two
			string thirdSectorName = "Byea";
			world->CreateSector(thirdSectorName);

			//Testing update with 3 sectors;
			world->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentWorld());

			delete world;
		}

		TEST_METHOD(TestWorldRTTI)
		{
			string firstWorldName = "PaulzWorld";
			World* world = new World(firstWorldName);

			Assert::IsTrue(world->Is("World"));
			Assert::IsFalse(world->Is("Wor"));

			RTTI* worldRTTI = world->QueryInterface(world->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(worldRTTI) == world);
			Assert::IsTrue(world->Equals(worldRTTI));

			worldRTTI = world->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(worldRTTI) == world);
			Assert::IsTrue(world->Equals(worldRTTI));

			worldRTTI = world->As<Attributed>();
			Assert::IsTrue(world->Equals(worldRTTI));

			Assert::IsTrue(worldRTTI->Is(world->TypeIdClass()));
			Assert::IsTrue(worldRTTI->Is(World::TypeIdClass()));

			Assert::IsTrue(worldRTTI->TypeIdInstance() == World::TypeIdClass());
			Assert::IsTrue(worldRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete world;
		}

		TEST_METHOD(TestSector)
		{
			GameTime gameTime;
			WorldState worldState(gameTime);

			string firstWorldName = "PaulzWorldOne";
			World* firstWorld = new World(firstWorldName);

			string firstSectorName = "PaulzSectorOne";
			Sector* sector = new Sector(firstSectorName);
			firstWorld->AdoptSector(*sector);
			Assert::IsTrue(sector->Name() == firstSectorName);

			string secondSectorName = "PaulzSectorTwo";
			sector->SetName(secondSectorName);
			Assert::IsTrue(sector->Name() == secondSectorName);

			Datum& entityDatumOne = sector->Entities();
			Assert::IsTrue(entityDatumOne.Size() == 0U);
			Assert::IsTrue(entityDatumOne.Type() == DatumType::TABLE);

			//Testing update with 1 Sector, 0 Entities
			firstWorld->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentSector());

			Assert::IsTrue(firstWorld == &(sector->GetWorld()));

			//Testing with another World
			string secondWorldName = "PaulzWorldTwo";
			World* secondWorld = new World(secondWorldName);
			secondWorld->AdoptSector(*sector);

			Assert::IsTrue(secondWorld == &(sector->GetWorld()));

			//Adopting an Entity
			Entity* monsterEntity = new MonsterEntity();
			monsterEntity->SetName("MonsterOne");
			sector->AdoptEntity(*monsterEntity);

			//Testing update with 1 Sector, 1 Entity
			secondWorld->Update(worldState);
			Assert::IsTrue(nullptr == worldState.GetCurrentSector());
			Assert::IsTrue(nullptr == worldState.GetCurrentEntity());

			Datum& entityDatumTwo = sector->Entities();
			Assert::IsTrue(entityDatumTwo.Size() == 1U);
			Assert::IsTrue(entityDatumTwo.Type() == DatumType::TABLE);

			//Creating a new Entity
			MonsterEntityFactory monsterFactory;
			sector->CreateEntity("MonsterEntity", "MonsterTwo");

			Datum& entityDatumThree = sector->Entities();
			Assert::IsTrue(entityDatumThree.Size() == 2U);
			Assert::IsTrue(entityDatumThree.Type() == DatumType::TABLE);

			delete secondWorld;
			delete firstWorld;
		}

		TEST_METHOD(TestSectorRTTI)
		{
			string firstSectorName = "PaulzSector";
			Sector* sector = new Sector(firstSectorName);

			Assert::IsTrue(sector->Is("Sector"));
			Assert::IsFalse(sector->Is("Sec"));

			RTTI* sectorRTTI = sector->QueryInterface(sector->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(sectorRTTI) == sector);
			Assert::IsTrue(sector->Equals(sectorRTTI));

			sectorRTTI = sector->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(sectorRTTI) == sector);
			Assert::IsTrue(sector->Equals(sectorRTTI));

			sectorRTTI = sector->As<Attributed>();
			Assert::IsTrue(sector->Equals(sectorRTTI));

			Assert::IsTrue(sectorRTTI->Is(sector->TypeIdClass()));
			Assert::IsTrue(sectorRTTI->Is(Sector::TypeIdClass()));

			Assert::IsTrue(sectorRTTI->TypeIdInstance() == Sector::TypeIdClass());
			Assert::IsTrue(sectorRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete sector;
		}

		TEST_METHOD(TestEntity)
		{
			// TODO
			//GameTime gameTime;
			//WorldState worldState(gameTime);
			//string firstWorldName = "PaulzWorldOne";
			//string firstSectorName = "PaulzSectorOne";
			//string secondSectorName = "PaulzSectorTwo";
			//string firstEntityName = "PaulzEntityOne";
			//string entityRenamed = "StupidEntity";

			//World* world = new World(firstWorldName);
			//Sector& firstSector = world->CreateSector("firstWorldName");
			//MonsterEntityFactory monsterFactory;
			//Entity& entity = firstSector.CreateEntity("MonsterEntity", firstEntityName);

			//Assert::IsTrue(entity.Name() == firstEntityName);
			//entity.SetName(entityRenamed);
			//Assert::IsTrue(entity.Name() == entityRenamed);

			////Checking for sector
			//Assert::IsTrue(&firstSector == &entity.GetSector());

			////Changing the sector for Entity
			//Sector& secondSector = world->CreateSector(secondSectorName);
			//secondSector.AdoptEntity(entity);

			////Testing for Datum
			//Datum& firstSectorDatum = firstSector.Entities();
			//Assert::AreEqual(0U, firstSectorDatum.Size());

			//Datum& secondSectorDatum = secondSector.Entities();
			//Assert::AreEqual(1U, secondSectorDatum.Size());

			////Checking for sector
			//Assert::IsTrue(&secondSector == &entity.GetSector());

			//world->Update(worldState);
			//Assert::IsTrue(nullptr == worldState.GetCurrentEntity());

			////Checking for IsPrescribed()
			//Assert::IsTrue(static_cast<MonsterEntity&>(entity).IsPrescribedAttribute("EntityName"));
			//Assert::IsTrue(entity.IsPrescribedAttribute("EntityName"));
			//delete world;
		}

		TEST_METHOD(TestEntityRTTI)
		{
			string firstEntityName = "MonsterEntity";
			Entity* entity = new Entity();

			Assert::IsTrue(entity->Is("Entity"));
			Assert::IsFalse(entity->Is("Ent"));

			RTTI* entityRTTI = entity->QueryInterface(entity->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(entityRTTI) == entity);
			Assert::IsTrue(entity->Equals(entityRTTI));

			entityRTTI = entity->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(entityRTTI) == entity);
			Assert::IsTrue(entity->Equals(entityRTTI));

			entityRTTI = entity->As<Attributed>();
			Assert::IsTrue(entity->Equals(entityRTTI));

			Assert::IsTrue(entityRTTI->Is(entity->TypeIdClass()));
			Assert::IsTrue(entityRTTI->Is(Entity::TypeIdClass()));

			Assert::IsTrue(entityRTTI->TypeIdInstance() == Entity::TypeIdClass());
			Assert::IsTrue(entityRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete entity;
		}

		bool CompareGameTime(GameTime& lhsGameTime, GameTime& rhsGameTime)
		{
			return (lhsGameTime.CurrentTime() == rhsGameTime.CurrentTime() && lhsGameTime.TotalGameTime() == rhsGameTime.TotalGameTime() && lhsGameTime.ElapsedGameTime() == rhsGameTime.ElapsedGameTime());
		}

		TEST_METHOD(TestWorldState)
		{
			GameTime firstGameTime;
			WorldState worldState(firstGameTime);

			Assert::IsTrue(CompareGameTime(firstGameTime, worldState.GetGameTime()));

			GameTime secondGameTime;
			worldState.SetGameTime(secondGameTime);
			Assert::IsTrue(CompareGameTime(secondGameTime, worldState.GetGameTime()));
		}
	private:
		static _CrtMemState sStartMemState;
	};

	_CrtMemState WorldSectorEntityTest::sStartMemState;
}
