#pragma once

namespace GameEngineLibrary
{
	/** Forward declared EventPublisher class.
	*/
	class EventPublisher;

	/** Abstract base class for all event subscribers.
	*/
	class EventSubscriber
	{
	public:
		/** Defaulted constructor.
		*/
		EventSubscriber() = default;

		/** Defaulted copy constructor.
		*/
		EventSubscriber(const EventSubscriber&) = default;

		/** Defaulted assignment operator.
		*/
		EventSubscriber& operator=(const EventSubscriber&) = default;

		/** Pure virtual function Notify. Derived classes must override this function to get notifications.
		*	@param eventPublisher The event publisher which contains the event message.
		*/
		virtual void Notify(const EventPublisher& eventPublisher) = 0;

		/** Defaulted virtual destructor.
		*/
		virtual ~EventSubscriber() = default;
	};
}