#include "Pch.h"
#include "CppUnitTest.h"
#include "ActionList.h"
#include "AttackAction.h"
#include "HealAction.h"
#include "MonsterEntity.h"
#include "XmlParseHelperWSE.h"
#include "XmlParseHelperIf.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace std;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;

namespace LibraryDesktopTest
{
	TEST_CLASS(ActionTest)
	{
	public:
		const string actionXmlFolder = "..\\..\\..\\TestXmlFiles\\Action\\";

		const string invalidActionXmlFolder = "..\\..\\..\\TestXmlFiles\\Action\\InvalidXml\\";

		const string ifActionXmlFolder = "..\\..\\..\\TestXmlFiles\\IfAction\\";

		//Actions
		const string xmlFileEmptyAction = actionXmlFolder + "TestEmptyAction.xml";

		const string xmlFileActionArrays = actionXmlFolder + "TestActionArrays.xml";

		//ActionList
		const string xmlFileActionList = actionXmlFolder + "TestActionList.xml";

		const string xmlFileActionListArrays = actionXmlFolder + "TestActionListArrays.xml";

		//ActionCreateAction
		const string xmlFileActionCreateAction = actionXmlFolder + "TestActionCreateAction.xml";

		const string xmlFileTestActionCreateActionInActionList = actionXmlFolder + "TestActionCreateActionInActionList.xml";

		//ActionDeleteAction
		const string xmlFileActionDeleteAction = actionXmlFolder + "TestActionDeleteAction.xml";

		const string xmlFileActionDeleteActionInActionList = actionXmlFolder + "TestActionDeleteActionInActionList.xml";

		//If Blocks
		const string xmlFileActionIfThenExecute = ifActionXmlFolder + "TestActionIfThenExecute.xml";

		const string xmlFileActionIfElseExecute = ifActionXmlFolder + "TestActionIfElseExecute.xml";

		const string xmlFileActionIfAuxiliaryInside = ifActionXmlFolder + "TestActionIfAuxiliaryInside.xml";

		const string xmlFileActionIfWithNoElse = ifActionXmlFolder + "TestActionIfWithNoElse.xml";

		//Invalid If Blocks
		const string xmlFileTestActionInvalidIfOne = ifActionXmlFolder + "TestActionInvalidIfOne.xml";

		const string xmlFileTestActionInvalidIfTwo = ifActionXmlFolder + "TestActionInvalidIfTwo.xml";

		const string xmlFileTestInvalidThenAction = ifActionXmlFolder + "TestInvalidThenAction.xml";

		const string xmlFileTestInvalidElseAction = ifActionXmlFolder + "TestInvalidElseAction.xml";

		const string xmlFileTestActionInvalidIfThree = ifActionXmlFolder + "TestActionInvalidIfThree.xml";

		const string xmlFileTestActionIfElseExecuteInsideActionList = ifActionXmlFolder + "TestActionIfElseExecuteInsideActionList.xml";

		const string xmlFileTestInvalidElseOutsideIf = ifActionXmlFolder + "TestInvalidElseOutsideIf.xml";

		const string xmlFileTestInvalidIfWithNoAttributesOne = ifActionXmlFolder + "TestInvalidIfWithNoAttributesOne.xml";

		const string xmlFileTestInvalidIfWithNoAttributesTwo = ifActionXmlFolder + "TestInvalidIfWithNoAttributesTwo.xml";

		const string xmlFileTestInvalidThenOutSideIf = ifActionXmlFolder + "TestInvalidThenOutSideIf.xml";

		const string xmlFileTestInvalidIfThenExecuteWithData = ifActionXmlFolder + "TestInvalidIfThenExecuteWithData.xml";

		const string xmlFileTestInvalidIfPath = ifActionXmlFolder + "TestInvalidIfPath.xml";

		//Invalid Actions
		const string xmlFileTestInvalidAction = invalidActionXmlFolder + "TestInvalidAction.xml";

		const string xmlFileTestInvalidActionTwo = invalidActionXmlFolder + "TestInvalidActionTwo.xml";

		const string xmlFileTestInvalidActionList = invalidActionXmlFolder + "TestInvalidActionList.xml";

		const string xmlFileTestInvalidActionNoAttributes = invalidActionXmlFolder + "TestInvalidActionNoAttributes.xml";

		const string xmlFileTestInvalidActionListNoAttributes = invalidActionXmlFolder + "TestInvalidActionListNoAttributes.xml";

		//Invalid ActionCreateAction
		const string xmlFileTestInvalidActionCreateActionOne = invalidActionXmlFolder + "TestInvalidActionCreateActionOne.xml";

		const string xmlFileTestInvalidActionCreateActionTwo = invalidActionXmlFolder + "TestInvalidActionCreateActionTwo.xml";

		const string xmlFileTestInvalidCreateActionNoAttributes = invalidActionXmlFolder + "TestInvalidCreateActionNoAttributes.xml";

		//Invalid ActionDeleteAction
		const string xmlFileTestInvalidActionDeleteActionOne = invalidActionXmlFolder + "TestInvalidActionDeleteActionOne.xml";

		const string xmlFileTestInvalidActionDeleteActionTwo = invalidActionXmlFolder + "TestInvalidActionDeleteActionTwo.xml";

		const string xmlFileTestInvalidDeleteActionNoAttributes = invalidActionXmlFolder + "TestInvalidDeleteActionNoAttributes.xml";

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

		TEST_METHOD(ActionEmptyActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileEmptyAction);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Assert::IsTrue(&entityScope->GetSector() == sectorScope);
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 1U);

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatum.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");
		}

		TEST_METHOD(ActionArraysTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionArrays);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 2U);

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatum.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatum.Get<Scope*>(1));
			Assert::IsTrue(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");
		}

		TEST_METHOD(ActionListTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionList);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListDatum = entityScope->Actions();

			Assert::IsTrue(actionListDatum.Size() == 1U);

			ActionList* actionList = static_cast<ActionList*>(actionListDatum.Get<Scope*>(0));
			Datum& actionDatum = actionList->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatum.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatum.Get<Scope*>(1));
			Assert::IsTrue(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");
		}

		TEST_METHOD(ActionListArraysTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionListArrays);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListDatum = entityScope->Actions();

			Assert::IsTrue(actionListDatum.Size() == 2U);

			//First ActionList
			ActionList* actionListOne = static_cast<ActionList*>(actionListDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			Assert::IsTrue(actionDatumOne.Size() == 2U);

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatumOne.Get<Scope*>(1));
			Assert::IsTrue(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");

			//Second ActionList
			ActionList* actionListTwo = static_cast<ActionList*>(actionListDatum.Get<Scope*>(1));
			Datum& actionDatumTwo = actionListTwo->Actions();
			Assert::IsTrue(actionDatumTwo.Size() == 1U);

			HealAction* healActionScopeTwo = static_cast<HealAction*>(actionDatumTwo.Get<Scope*>(0));
			Assert::IsTrue(healActionScopeTwo->IsHealCalled());
			Assert::IsTrue(healActionScopeTwo->Name() == "Heal");
		}

		TEST_METHOD(ActionIfThenExecuteTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileActionIfThenExecute);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListIfDatum = entityScope->Actions();

			//First ActionList
			ActionListIf* actionListOne = static_cast<ActionListIf*>(actionListIfDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatumOne.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");
		}

		TEST_METHOD(ActionIfCloneTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			//Cloning XmlParseHelperIf
			XmlParseHelperIf* clonedIfXmlParseHelper = static_cast<XmlParseHelperIf*>(ifXmlParseHelper.Clone());
			xmlParseMaster.RemoveHelper(ifXmlParseHelper);			
			clonedIfXmlParseHelper;

			xmlParseMaster.ParseFromFile(xmlFileActionIfThenExecute);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListIfDatum = entityScope->Actions();

			//First ActionList
			ActionListIf* actionListOne = static_cast<ActionListIf*>(actionListIfDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatumOne.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");

			delete clonedIfXmlParseHelper;
		}

		TEST_METHOD(ActionIfElseExecute)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileActionIfElseExecute);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListIfDatum = entityScope->Actions();

			//First ActionList
			ActionListIf* actionListOne = static_cast<ActionListIf*>(actionListIfDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsFalse(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatumOne.Get<Scope*>(1));
			Assert::IsTrue(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");
		}

		TEST_METHOD(ActionIfAuxiliaryInside)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			xmlParseMaster.ParseFromFile(xmlFileActionIfAuxiliaryInside);

			GameTime gameTime;
			gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListIfDatum = entityScope->Actions();

			ActionListIf* actionListOne = static_cast<ActionListIf*>(actionListIfDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			HealAction* healActionScope = static_cast<HealAction*>(actionDatumOne.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");
		}

		TEST_METHOD(ActionIfWithNoElse)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			/*xmlParseMaster.ParseFromFile(xmlFileActionIfWithNoElse);

			GameTime gameTime;
			gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			world->Update(worldState);
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListIfDatum = entityScope->Actions();

			ActionListIf* actionListOne = static_cast<ActionListIf*>(actionListIfDatum.Get<Scope*>(0));
			Datum& actionDatumOne = actionListOne->Actions();

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatumOne.Get<Scope*>(0));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");*/
		}

		TEST_METHOD(ActionCreateActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			ActionCreateActionFactory createActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionCreateAction);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 1U);
			world->Update(worldState);
			Assert::IsTrue(actionDatum.Size() == 2U);

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatum.Get<Scope*>(1));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");
		}

		TEST_METHOD(ActionCreateActionInActionListTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			ActionCreateActionFactory createActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileTestActionCreateActionInActionList);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListDatum = entityScope->Actions();

			Assert::IsTrue(actionListDatum.Size() == 1U);

			ActionList* actionList = static_cast<ActionList*>(actionListDatum.Get<Scope*>(0));
			Datum& actionDatum = actionList->Actions();

			Assert::IsTrue(actionDatum.Size() == 1U);
			world->Update(worldState);
			Assert::IsTrue(actionDatum.Size() == 2U);
		}

		TEST_METHOD(ActionDeleteActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionDeleteAction);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 2U);
			world->Update(worldState);
			Assert::IsTrue(actionDatum.Size() == 2U);

			AttackAction* attackActionScope = static_cast<AttackAction*>(actionDatum.Get<Scope*>(1));
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");
		}

		TEST_METHOD(ActionDeleteActionInActionListTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileActionDeleteActionInActionList);

			GameTime gameTime;
			WorldState worldState(gameTime);

			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionListDatum = entityScope->Actions();

			Assert::IsTrue(actionListDatum.Size() == 1U);

			ActionList* actionList = static_cast<ActionList*>(actionListDatum.Get<Scope*>(0));
			Datum& actionDatum = actionList->Actions();

			Assert::IsTrue(actionDatum.Size() == 2U);
			world->Update(worldState);
			Assert::IsTrue(actionDatum.Size() == 2U);
		}

		TEST_METHOD(ActionListRTTI)
		{			
			ActionList* actionList = new ActionList();

			Assert::IsTrue(actionList->Is("ActionList"));
			Assert::IsFalse(actionList->Is("ActionLi"));

			RTTI* actionListRTTI = actionList->QueryInterface(actionList->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionListRTTI) == actionList);
			Assert::IsTrue(actionList->Equals(actionListRTTI));

			actionListRTTI = actionList->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionListRTTI) == actionList);
			Assert::IsTrue(actionList->Equals(actionListRTTI));

			actionListRTTI = actionList->As<Attributed>();
			Assert::IsTrue(actionList->Equals(actionListRTTI));

			Assert::IsTrue(actionListRTTI->Is(actionList->TypeIdClass()));
			Assert::IsTrue(actionListRTTI->Is(ActionList::TypeIdClass()));

			Assert::IsTrue(actionListRTTI->TypeIdInstance() == ActionList::TypeIdClass());
			Assert::IsTrue(actionListRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete actionList;
		}

		TEST_METHOD(ActionListIfRTTI)
		{
			ActionListIf* actionListIf = new ActionListIf();

			Assert::IsTrue(actionListIf->Is("ActionListIf"));
			Assert::IsFalse(actionListIf->Is("ActionLi"));

			RTTI* actionListIfRTTI = actionListIf->QueryInterface(actionListIf->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionListIfRTTI) == actionListIf);
			Assert::IsTrue(actionListIf->Equals(actionListIfRTTI));

			actionListIfRTTI = actionListIf->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionListIfRTTI) == actionListIf);
			Assert::IsTrue(actionListIf->Equals(actionListIfRTTI));

			actionListIfRTTI = actionListIf->As<Attributed>();
			Assert::IsTrue(actionListIf->Equals(actionListIfRTTI));

			Assert::IsTrue(actionListIfRTTI->Is(actionListIf->TypeIdClass()));
			Assert::IsTrue(actionListIfRTTI->Is(ActionListIf::TypeIdClass()));

			Assert::IsTrue(actionListIfRTTI->TypeIdInstance() == ActionListIf::TypeIdClass());
			Assert::IsTrue(actionListIfRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete actionListIf;
		}

		TEST_METHOD(ActionCreateActionRTTI)
		{
			ActionCreateAction* actionCreateAction = new ActionCreateAction();

			Assert::IsTrue(actionCreateAction->Is("ActionCreateAction"));
			Assert::IsFalse(actionCreateAction->Is("ActionCreateActi"));

			RTTI* actionCreateActionRTTI = actionCreateAction->QueryInterface(actionCreateAction->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionCreateActionRTTI) == actionCreateAction);
			Assert::IsTrue(actionCreateAction->Equals(actionCreateActionRTTI));

			actionCreateActionRTTI = actionCreateAction->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionCreateActionRTTI) == actionCreateAction);
			Assert::IsTrue(actionCreateAction->Equals(actionCreateActionRTTI));

			actionCreateActionRTTI = actionCreateAction->As<Attributed>();
			Assert::IsTrue(actionCreateAction->Equals(actionCreateActionRTTI));

			Assert::IsTrue(actionCreateActionRTTI->Is(actionCreateAction->TypeIdClass()));
			Assert::IsTrue(actionCreateActionRTTI->Is(ActionCreateAction::TypeIdClass()));

			Assert::IsTrue(actionCreateActionRTTI->TypeIdInstance() == ActionCreateAction::TypeIdClass());
			Assert::IsTrue(actionCreateActionRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete actionCreateAction;
		}

		TEST_METHOD(ActionCreateDeleteActionRTTI)
		{
			ActionDeleteAction* actionDeleteAction = new ActionDeleteAction();

			Assert::IsTrue(actionDeleteAction->Is("ActionDeleteAction"));
			Assert::IsFalse(actionDeleteAction->Is("ActionCreateActi"));

			RTTI* actionDeleteActionRTTI = actionDeleteAction->QueryInterface(actionDeleteAction->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionDeleteActionRTTI) == actionDeleteAction);
			Assert::IsTrue(actionDeleteAction->Equals(actionDeleteActionRTTI));

			actionDeleteActionRTTI = actionDeleteAction->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionDeleteActionRTTI) == actionDeleteAction);
			Assert::IsTrue(actionDeleteAction->Equals(actionDeleteActionRTTI));

			actionDeleteActionRTTI = actionDeleteAction->As<Attributed>();
			Assert::IsTrue(actionDeleteAction->Equals(actionDeleteActionRTTI));

			Assert::IsTrue(actionDeleteActionRTTI->Is(actionDeleteAction->TypeIdClass()));
			Assert::IsTrue(actionDeleteActionRTTI->Is(ActionDeleteAction::TypeIdClass()));

			Assert::IsTrue(actionDeleteActionRTTI->TypeIdInstance() == ActionDeleteAction::TypeIdClass());
			Assert::IsTrue(actionDeleteActionRTTI->TypeIdInstance() != Attributed::TypeIdClass());

			delete actionDeleteAction;
		}

		TEST_METHOD(XmlParseHelperIfRTTITest)
		{
			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			XmlParseHelperIf* xmlParseHelperIf = new XmlParseHelperIf(xmlParseMaster);

			Assert::IsTrue(xmlParseHelperIf->Is("XmlParseHelperIf"));
			Assert::IsFalse(xmlParseHelperIf->Is("XmlParseHelp"));

			RTTI* xmlParseHelperIfRTTI = xmlParseHelperIf->QueryInterface(xmlParseHelperIf->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperIfRTTI) == xmlParseHelperIf);
			Assert::IsTrue(xmlParseHelperIf->Equals(xmlParseHelperIfRTTI));

			xmlParseHelperIfRTTI = xmlParseHelperIf->QueryInterface(IXmlParseHelper::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(xmlParseHelperIfRTTI) == xmlParseHelperIf);
			Assert::IsTrue(xmlParseHelperIf->Equals(xmlParseHelperIfRTTI));

			xmlParseHelperIfRTTI = xmlParseHelperIf->As<IXmlParseHelper>();
			Assert::IsTrue(xmlParseHelperIf->Equals(xmlParseHelperIfRTTI));

			Assert::IsTrue(xmlParseHelperIfRTTI->Is(xmlParseHelperIf->TypeIdClass()));
			Assert::IsTrue(xmlParseHelperIfRTTI->Is(XmlParseHelperIf::TypeIdClass()));

			Assert::IsTrue(xmlParseHelperIfRTTI->TypeIdInstance() == XmlParseHelperIf::TypeIdClass());
			Assert::IsTrue(xmlParseHelperIfRTTI->TypeIdInstance() != IXmlParseHelper::TypeIdClass());

			delete xmlParseHelperIf;
		}

