#include "Pch.h"
#include "ReactionAttributed.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(ReactionAttributed);

	const string ReactionAttributed::sSubTypeAttributeName = "SubType";

	ReactionAttributed::ReactionAttributed()
		:mSubType(0)
	{
		InitializeSignatures();
		Event<EventMessage>::Subscribe(*this);
	}

	void ReactionAttributed::Notify(const EventPublisher& eventPublisher)
	{
		assert(eventPublisher.Is(Event<EventMessage>::TypeIdClass()));

		EventMessage eventMessage = static_cast<const Event<EventMessage>&>(eventPublisher).Message();
		if (eventMessage.GetSubType() == mSubType)
		{
			//TODO confirm this
			CopyAuxiliaryAttributes(eventMessage);
			ActionList::Update(eventMessage.GetWorldState());
		}
	}

	void ReactionAttributed::InitializeSignatures()
	{
		ActionList::InitializeSignatures();
		AddExternalAttribute(sSubTypeAttributeName, &mSubType, 1);
	}
}