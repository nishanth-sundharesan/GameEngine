#include "Pch.h"
#include "CppUnitTest.h"
#include "ReactionAttributed.h"
#include "ActionEvent.h"
#include "EventMessage.h"
#include "MonsterEntity.h"
#include "AttackAction.h"
#include "HealAction.h"
#include "SharedDataWSE.h"
#include "XmlParseHelperWSE.h"
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;
using namespace std::chrono;

namespace LibraryDesktopTest
{
	TEST_CLASS(ReactionTest)
	{
	public:
		const string xmlFileReactionBasicWithNoDelay = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionBasicWithNoDelay.xml";

		const string xmlFileReactionNotMatchingSubType = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionNotMatchingSubType.xml";

		const string xmlFileReactionBasicWithDelay = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionBasicWithDelay.xml";

		const string xmlFileReactionInvalidInsideSector = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionInvalidInsideSector.xml";

		const string xmlFileReactionInvalidWithoutAttribute = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionInvalidWithoutAttribute.xml";

		const string xmlFileActionEventInvalidWithNoDelay = "..\\..\\..\\TestXmlFiles\\Reaction\\ActionEventInvalidWithNoDelay.xml";

		const string xmlFileReactionInvalidSubType = "..\\..\\..\\TestXmlFiles\\Reaction\\ReactionInvalidSubType.xml";

		const string xmlFileActionEventInvalidSubType = "..\\..\\..\\TestXmlFiles\\Reaction\\ActionEventInvalidSubType.xml";

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

		TEST_METHOD(ReactionBasicWithNoDelay)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileReactionBasicWithNoDelay);

			GameTime gameTime;
			WorldState worldState(gameTime);

