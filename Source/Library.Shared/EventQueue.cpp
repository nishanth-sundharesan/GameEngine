#include "Pch.h"
#include "EventQueue.h"
#include <algorithm>

using namespace std;
using namespace std::chrono;

namespace GameEngineLibrary
{
	EventQueue::EventQueue(EventQueue&& rhs)
		:mListEventPublishers(move(rhs.mListEventPublishers))
	{
	}

	EventQueue& EventQueue::operator=(EventQueue&& rhs)
	{
		if (this != &rhs)
		{
			mListEventPublishers = move(rhs.mListEventPublishers);
		}
		return *this;
	}

	void EventQueue::Enqueue(EventPublisher& eventPublisher, const GameTime& gameTime, const uint32_t delay)
	{
		mListEventPublishers.PushBack(&eventPublisher);
		eventPublisher.SetTime(gameTime.CurrentTime(), milliseconds(delay));
	}

	void EventQueue::Send(EventPublisher& eventPublisher)
	{
		eventPublisher.Deliver();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		Vector<EventPublisher*>::Iterator partitionIterator = partition(mListEventPublishers.begin(), mListEventPublishers.end(),
			[&gameTime](const EventPublisher* eventPublisher)
		{
			return !(eventPublisher->IsExpired(gameTime.CurrentTime()));
		});

		for (auto iterator = partitionIterator; iterator != mListEventPublishers.end(); ++iterator)
		{
			(*iterator)->Deliver();
			if ((*iterator)->DeleteAfterPublishing())
			{
				delete *iterator;
			}
		}
		if (partitionIterator != mListEventPublishers.end())
		{
			mListEventPublishers.Remove(partitionIterator, mListEventPublishers.end());
		}
	}

	void EventQueue::Clear()
	{
		for (const EventPublisher* eventPublisher : mListEventPublishers)
		{
			if (eventPublisher->DeleteAfterPublishing())
			{
				delete eventPublisher;
			}
		}
		mListEventPublishers.Clear();
	}

	bool EventQueue::IsEmpty() const
	{
		return mListEventPublishers.Size() == 0;
	}

	uint32_t EventQueue::Size() const
	{
		return mListEventPublishers.Size();
	}
}