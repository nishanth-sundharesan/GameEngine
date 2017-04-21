#include "Pch.h"
#include "EventQueue.h"
#include <algorithm>
#include <vector>

using namespace std;
using namespace std::chrono;

namespace GameEngineLibrary
{
	void EventQueue::Enqueue(EventPublisher& eventPublisher, const GameTime& gameTime, const uint32_t delay)
	{
		lock_guard<mutex> lock(mMutex);
		mListEventPublishers.push_back(&eventPublisher);
		eventPublisher.SetTime(gameTime.CurrentTime(), milliseconds(delay));
	}

	void EventQueue::Send(EventPublisher& eventPublisher)
	{
		eventPublisher.Deliver();
	}

	void EventQueue::Update(const GameTime& gameTime)
	{
		vector<EventPublisher*>::iterator partitionIterator = partition(mListEventPublishers.begin(), mListEventPublishers.end(),
			[&gameTime](const EventPublisher* eventPublisher)
		{
			return !(eventPublisher->IsExpired(gameTime.CurrentTime()));
		});

		vector<EventPublisher*> expiredList(partitionIterator, mListEventPublishers.end());
		mListEventPublishers.erase(partitionIterator, mListEventPublishers.end());

		vector<future<void>> futureList;
		for (auto iterator = expiredList.begin(); iterator != expiredList.end(); ++iterator)
		{
			futureList.emplace_back(async(launch::async, [iterator, &mListExceptions = mListExceptions]() {
				try
				{
					(*iterator)->Deliver();
					if ((*iterator)->DeleteAfterPublishing())
					{
						delete *iterator;
					}
				}
				catch (vector<std::exception_ptr>& listExceptions)									//If an exception occurs in the child thread, then catch it and add all the exceptions to the list, which will be thrown back to the caller thread.
				{					
					if ((*iterator)->DeleteAfterPublishing())										//Clear up before logging the exception
					{
						delete *iterator;
					}
					mListExceptions.insert(mListExceptions.end(), listExceptions.begin(), listExceptions.end());
				}
			}));
		}

		for (auto& futureInstance : futureList)
		{
			futureInstance.get();
		}

		if (!mListExceptions.empty())																//If the child threads have thrown any exception.
		{
			throw mListExceptions;
		}
	}

	void EventQueue::Clear()
	{
		lock_guard<mutex> lock(mMutex);
		for (const EventPublisher* eventPublisher : mListEventPublishers)
		{
			if (eventPublisher->DeleteAfterPublishing())
			{
				delete eventPublisher;
			}
		}
		mListEventPublishers.clear();
	}

	bool EventQueue::IsEmpty() const
	{
		lock_guard<mutex> lock(mMutex);
		return mListEventPublishers.size() == 0;
	}

	uint32_t EventQueue::Size() const
	{
		lock_guard<mutex> lock(mMutex);
		return static_cast<uint32_t>(mListEventPublishers.size());
	}
}