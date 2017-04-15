#pragma once
#include "Reaction.h"

namespace GameEngineLibrary
{
	/** ReactionAttributed class which receives notifications of the Event Messages.
	*/
	class ReactionAttributed :public Reaction
	{		
	public:
		/** Default constructor. Initializes the private members of the class, initializes the signatures and subscribes itself to the events of type Event<EventMessage>.
		*/
		ReactionAttributed();

		/** Marked copy constructor as delete. Cannot copy ReactionAttributed objects.
		*/
		ReactionAttributed(const ReactionAttributed&) = delete;

		/** Marked assignment operator as delete. Cannot copy ReactionAttributed objects.
		*/
		ReactionAttributed& operator=(const ReactionAttributed&) = delete;

		/** This method gets called whenever the Event delivery fires.
		*	@param eventPublisher The event publisher which contains the event message.
		*/
		virtual void Notify(const EventPublisher& eventPublisher) override;

		/** Sets the subType for the ReactionAttributed (ReactionAttributed is the Event receiver).
		*	@param subType A Datum containing the subType of the Reaction.
		*/
		void SetSubType(const Datum& subType);

		/** Virtual destructor. Unsubscribes itself from the events of type Event<EventMessage>.
		*/
		virtual ~ReactionAttributed();
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures() override;
	private:
		/** The name for the prescribed attribute subType.
		*/
		static const std::string sSubTypeAttributeName;

		/** Cached subType. The subtype(like a filter) for the ReactionAttributed.
		*/
		std::string mSubType;
	public:
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	};

	ReactionFactory(ReactionAttributed);
}