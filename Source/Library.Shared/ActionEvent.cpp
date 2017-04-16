#include "Pch.h"
#include "ActionEvent.h"
#include "EventMessage.h"
#include "Event.h"
#include "World.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ActionEvent);

	const string ActionEvent::sDelayAttributeName = "Delay";

	const string ActionEvent::sSubTypeAttributeName = "SubType";

	ActionEvent::ActionEvent()
		:mDelay(0), mSubType(string())
	{
		InitializeSignatures();
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		worldState.SetCurrentAction(this);

		EventMessage eventMessage(mSubType, worldState);
		eventMessage.CopyAuxiliaryAttributes(*this);

		Event<EventMessage>* event = new Event<EventMessage>(eventMessage, true);
		worldState.GetCurrentWorld()->GetEventQueue().Enqueue(*event, worldState.GetGameTime(), mDelay);

		worldState.SetCurrentAction(nullptr);
	}

	void ActionEvent::SetSubTypeAndDelay(const Datum& datum, const string& delay)
	{
		if (datum.Type() != DatumType::STRING)
		{
			throw exception("Invalid DatumType for the SubType");
		}
		mSubType = datum.Get<string>();
		sscanf_s(delay.c_str(), "%d", &mDelay);
	}
	
	void ActionEvent::InitializeSignatures()
	{
		Action::InitializeSignatures();
		AddExternalAttribute(sDelayAttributeName, &mDelay, 1);
		AddExternalAttribute(sSubTypeAttributeName, &mSubType, 1);
	}
}