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
	void Event<T>::Subscribe(EventSubscriber& eventSubscriber)
	{
		sListEventSubscribers.PushBack(&eventSubscriber);
	}

	template<class T>
	bool Event<T>::UnSubscribe(EventSubscriber& eventSubscriber)
	{
		return sListEventSubscribers.Remove(&eventSubscriber);
	}

	template<class T>
	void Event<T>::UnSubscribeAll()
	{
		sListEventSubscribers.Clear();
	}
}