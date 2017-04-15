#include "Pch.h"
#include "ReactionAttributed.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Reaction);
	RTTI_DEFINITIONS(ReactionAttributed);

	const string ReactionAttributed::sSubTypeAttributeName = "SubType";

	ReactionAttributed::ReactionAttributed()
		:mSubType(string())
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
			CopyAuxiliaryAttributes(eventMessage);
			ActionList::Update(eventMessage.GetWorldState());
		}
	}

	void ReactionAttributed::SetSubType(const Datum& datum)
	{
		if (datum.Type() != DatumType::STRING)
		{
			throw exception("Invalid DatumType for the SubType");
		}
		mSubType = datum.Get<string>();
	}

	ReactionAttributed::~ReactionAttributed()
	{
		Event<EventMessage>::UnSubscribe(*this);
	}

	void ReactionAttributed::InitializeSignatures()
	{
		ActionList::InitializeSignatures();
		AddExternalAttribute(sSubTypeAttributeName, &mSubType, 1);
	}
}