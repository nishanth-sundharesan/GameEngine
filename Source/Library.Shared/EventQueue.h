#pragma once
#include "EventPublisher.h"
#include "GameTime.h"

namespace GameEngineLibrary
{
	/** EventQueue class which is responsible for queuing and dispatching the events.
	*/
	class EventQueue
	{
	public:
		/** Defaulted constructor.
		*/
		EventQueue() = default;

		/** Defaulted copy constructor.
		*/
		EventQueue(const EventQueue&) = default;

		/** Defaulted assignment operator.
		*/
		EventQueue& operator=(const EventQueue&) = default;

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		EventQueue(EventQueue&& rhs);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the right hand side object.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		EventQueue& operator=(EventQueue&& rhs);

		/** Enqueues the passed event.
		*	@param eventPublisher The event that has to be enqueued.
		*	@param gameTime The time when the event is enqueued.
		*	@param delay (Optional parameter)The amount of time after being enqueued that this event expires.
		*/
		void Enqueue(EventPublisher& eventPublisher, const GameTime& gameTime, const std::uint32_t delay = 0U);

		/** Dispatches the passed event synchronously.
		*	@param eventPublisher The event that has to be dispatched synchronously.
		*/
		void Send(EventPublisher& eventPublisher);

		/** Loops through the enqueued events and dispatches the expired events. It also deletes the events if it is marked for delete after publishing.
		*	@param gameTime The game time of the game.
		*/
		void Update(const GameTime& gameTime);

		/** Clears all the enqueued events.
		*/
		void Clear();

		/** Returns true if the enqueued event list is empty, false otherwise.
		*	@returns Returns true if the enqueued event list is empty, false otherwise.
		*/
		bool IsEmpty() const;

		/** Returns the size of the enqueued event list.
		*	@returns Returns the size of the enqueued event list.
		*/
		std::uint32_t Size() const;
		
		/** Defaulted virtual destructor.
		*/
		virtual ~EventQueue() = default;
	private:		
		/** List of enqueued events.
		*/
		Vector<EventPublisher*> mListEventPublishers;
	};
}