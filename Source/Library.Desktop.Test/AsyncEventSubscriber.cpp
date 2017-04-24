#include "Pch.h"
#include "Event.h"
#include "Foo.h"
#include "AsyncEventSubscriber.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	AsyncEventSubscriber::AsyncEventSubscriber(AsyncAction asyncAction, EventQueue& eventQueue, GameTime& gameTime)
		:mIsNotifyCalled(false), mAsyncAction(asyncAction), mSavedEventQueue(&eventQueue), mSavedGameTime(&gameTime), mNumberOfTimesCalled(0)
	{
	}

	void AsyncEventSubscriber::Notify(const EventPublisher& eventPublisher)
	{
		if (eventPublisher.Is(Event<Foo>::TypeIdClass()))
		{
			mIsNotifyCalled = true;
			++mNumberOfTimesCalled;

			Foo foo = static_cast<const Event<Foo>&>(eventPublisher).Message();

			switch (mAsyncAction)
			{
			case AsyncAction::SUBSCRIBE:
				Event<Foo>::Subscribe(*this);
				break;

			case AsyncAction::UNSUBSCRIBE:
				Event<Foo>::UnSubscribe(*this);
				break;

			case AsyncAction::CLEARSUBSCRIBERS:
				Event<Foo>::UnSubscribeAll();
				break;

			case AsyncAction::QUEUECLEAR:
				mSavedEventQueue->Clear();
				break;

			case AsyncAction::THROWEXCEPTION:
				throw exception("Beauty is only skin deep!");
				break;

			case AsyncAction::ENQUEUE:
				Event<Foo>* event = new Event<Foo>(foo, true);
				mSavedEventQueue->Enqueue(*event, *mSavedGameTime);				
				break;			
			}
		}
	}

	bool AsyncEventSubscriber::WasNotified() const
	{
		return mIsNotifyCalled;
	}

	uint32_t AsyncEventSubscriber::NumberOfTimesCalled() const
	{
		return mNumberOfTimesCalled;
	}
}
