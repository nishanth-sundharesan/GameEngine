#include "Pch.h"
#include "SubscriberBar.h"
#include "Event.h"
#include "Bar.h"

using namespace GameEngineLibrary;

namespace UnitTestSupportClasses
{
	SubscriberBar::SubscriberBar()
		:mIsNotifyCalled(false)
	{
	}

	void SubscriberBar::Notify(const EventPublisher& eventPublisher)
	{
		if (eventPublisher.Is(Event<Bar>::TypeIdClass()))
		{
			mIsNotifyCalled = true;
			Bar bar = static_cast<const Event<Bar>&>(eventPublisher).Message();
		}
	}

	bool SubscriberBar::WasNotified() const
	{
		return mIsNotifyCalled;
	}
}