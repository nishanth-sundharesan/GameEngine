#include "Pch.h"
#include "EventPublisher.h"
#include <future>

using namespace std::chrono;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(EventPublisher);

	EventPublisher::EventPublisher(const Vector<EventSubscriber*>& listEventSubscribers, const bool isDeleteAfterPublish)
		:mEventSubscribers(&listEventSubscribers), mIsDeleteAfterPublishing(isDeleteAfterPublish), mCurrentTime(high_resolution_clock::time_point()), mDelay(milliseconds())
	{
	}

	EventPublisher::EventPublisher(EventPublisher&& rhs)
		: mCurrentTime(move(rhs.mCurrentTime)), mDelay(move(rhs.mDelay)), mEventSubscribers(rhs.mEventSubscribers), mIsDeleteAfterPublishing(rhs.mIsDeleteAfterPublishing)
	{
		rhs.mEventSubscribers = nullptr;
		rhs.mIsDeleteAfterPublishing = false;
	}

	EventPublisher& EventPublisher::operator=(EventPublisher&& rhs)
	{
		if (this != &rhs)
		{
			mCurrentTime = move(rhs.mCurrentTime);
			mDelay = move(rhs.mDelay);
			mEventSubscribers = rhs.mEventSubscribers;
			mIsDeleteAfterPublishing = rhs.mIsDeleteAfterPublishing;

			rhs.mEventSubscribers = nullptr;
			rhs.mIsDeleteAfterPublishing = false;
		}
		return *this;
	}

	void EventPublisher::SetTime(const high_resolution_clock::time_point& currentTime, const milliseconds& delay)
	{
		mCurrentTime = currentTime;
		mDelay = delay;
	}

	high_resolution_clock::time_point& EventPublisher::TimeEnqueued()
	{
		return const_cast<high_resolution_clock::time_point&>(const_cast<const EventPublisher*>(this)->TimeEnqueued());
	}

	const high_resolution_clock::time_point& EventPublisher::TimeEnqueued() const
	{
		return mCurrentTime;
	}

	milliseconds& EventPublisher::Delay()
	{
		return const_cast<milliseconds&>(const_cast<const EventPublisher*>(this)->Delay());
	}

	const milliseconds& EventPublisher::Delay() const
	{
		return mDelay;
	}

	bool EventPublisher::IsExpired(const high_resolution_clock::time_point& currentTime) const
	{
		if (mCurrentTime + mDelay <= currentTime)
		{
			return true;
		}
		return false;
	}

	void EventPublisher::Deliver()
	{
		Vector<EventSubscriber*> tempEventSubscribers = *mEventSubscribers;

		vector<future<void>> futureList;
		for (EventSubscriber* eventSubscriber : tempEventSubscribers)
		{
			futureList.emplace_back(async(launch::async, [eventSubscriber, this]() {
				eventSubscriber->Notify(*this);
			}));
		}

		for (auto& futureInstance : futureList)
		{
			try
			{
				futureInstance.get();
			}
			catch (...)
			{
				mExceptionList.push_back(current_exception());
			}
		}
		if (!mExceptionList.empty())
		{
			throw mExceptionList;
		}
	}

	bool EventPublisher::DeleteAfterPublishing() const
	{
		return mIsDeleteAfterPublishing;
	}
}