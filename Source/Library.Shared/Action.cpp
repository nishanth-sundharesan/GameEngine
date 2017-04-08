#include "Pch.h"
#include "Action.h"

using namespace std;

namespace GameEngineLibrary
{
	RTTI_DEFINITIONS(Action);

	const string Action::sActionsName = "Actions";
	const string Action::sAttributeName = "Name";

	Action::Action()
	{
		InitializeSignatures();
	}

	void Action::SetName(const string& name)
	{
		mName = name;
	}

	string& Action::Name()
	{
		return const_cast<string&>(const_cast<const Action*>(this)->Name());
	}

	const string& Action::Name() const
	{
		return mName;
	}

	void Action::InitializeSignatures()
	{
		AddExternalAttribute(sAttributeName, &mName, 1);
	}
}