#pragma region Invalid Action Tests
		TEST_METHOD(ActionInvalidActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidAction); });
		}

		TEST_METHOD(ActionTestInvalidActionTwoTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionTwo); });
		}

		TEST_METHOD(ActionInvalidActionListTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionList); });
		}

		TEST_METHOD(ActionInvalidActionCreateActionOneTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionCreateActionOne); });
		}

		TEST_METHOD(ActionInvalidActionCreateActionTwoTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionCreateActionTwo); });
		}

		TEST_METHOD(ActionTestInvalidActionDeleteActionOneTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionDeleteActionOne); });
		}

		TEST_METHOD(ActionTestInvalidActionDeleteActionTwoTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionDeleteActionTwo); });
		}

		TEST_METHOD(ActionTestInvalidCreateActionNoAttributesTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidCreateActionNoAttributes); });
		}

		TEST_METHOD(ActionTestInvalidDeleteActionNoAttributesTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidDeleteActionNoAttributes); });
		}

		TEST_METHOD(ActionTestInvalidActionNoAttributesTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionNoAttributes); });
		}

		TEST_METHOD(ActionTestInvalidActionListNoAttributesTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidActionListNoAttributes); });
		}

#pragma endregion

#pragma region Invalid If Action Tests
		TEST_METHOD(ActionTestActionInvalidIfOneTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestActionInvalidIfOne); });
		}

		TEST_METHOD(ActionTestActionInvalidIfTwoTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestActionInvalidIfTwo); });
		}

		TEST_METHOD(ActionTestActionInvalidIfThreeTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestActionInvalidIfThree); });
		}

		TEST_METHOD(ActionTestInvalidElseOutsideIfTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidElseOutsideIf); });
		}

		TEST_METHOD(ActionTestInvalidIfWithNoAttributesOneTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidIfWithNoAttributesOne); });
		}

		TEST_METHOD(ActionTestInvalidIfWithNoAttributesTwoTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidIfWithNoAttributesTwo); });
		}

		TEST_METHOD(ActionTestInvalidThenOutSideIfTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidThenOutSideIf); });
		}

		TEST_METHOD(ActionTestInvalidIfThenExecuteWithDataTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidIfThenExecuteWithData); });
		}

		TEST_METHOD(ActionTestInvalidThenActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidThenAction); });
		}

		TEST_METHOD(ActionTestInvalidElseActionTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidElseAction); });
		}

		TEST_METHOD(ActionTestActionIfElseExecuteInsideActionListTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestActionIfElseExecuteInsideActionList); });
		}

		TEST_METHOD(ActionTestInvalidIfPathTest)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ActionListIfFactory actionListIfFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			XmlParseHelperIf ifXmlParseHelper(xmlParseMaster);

			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileTestInvalidIfPath); });
		}
#pragma endregion
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState ActionTest::sStartMemState;
}