			//World
			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			//Sector
			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			//Entity
			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 3U);

			//Reaction
			Reaction* reaction = static_cast<Reaction*>(actionDatum.Get<Scope*>(0));
			Datum& reactionActions = reaction->Actions();

			Assert::AreEqual(reaction->Size(), 4U);

			//AttackAction
			AttackAction* attackActionScope = static_cast<AttackAction*>(reactionActions.Get<Scope*>(0));
			Assert::IsFalse(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			//HealAction
			HealAction* healActionScope = static_cast<HealAction*>(reactionActions.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");

			world->Update(worldState);		
			world->Update(worldState);																			//Second update to actually fire the event.

			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(healActionScope->IsHealCalled());

			Assert::AreEqual(reaction->Size(), 6U);

			world->Update(worldState);			
		}

		TEST_METHOD(ReactionNotMatchingSubType)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileReactionNotMatchingSubType);

			GameTime gameTime;
			WorldState worldState(gameTime);

			//World
			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			//Sector
			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			//Entity
			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 3U);

			//Reaction
			Reaction* reaction = static_cast<Reaction*>(actionDatum.Get<Scope*>(0));
			Datum& reactionActions = reaction->Actions();

			Assert::AreEqual(reaction->Size(), 4U);

			//AttackAction
			AttackAction* attackActionScope = static_cast<AttackAction*>(reactionActions.Get<Scope*>(0));
			Assert::IsFalse(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			//HealAction
			HealAction* healActionScope = static_cast<HealAction*>(reactionActions.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");

			world->Update(worldState);

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(20));
			worldState.SetGameTime(gameTime);

			world->Update(worldState);																				//Second update to actually fire the event.

			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(healActionScope->IsHealCalled());

			Assert::AreEqual(reaction->Size(), 4U);

			world->Update(worldState);
		}

		TEST_METHOD(ReactionBasicTestWithDelay)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionCreateActionFactory createActionFactory;
			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			xmlParseMaster.ParseFromFile(xmlFileReactionBasicWithDelay);

			GameTime gameTime;
			WorldState worldState(gameTime);

			//World
			World* world = sharedDataWSE.GetWorld();
			Datum& sectorDatum = world->Sectors();

			//Sector
			Sector* sectorScope = static_cast<Sector*>(sectorDatum.Get<Scope*>(0));
			Datum& entityDatum = sectorScope->Entities();

			//Entity
			Entity* entityScope = static_cast<Entity*>(entityDatum.Get<Scope*>(0));
			Datum& actionDatum = entityScope->Actions();

			Assert::IsTrue(actionDatum.Size() == 3U);

			//Reaction
			Reaction* reaction = static_cast<Reaction*>(actionDatum.Get<Scope*>(0));
			Datum& reactionActions = reaction->Actions();

			Assert::AreEqual(reaction->Size(), 4U);

			//AttackAction
			AttackAction* attackActionScope = static_cast<AttackAction*>(reactionActions.Get<Scope*>(0));
			Assert::IsFalse(attackActionScope->IsAttackCalled());
			Assert::IsTrue(attackActionScope->Name() == "Attack");

			//HealAction
			HealAction* healActionScope = static_cast<HealAction*>(reactionActions.Get<Scope*>(1));
			Assert::IsFalse(healActionScope->IsHealCalled());
			Assert::IsTrue(healActionScope->Name() == "Heal");

			world->Update(worldState);

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(20));
			worldState.SetGameTime(gameTime);

			world->Update(worldState);																					//Second update to actually fire the event.
			
			Assert::IsTrue(attackActionScope->IsAttackCalled());
			Assert::IsTrue(healActionScope->IsHealCalled());

			Assert::AreEqual(reaction->Size(), 7U);

			world->Update(worldState);			
		}

		TEST_METHOD(ReactionRTTITest)
		{
			ReactionAttributed* reactionAttributed = new ReactionAttributed();
			Reaction* reaction = reactionAttributed;

			Assert::IsTrue(reaction->Is("Reaction"));

			RTTI* reactionRTTI = reaction->QueryInterface(reaction->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(reactionRTTI) == reaction);
			Assert::IsTrue(reaction->Equals(reactionRTTI));

			reactionRTTI = reaction->QueryInterface(ActionList::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(reactionRTTI) == reaction);
			Assert::IsTrue(reaction->Equals(reactionRTTI));

			reactionRTTI = reaction->As<Reaction>();
			Assert::IsTrue(reaction->Equals(reactionRTTI));

			Assert::IsTrue(reactionRTTI->Is(reaction->TypeIdClass()));
			Assert::IsTrue(reactionRTTI->Is(ReactionAttributed::TypeIdClass()));

			Assert::IsTrue(reaction->TypeIdInstance() == ReactionAttributed::TypeIdClass());

			Assert::IsTrue(reactionRTTI->TypeIdInstance() == ReactionAttributed::TypeIdClass());
			Assert::IsTrue(reactionRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());

			Assert::IsTrue(reactionRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());

			delete reaction;
		}

		TEST_METHOD(ReactionAttributedRTTITest)
		{
			ReactionAttributed* reactionAttributed = new ReactionAttributed();

			Assert::IsTrue(reactionAttributed->Is("ReactionAttributed"));

			RTTI* reactionRTTI = reactionAttributed->QueryInterface(reactionAttributed->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(reactionRTTI) == reactionAttributed);
			Assert::IsTrue(reactionAttributed->Equals(reactionRTTI));

			reactionRTTI = reactionAttributed->QueryInterface(ActionList::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(reactionRTTI) == reactionAttributed);
			Assert::IsTrue(reactionAttributed->Equals(reactionRTTI));

			reactionRTTI = reactionAttributed->As<ReactionAttributed>();
			Assert::IsTrue(reactionAttributed->Equals(reactionRTTI));

			Assert::IsTrue(reactionRTTI->Is(reactionAttributed->TypeIdClass()));
			Assert::IsTrue(reactionRTTI->Is(ReactionAttributed::TypeIdClass()));

			Assert::IsTrue(reactionRTTI->TypeIdInstance() == ReactionAttributed::TypeIdClass());
			Assert::IsTrue(reactionRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());

			delete reactionAttributed;
		}

		TEST_METHOD(ActionEventRTTI)
		{
			ActionEvent* actionEvent = new ActionEvent();

			Assert::IsTrue(actionEvent->Is("ActionEvent"));

			RTTI* actionEventRTTI = actionEvent->QueryInterface(actionEvent->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionEventRTTI) == actionEvent);
			Assert::IsTrue(actionEvent->Equals(actionEventRTTI));

			actionEventRTTI = actionEvent->QueryInterface(Attributed::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(actionEventRTTI) == actionEvent);
			Assert::IsTrue(actionEvent->Equals(actionEventRTTI));

			actionEventRTTI = actionEvent->As<ActionEvent>();
			Assert::IsTrue(actionEvent->Equals(actionEventRTTI));

			Assert::IsTrue(actionEventRTTI->Is(actionEvent->TypeIdClass()));
			Assert::IsTrue(actionEventRTTI->Is(ActionEvent::TypeIdClass()));

			Assert::IsTrue(actionEventRTTI->TypeIdInstance() == ActionEvent::TypeIdClass());
			Assert::IsTrue(actionEventRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());

			delete actionEvent;
		}

		TEST_METHOD(ReactionInvalidInsideSector)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileReactionInvalidInsideSector); });
		}

		TEST_METHOD(ReactionInvalidWithoutAttribute)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileReactionInvalidWithoutAttribute); });
		}

		TEST_METHOD(ActionEventInvalidWithNoDelay)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileActionEventInvalidWithNoDelay); });
		}

		TEST_METHOD(ReactionInvalidSubType)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileReactionInvalidSubType); });
		}

		TEST_METHOD(ActionEventInvalidSubType)
		{
			MonsterEntityFactory monsterFactory;
			AttackActionFactory attackFactory;
			HealActionFactory healFactory;
			ReactionAttributedFactory reactionFactory;
			ActionEventFactory actionEventFactory;

			ActionDeleteActionFactory deleteActionFactory;

			SharedDataWSE sharedDataWSE;
			XmlParseMaster xmlParseMaster(sharedDataWSE);
			sharedDataWSE.SetXmlParseMaster(xmlParseMaster);

			XmlParseHelperWSE wseXmlParseHelper(xmlParseMaster);
			Assert::ExpectException<exception>([&] {xmlParseMaster.ParseFromFile(xmlFileActionEventInvalidSubType); });
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState ReactionTest::sStartMemState;
}