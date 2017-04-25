#include "Pch.h"
#include "CppUnitTest.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "AsyncEventSubscriber.h"
#include "Foo.h"
#include "Event.h"
#include <chrono>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;
using namespace chrono;

namespace LibraryDesktopTest
{
	TEST_CLASS(AsyncEventTest)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{
			Event<Foo>::ReserveSubscribers(10000);

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

		TEST_METHOD(EventQueueNoDelayDynamicAllocation100Events)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Event<Foo>::Subscribe(subscriberFoo);

			for (uint32_t i = 0; i < 100; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventQueueNoDelayDynamicAllocation10Events100Subscribers)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 10;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<SubscriberFoo*> listSubscriberFoo;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				SubscriberFoo* subscriberFoo = new SubscriberFoo();
				listSubscriberFoo.push_back(subscriberFoo);
				Event<Foo>::Subscribe(*listSubscriberFoo[i]);
			}

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsFalse(listSubscriberFoo[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsTrue(listSubscriberFoo[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberFoo.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				delete listSubscriberFoo[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventQueueWithDelayDynamicAllocation10Events100Subscribers)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 10;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime, 500U);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<SubscriberFoo*> listSubscriberFoo;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				SubscriberFoo* subscriberFoo = new SubscriberFoo();
				listSubscriberFoo.push_back(subscriberFoo);
				Event<Foo>::Subscribe(*listSubscriberFoo[i]);
			}

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsFalse(listSubscriberFoo[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsFalse(listSubscriberFoo[i]->WasNotified());
			}

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(499));
			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsFalse(listSubscriberFoo[i]->WasNotified());
			}

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(1));
			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsTrue(listSubscriberFoo[i]->WasNotified());
			}

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(10));
			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				Assert::IsTrue(listSubscriberFoo[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberFoo.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberFoo.size(); ++i)
			{
				delete listSubscriberFoo[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersSubscribingBack)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 1;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 1000;
			vector<AsyncEventSubscriber*> listSubscriberSubscribing;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberSubscribing = new AsyncEventSubscriber(AsyncAction::SUBSCRIBE, eventQueue, gameTime);
				listSubscriberSubscribing.push_back(subscriberSubscribing);
				Event<Foo>::Subscribe(*listSubscriberSubscribing[i]);
			}

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				Assert::IsFalse(listSubscriberSubscribing[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				Assert::IsTrue(listSubscriberSubscribing[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberSubscribing.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				delete listSubscriberSubscribing[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersUnSubscribingItself)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 1;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 1000;
			vector<AsyncEventSubscriber*> listSubscriberUnsubscribing;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberUnsubscribing = new AsyncEventSubscriber(AsyncAction::UNSUBSCRIBE, eventQueue, gameTime);
				listSubscriberUnsubscribing.push_back(subscriberUnsubscribing);
				Event<Foo>::Subscribe(*listSubscriberUnsubscribing[i]);
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				Assert::IsFalse(listSubscriberUnsubscribing[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				Assert::IsTrue(listSubscriberUnsubscribing[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberUnsubscribing.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				delete listSubscriberUnsubscribing[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersUnSubscribeAll)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 1;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 1000;
			vector<AsyncEventSubscriber*> listSubscriberUnsubscribeAll;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberUnsubscribeAll = new AsyncEventSubscriber(AsyncAction::CLEARSUBSCRIBERS, eventQueue, gameTime);
				listSubscriberUnsubscribeAll.push_back(subscriberUnsubscribeAll);
				Event<Foo>::Subscribe(*listSubscriberUnsubscribeAll[i]);
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				Assert::IsFalse(listSubscriberUnsubscribeAll[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				Assert::IsTrue(listSubscriberUnsubscribeAll[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberUnsubscribeAll.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				delete listSubscriberUnsubscribeAll[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersSubscribingUnsubscribing)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 10;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<AsyncEventSubscriber*> listSubscriberSubscribing;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberSubscribing = new AsyncEventSubscriber(AsyncAction::SUBSCRIBE, eventQueue, gameTime);
				listSubscriberSubscribing.push_back(subscriberSubscribing);
				Event<Foo>::Subscribe(*listSubscriberSubscribing[i]);
			}

			vector<AsyncEventSubscriber*> listSubscriberUnsubscribing;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberUnsubscribing = new AsyncEventSubscriber(AsyncAction::UNSUBSCRIBE, eventQueue, gameTime);
				listSubscriberUnsubscribing.push_back(subscriberUnsubscribing);
				Event<Foo>::Subscribe(*listSubscriberUnsubscribing[i]);
			}

			vector<AsyncEventSubscriber*> listSubscriberUnsubscribeAll;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* subscriberUnsubscribeAll = new AsyncEventSubscriber(AsyncAction::CLEARSUBSCRIBERS, eventQueue, gameTime);
				listSubscriberUnsubscribeAll.push_back(subscriberUnsubscribeAll);
				Event<Foo>::Subscribe(*listSubscriberUnsubscribeAll[i]);
			}

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				Assert::IsFalse(listSubscriberSubscribing[i]->WasNotified());
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				Assert::IsFalse(listSubscriberUnsubscribing[i]->WasNotified());
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				Assert::IsFalse(listSubscriberUnsubscribeAll[i]->WasNotified());
			}

			eventQueue.Update(gameTime);

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				Assert::IsTrue(listSubscriberSubscribing[i]->WasNotified());
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				Assert::IsTrue(listSubscriberUnsubscribing[i]->WasNotified());
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				Assert::IsTrue(listSubscriberUnsubscribeAll[i]->WasNotified());
			}

			Assert::IsTrue(listSubscriberSubscribing.size() == numberOfEventSubscribers);
			Assert::IsTrue(listSubscriberUnsubscribing.size() == numberOfEventSubscribers);
			Assert::IsTrue(listSubscriberUnsubscribeAll.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listSubscriberSubscribing.size(); ++i)
			{
				delete listSubscriberSubscribing[i];
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribing.size(); ++i)
			{
				delete listSubscriberUnsubscribing[i];
			}

			for (uint32_t i = 0; i < listSubscriberUnsubscribeAll.size(); ++i)
			{
				delete listSubscriberUnsubscribeAll[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersEnqueing)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 1;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<AsyncEventSubscriber*> listAsyncEventSubscribers;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* asyncEventSubscriber = new AsyncEventSubscriber(AsyncAction::ENQUEUE, eventQueue, gameTime);
				listAsyncEventSubscribers.push_back(asyncEventSubscriber);
				Event<Foo>::Subscribe(*listAsyncEventSubscribers[i]);
			}

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsFalse(listAsyncEventSubscribers[i]->WasNotified());
				Assert::IsTrue(listAsyncEventSubscribers[i]->NumberOfTimesCalled() == 0);
			}

			Assert::IsTrue(eventQueue.Size() == numberOfEvents);

			eventQueue.Update(gameTime);

			Assert::IsTrue(eventQueue.Size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsTrue(listAsyncEventSubscribers[i]->WasNotified());
				Assert::IsTrue(listAsyncEventSubscribers[i]->NumberOfTimesCalled() == 1);
			}

			eventQueue.Update(gameTime);

			Assert::IsTrue(eventQueue.Size() == numberOfEventSubscribers * numberOfEventSubscribers);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsTrue(listAsyncEventSubscribers[i]->WasNotified());
				//We don't check for the number of times a subscriber is called here because many events will try to access the same subscriber and hence data race.
			}

			Assert::IsTrue(listAsyncEventSubscribers.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				delete listAsyncEventSubscribers[i];
			}

			Event<Foo>::UnSubscribeAll();

			eventQueue.Update(gameTime);

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersClearingEventQueue)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 10;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<AsyncEventSubscriber*> listAsyncEventSubscribers;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* asyncEventSubscriber = new AsyncEventSubscriber(AsyncAction::QUEUECLEAR, eventQueue, gameTime);
				listAsyncEventSubscribers.push_back(asyncEventSubscriber);
				Event<Foo>::Subscribe(*listAsyncEventSubscribers[i]);
			}

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsFalse(listAsyncEventSubscribers[i]->WasNotified());
				Assert::IsTrue(listAsyncEventSubscribers[i]->NumberOfTimesCalled() == 0);
			}

			Assert::IsTrue(eventQueue.Size() == numberOfEvents);

			eventQueue.Update(gameTime);

			Assert::IsTrue(eventQueue.Size() == 0);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsTrue(listAsyncEventSubscribers[i]->WasNotified());				
			}

			Assert::IsTrue(listAsyncEventSubscribers.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				delete listAsyncEventSubscribers[i];
			}

			Event<Foo>::UnSubscribeAll();
		}

		TEST_METHOD(EventSubscribersThrowingException)
		{
			Foo foo;
			GameTime gameTime;
			EventQueue eventQueue;

			uint32_t numberOfEvents = 1;
			for (uint32_t i = 0; i < numberOfEvents; ++i)
			{
				Event<Foo>* event = new Event<Foo>(foo, true);
				eventQueue.Enqueue(*event, gameTime);
			}

			uint32_t numberOfEventSubscribers = 100;
			vector<AsyncEventSubscriber*> listAsyncEventSubscribers;
			for (uint32_t i = 0; i < numberOfEventSubscribers; ++i)
			{
				AsyncEventSubscriber* asyncEventSubscriber = new AsyncEventSubscriber(AsyncAction::THROWEXCEPTION, eventQueue, gameTime);
				listAsyncEventSubscribers.push_back(asyncEventSubscriber);
				Event<Foo>::Subscribe(*listAsyncEventSubscribers[i]);
			}

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsFalse(listAsyncEventSubscribers[i]->WasNotified());
				Assert::IsTrue(listAsyncEventSubscribers[i]->NumberOfTimesCalled() == 0);
			}

			Assert::IsTrue(eventQueue.Size() == numberOfEvents);

			Assert::ExpectException<vector<std::exception_ptr>>([&] {eventQueue.Update(gameTime); });
			//eventQueue.Update(gameTime);

			Assert::IsTrue(eventQueue.Size() == 0);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				Assert::IsTrue(listAsyncEventSubscribers[i]->WasNotified());
			}

			Assert::IsTrue(listAsyncEventSubscribers.size() == numberOfEventSubscribers);

			for (uint32_t i = 0; i < listAsyncEventSubscribers.size(); ++i)
			{
				delete listAsyncEventSubscribers[i];
			}

			Event<Foo>::UnSubscribeAll();
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState AsyncEventTest::sStartMemState;
}