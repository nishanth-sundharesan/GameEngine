#pragma once
#include "EventPublisher.h"
#include "GameTime.h"
#include <future>

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

		/** Deleted copy constructor. Cannot copy EventQueue.
		*/
		EventQueue(const EventQueue&) = delete;

		/** Deleted assignment operator. Cannot copy EventQueue.
		*/
		EventQueue& operator=(const EventQueue&) = delete;		

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
		*	Note: The dispatched events are all individual threads.
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
		/** Mutex to lock the Vector list of EventPublishers.
		*/
		mutable std::mutex mMutex;

		/** List of enqueued events.
		*/
		vector<EventPublisher*> mListEventPublishers;

		/** List of exceptions occurred in this child thread.
		*/
		vector<std::exception_ptr> mListExceptions;
	};
}