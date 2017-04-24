#include "Pch.h"
#include "CppUnitTest.h"
#include "Event.h"
#include "EventQueue.h"
#include "SubscriberFoo.h"
#include "SubscriberBar.h"
#include "Foo.h"
#include "Bar.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace GameEngineLibrary;
using namespace UnitTestSupportClasses;
using namespace std;
using namespace std::chrono;

namespace LibraryDesktopTest
{
	TEST_CLASS(EventTest)
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

		TEST_METHOD(EventQueueSendTest)
		{
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> event(foo, false);
			event.Subscribe(subscriberFoo);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Send(event);
			Assert::IsTrue(subscriberFoo.WasNotified());

			event.UnSubscribe(subscriberFoo);
		}
		
		TEST_METHOD(EventEnqueNoDelay)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> event(foo, false);
			event.Subscribe(subscriberFoo);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Enqueue(event, gameTime);
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			event.UnSubscribe(subscriberFoo);
		}

		TEST_METHOD(EventUnsubscribeTest)
		{
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> event(foo, false);

			event.Subscribe(subscriberFoo);
			event.UnSubscribe(subscriberFoo);

			Assert::IsFalse(subscriberFoo.WasNotified());
			//eventQueue.Send(event);
			Assert::IsFalse(subscriberFoo.WasNotified());
		}

		TEST_METHOD(EventEnqueWithDelay)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> event(foo, false);
			event.Subscribe(subscriberFoo);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Enqueue(event, gameTime, 500U);
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(499));
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(1));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(10));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			event.UnSubscribeAll();
		}

		TEST_METHOD(EventEnqueWithDelayDynamicAllocation)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo>::Subscribe(subscriberFoo);
			Event<Foo>* event = new Event<Foo>(foo, true);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Enqueue(*event, gameTime, 500U);
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(499));
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(1));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(10));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());
		}

		TEST_METHOD(EventEnqueMoveCopyTest)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> event(foo, false);

			event.Subscribe(subscriberFoo);
			Event<Foo> newEvent = move(event);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Enqueue(newEvent, gameTime, 500U);
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(499));
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(1));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(10));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			newEvent.UnSubscribeAll();
		}

		TEST_METHOD(EventEnqueMoveAssignmentTest)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;

			Event<Foo> event(foo, false);
			event.Subscribe(subscriberFoo);

			Foo fooTemp;
			Event<Foo> newEvent(fooTemp, true);
			newEvent = move(event);

			Assert::IsFalse(subscriberFoo.WasNotified());
			eventQueue.Enqueue(newEvent, gameTime, 500U);
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(499));
			eventQueue.Update(gameTime);
			Assert::IsFalse(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(1));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(10));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());

			newEvent.UnSubscribeAll();
		}

		TEST_METHOD(EventRTTITest)
		{
			Foo foo;
			Event<Foo>* event = new Event<Foo>(foo, false);

			Assert::IsFalse(event->Is("Event<Foo>"));

			RTTI* eventRTTI = event->QueryInterface(event->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(eventRTTI) == event);
			Assert::IsTrue(event->Equals(eventRTTI));

			eventRTTI = event->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(eventRTTI) == event);
			Assert::IsTrue(event->Equals(eventRTTI));

			eventRTTI = event->As<Event<Foo>>();
			Assert::IsTrue(event->Equals(eventRTTI));

			Assert::IsTrue(eventRTTI->Is(event->TypeIdClass()));
			Assert::IsTrue(eventRTTI->Is(Event<Foo>::TypeIdClass()));

			Assert::IsTrue(eventRTTI->TypeIdInstance() == Event<Foo>::TypeIdClass());
			Assert::IsTrue(eventRTTI->TypeIdInstance() != Event<Bar>::TypeIdClass());

			Assert::IsTrue(eventRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());

			delete event;
		}

		TEST_METHOD(EventEnqueWithDelayDynamicAllocationWithBar)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;
			SubscriberBar subscriberBar;

			Foo foo;
			Event<Foo>* eventFoo = new Event<Foo>(foo, true);
			eventFoo->Subscribe(subscriberFoo);

			Bar bar;
			Event<Bar>* eventBar = new Event<Bar>(bar, true);
			eventBar->Subscribe(subscriberBar);

			Assert::IsFalse(subscriberFoo.WasNotified());
			Assert::IsFalse(subscriberBar.WasNotified());

			eventQueue.Enqueue(*eventFoo, gameTime, 500U);
			eventQueue.Enqueue(*eventBar, gameTime, 200U);
			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(201));
			eventQueue.Update(gameTime);

			Assert::IsFalse(subscriberFoo.WasNotified());
			Assert::IsTrue(subscriberBar.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(300));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());
		}

		TEST_METHOD(EventQueueSizeClearIsEmpty)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;
			SubscriberBar subscriberBar;

			Assert::IsTrue(eventQueue.Size() == 0);
			Assert::IsTrue(eventQueue.IsEmpty());
			eventQueue.Clear();
			Assert::IsTrue(eventQueue.Size() == 0);
			Assert::IsTrue(eventQueue.IsEmpty());

			Foo foo;
			Event<Foo>* eventFoo = new Event<Foo>(foo, true);
			eventFoo->Subscribe(subscriberFoo);

			Bar bar;
			Event<Bar>* eventBar = new Event<Bar>(bar, true);
			eventBar->Subscribe(subscriberBar);

			Assert::IsFalse(subscriberFoo.WasNotified());
			Assert::IsFalse(subscriberBar.WasNotified());

			eventQueue.Enqueue(*eventFoo, gameTime, 500U);
			Assert::IsTrue(eventQueue.Size() == 1U);
			Assert::IsFalse(eventQueue.IsEmpty());

			eventQueue.Enqueue(*eventBar, gameTime, 200U);
			Assert::IsTrue(eventQueue.Size() == 2U);
			Assert::IsFalse(eventQueue.IsEmpty());

			eventQueue.Clear();
			Assert::IsTrue(eventQueue.Size() == 0);
			Assert::IsTrue(eventQueue.IsEmpty());

			eventFoo = new Event<Foo>(foo, true);
			eventBar = new Event<Bar>(bar, true);

			eventQueue.Enqueue(*eventFoo, gameTime, 500U);
			eventQueue.Enqueue(*eventBar, gameTime, 200U);
			Assert::IsTrue(eventQueue.Size() == 2U);
			Assert::IsFalse(eventQueue.IsEmpty());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(201));
			eventQueue.Update(gameTime);

			Assert::IsFalse(subscriberFoo.WasNotified());
			Assert::IsTrue(subscriberBar.WasNotified());

			gameTime.SetCurrentTime(gameTime.CurrentTime() + milliseconds(300));
			eventQueue.Update(gameTime);
			Assert::IsTrue(subscriberFoo.WasNotified());
		}		

		TEST_METHOD(EventPublisherTimeEnqueuedAndDelayTest)
		{
			GameTime gameTime;
			EventQueue eventQueue;
			SubscriberFoo subscriberFoo;

			Foo foo;
			Event<Foo> eventFoo(foo, false);
			eventFoo.Subscribe(subscriberFoo);

			Bar bar;
			Event<Bar> eventBar(bar, false);
			eventBar.Subscribe(subscriberFoo);

			eventQueue.Enqueue(eventFoo, gameTime, 500U);
			eventQueue.Enqueue(eventBar, gameTime);

			Assert::IsTrue(eventFoo.TimeEnqueued() == gameTime.CurrentTime());
			Assert::IsTrue(eventFoo.Delay() == milliseconds(500U));

			Assert::IsTrue(eventBar.TimeEnqueued() == gameTime.CurrentTime());
			Assert::IsTrue(eventBar.Delay() == milliseconds());
		}

		TEST_METHOD(EventPublisherRTTI)
		{
			Foo foo;
			Event<Foo>* eventFoo = new Event<Foo>(foo, false);
			EventPublisher* event = eventFoo;

			Assert::IsFalse(event->Is("Event<Foo>"));

			RTTI* eventRTTI = event->QueryInterface(event->TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(eventRTTI) == event);
			Assert::IsTrue(event->Equals(eventRTTI));

			eventRTTI = event->QueryInterface(EventPublisher::TypeIdClass());
			Assert::IsTrue(static_cast<RTTI*>(eventRTTI) == event);
			Assert::IsTrue(event->Equals(eventRTTI));

			eventRTTI = event->As<Event<Foo>>();
			Assert::IsTrue(event->Equals(eventRTTI));

			Assert::IsTrue(eventRTTI->Is(event->TypeIdClass()));
			Assert::IsTrue(eventRTTI->Is(Event<Foo>::TypeIdClass()));

			Assert::IsTrue(eventRTTI->TypeIdInstance() == Event<Foo>::TypeIdClass());
			Assert::IsTrue(eventRTTI->TypeIdInstance() != Event<Bar>::TypeIdClass());

			Assert::IsTrue(eventRTTI->TypeIdInstance() != EventPublisher::TypeIdClass());			

			delete event;
		}
	private:
		static _CrtMemState sStartMemState;
	};
	_CrtMemState EventTest::sStartMemState;
}