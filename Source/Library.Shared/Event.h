#pragma once
#include "EventPublisher.h"
#include <future>

namespace GameEngineLibrary
{
	/** Templated final Event class which contains the event message.
	*/
	template<class T>
	class Event final :public EventPublisher
	{
	public:
		/** Parameterized constructor. Caches the parameters.
		*	@param message The actual message of the event. This will be cached inside Event.
		*	@param isDeleteAfterPublish A boolean which indicates if the event has to be deleted after publishing.
		*/
		Event(const T& message, const bool isDeleteAfterPublish);

		/** Defaulted copy constructor.
		*/
		Event(const Event&) = default;

		/** Defaulted assignment operator.
		*/
		Event& operator=(const Event&) = default;

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		Event(Event&& rhs);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		Event& operator=(Event&& rhs);

		/** Returns the actual message of the event.
		*	@returns Returns the actual message of the event.
		*/
		const T& Message() const;

		/** Reserves the memory for the subscribers list to the specified capacity.
		*	@param capacity The new capacity of the Subscribers list. This should be greater than the current capacity.
		*/
		static void ReserveSubscribers(std::uint32_t size);

		/** Adds the passed subscriber to the Event Subscribers list.
		*	@param eventSubscriber The event subscriber which has subscribed to get notifications of the event.
		*/
		static void Subscribe(EventSubscriber& eventSubscriber);
		
		/** Removes the passed subscriber from the Event Subscribers list.
		*	@param eventSubscriber The event subscriber which has decided to stop receiving notification of the event.
		*/
		static bool UnSubscribe(EventSubscriber& eventSubscriber);

		/** Unsubscribes all subscribers from the subscribers list.
		*/
		static void UnSubscribeAll();
	private:
		/** Mutex to lock the vector list of EventSubscribers.
		*/
		static mutex mMutex;

		/** The actual message of the event.
		*/
		T mMessage;

		/** Static list of event subscribers.
		*/
		static Vector<EventSubscriber*> sListEventSubscribers;
	public:
		RTTI_DECLARATIONS(Event<T>, EventPublisher);
	};
}

#include "Event.inl"