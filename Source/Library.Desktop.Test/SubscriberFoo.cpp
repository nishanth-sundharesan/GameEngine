#include "Pch.h"
#include "SubscriberFoo.h"
#include "Event.h"
#include "Foo.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	SubscriberFoo::SubscriberFoo()
		:mIsNotifyCalled(false)
	{
	}

	void SubscriberFoo::Notify(const EventPublisher& eventPublisher)
	{
		if (eventPublisher.Is(Event<Foo>::TypeIdClass()))
		{
			mIsNotifyCalled = true;
			Foo foo = static_cast<const Event<Foo>&>(eventPublisher).Message();
		}		
	}

	bool SubscriberFoo::WasNotified() const
	{
		return mIsNotifyCalled;
	}
}