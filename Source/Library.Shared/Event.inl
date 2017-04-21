#include "Pch.h"
#include "Event.h"

using namespace std;

namespace GameEngineLibrary
{
	template<class T>
	RTTI_DEFINITIONS(Event<T>);

	template<class T>
	Vector<EventSubscriber*> Event<T>::sListEventSubscribers;

	template<class T>
	mutex Event<T>::mMutex;

	template<class T>
	Event<T>::Event(const T& message, const bool isDeleteAfterPublish)
		:EventPublisher(Event<T>::sListEventSubscribers, isDeleteAfterPublish), mMessage(message)
	{
	}

	template<class T>
	Event<T>::Event(Event&& rhs)
		: EventPublisher(move(rhs)), mMessage(move(rhs.mMessage))
	{
	}

	template<class T>
	Event<T>& Event<T>::operator=(Event&& rhs)
	{
		if (this != &rhs)
		{
			EventPublisher::operator=(move(rhs));
			mMessage = move(rhs.mMessage);
		}
		return *this;
	}

	template<class T>
	const T& Event<T>::Message() const
	{
		return mMessage;
	}

	template<class T>
	void Event<T>::ReserveSubscribers(uint32_t size)
	{
		lock_guard<mutex> lock(mMutex);
		sListEventSubscribers.Reserve(size);
	}

	template<class T>
	void Event<T>::Subscribe(EventSubscriber& eventSubscriber)
	{
		lock_guard<mutex> lock(mMutex);
		sListEventSubscribers.PushBack(&eventSubscriber);
	}

	template<class T>
	bool Event<T>::UnSubscribe(EventSubscriber& eventSubscriber)
	{
		lock_guard<mutex> lock(mMutex);
		return sListEventSubscribers.Remove(&eventSubscriber);
	}

	template<class T>
	void Event<T>::UnSubscribeAll()
	{
		lock_guard<mutex> lock(mMutex);
		sListEventSubscribers.Clear();		
	}
}