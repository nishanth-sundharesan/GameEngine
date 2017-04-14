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
		:mDelay(0), mSubType(0)
	{
		InitializeSignatures();
	}

	void ActionEvent::Update(WorldState& worldState)
	{
		EventMessage eventMessage(mSubType, *worldState.GetCurrentWorld(), worldState);
		eventMessage.CopyAuxiliaryAttributes(*this);

		Event<EventMessage>* event = new Event<EventMessage>(eventMessage, true);
		worldState.GetCurrentWorld()->GetEventQueue().Enqueue(*event, worldState.GetGameTime(), mDelay);
	}

	void ActionEvent::InitializeSignatures()
	{
		Action::InitializeSignatures();
		AddExternalAttribute(sDelayAttributeName, &mDelay, 1);
		AddExternalAttribute(sSubTypeAttributeName, &mSubType, 1);
	}
}