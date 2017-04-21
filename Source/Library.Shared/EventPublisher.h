#pragma once
#include "RTTI.h"
#include "EventSubscriber.h"
#include "Vector.h"
#include <chrono>

namespace GameEngineLibrary
{
	/** Base class for Events.
	*/
	class EventPublisher :public RTTI
	{	
	protected:
		/** Parameterized constructor. Caches the parameters and initializes the private members of the class.
		*	@param listEventSubscribers The list of Event Subscribers which will be cached.
		*	@param isDeleteAfterPublish A boolean which indicates if the event has to be deleted after publishing. Caches the boolean.
		*/
		EventPublisher(const Vector<EventSubscriber*>& listEventSubscribers, const bool isDeleteAfterPublish);
	public:
		/** Defaulted copy constructor.
		*/
		EventPublisher(const EventPublisher&) = default;

		/** Defaulted assignment operator.
		*/
		EventPublisher& operator=(const EventPublisher&) = default;

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		EventPublisher(EventPublisher&& rhs);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		EventPublisher& operator=(EventPublisher&& rhs);
	
		/** Sets the current time when the event was enqueued. A delay could also be passed which when expired, the event would be callable.
		*	@param currentTime The time when the event was enqueued.
		*	@param delay (Optional parameter)The amount of time after being enqueued that this event expires.
		*/
		void SetTime(const std::chrono::high_resolution_clock::time_point& currentTime, const std::chrono::milliseconds& delay = std::chrono::milliseconds());

		/** Returns the time when the event was enqueued.
		*	@returns Returns the time when the event was enqueued.
		*/
		std::chrono::high_resolution_clock::time_point& TimeEnqueued();

		/** Returns the time when the event was enqueued.
		*	@returns Returns the time when the event was enqueued.
		*/
		const std::chrono::high_resolution_clock::time_point& TimeEnqueued() const;

		/** Returns the amount of time after being enqueued that this event expires.
		*	@returns Returns the amount of time after being enqueued that this event expires.
		*/
		std::chrono::milliseconds& Delay();

		/** Returns the amount of time after being enqueued that this event expires.
		*	@returns Returns the amount of time after being enqueued that this event expires.
		*/
		const std::chrono::milliseconds& Delay() const;

		/** Takes in the time_point and returns true if the event has expired. (time enqueued + delay)
		*	@param currentTime The time against which the event has to be checked if it has expired.
		*	@returns Returns true if the event has expired, false otherwise.
		*/
		bool IsExpired(const std::chrono::high_resolution_clock::time_point& currentTime) const;

		/** Notifies all the subscribers who have subscribed to the event.
		*	Note: A separate thread is spun up for each notification call to the subscriber.
		*/
		void Deliver();

		/** Returns true if the event should be deleted after it is published, false otherwise.
		*	@param Returns true if the event should be deleted after it is published, false otherwise.
		*/
		bool DeleteAfterPublishing() const;

		/** Defaulted virtual destructor.
		*/
		virtual ~EventPublisher() = default;
	private:
		/** The time when the event was enqueued.
		*/
		std::chrono::high_resolution_clock::time_point mCurrentTime;

		/** The amount of time after being enqueued that this event expires.
		*/
		std::chrono::milliseconds mDelay;

		/** Pointer to the list of Event subscribers.
		*/
		const Vector<EventSubscriber*>* mEventSubscribers;

		/** A boolean indicating if the event should be deleted after it is published.
		*/
		bool mIsDeleteAfterPublishing;

		/** A vector to hold the exceptions thrown by the child threads.
		*/
		std::vector<std::exception_ptr> mExceptionList;
	public:
		RTTI_DECLARATIONS(EventPublisher, RTTI);
	};
}