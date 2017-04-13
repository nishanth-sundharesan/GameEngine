#pragma once
#include "EventSubscriber.h"
#include "EventPublisher.h"

namespace UnitTestSupportClasses
{
	class SubscriberBar :public GameEngineLibrary::EventSubscriber
	{
	public:
		SubscriberBar();

		SubscriberBar(const SubscriberBar&) = default;

		SubscriberBar& operator=(const SubscriberBar&) = default;

		virtual void Notify(const  GameEngineLibrary::EventPublisher& eventPublisher) override;

		bool WasNotified() const;

		virtual ~SubscriberBar() = default;
	private:
		bool mIsNotifyCalled;
	};
}
