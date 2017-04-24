#pragma once
#include "EventSubscriber.h"
#include "EventPublisher.h"
#include "EventQueue.h"

namespace UnitTestSupportClasses
{
	enum class AsyncAction
	{
		SUBSCRIBE,
		UNSUBSCRIBE,
		CLEARSUBSCRIBERS,
		ENQUEUE,
		QUEUECLEAR,
		THROWEXCEPTION
	};

	class AsyncEventSubscriber :public GameEngineLibrary::EventSubscriber
	{
	public:
		AsyncEventSubscriber(AsyncAction asyncAction, GameEngineLibrary::EventQueue& eventQueue, GameEngineLibrary::GameTime& gameTime);

		AsyncEventSubscriber(const AsyncEventSubscriber&) = default;

		AsyncEventSubscriber& operator=(const AsyncEventSubscriber&) = default;

		virtual void Notify(const GameEngineLibrary::EventPublisher& eventPublisher) override;

		bool WasNotified() const;

		std::uint32_t NumberOfTimesCalled() const;

		virtual ~AsyncEventSubscriber() = default;
	private:
		bool mIsNotifyCalled;

		AsyncAction mAsyncAction;

		GameEngineLibrary::EventQueue* mSavedEventQueue;

		GameEngineLibrary::GameTime* mSavedGameTime;

		std::uint32_t mNumberOfTimesCalled;
	};
}