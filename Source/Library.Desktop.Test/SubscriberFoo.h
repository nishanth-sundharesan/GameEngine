#pragma once
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace UnitTestSupportClasses
{
	class SubscriberFoo :public GameEngineLibrary::EventSubscriber
	{
	public:
		SubscriberFoo();

		SubscriberFoo(const SubscriberFoo&) = default;

		SubscriberFoo& operator=(const SubscriberFoo&) = default;

		virtual void Notify(const  GameEngineLibrary::EventPublisher& eventPublisher) override;

		bool WasNotified() const;

		virtual ~SubscriberFoo() = default;
	private:
		bool mIsNotifyCalled;
	};
}
