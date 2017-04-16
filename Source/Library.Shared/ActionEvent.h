#pragma once
#include "Action.h"

namespace GameEngineLibrary
{
	/** ActionEvent class which generates events.
	*/
	class ActionEvent :public Action
	{
	public:
		/** Default constructor. Defaults the private members of the class and initializes the signatures.
		*/
		ActionEvent();

		/** Marked copy constructor as delete. Cannot copy ActionEvents.
		*/
		ActionEvent(const ActionEvent&) = delete;

		/** Marked assignment operator as delete. Cannot copy ActionEvents.
		*/
		ActionEvent& operator=(const ActionEvent&) = delete;

		/** Creates attributed EventMessages and Enqueues it with the set delay.
		*	@param worldState The WorldState object of the game engine.
		*/
		virtual void Update(WorldState& worldState) override;

		/** Sets the subType and the delay for the generated Events.
		*	@param subType A Datum containing the subType for the Events to be generated.
		*	@param delay The delay for the EventMessages which are generated in the Update().
		*/
		void SetSubTypeAndDelay(const Datum& subType, const std::string& delay);

		/** Defaulted virtual destructor.
		*/
		virtual ~ActionEvent() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures();
	private:
		/** The name for the prescribed attribute delay.
		*/
		static const std::string sDelayAttributeName;

		/** The name for the prescribed attribute subType.
		*/
		static const std::string sSubTypeAttributeName;

		/** Cached delay. Events will be generated with this delay.
		*/
		std::int32_t mDelay;

		/** Cached subType. Events will be generated with this subType.
		*/
		std::string mSubType;
	public:
		RTTI_DECLARATIONS(ActionEvent, Action);
	};

	ActionFactory(ActionEvent);
}
