#pragma once
#include "Action.h"

namespace GameEngineLibrary
{
	/** ActionList is a container for Actions.
	*/
	class ActionList :public Action
	{
	public:
		/** Default constructor. Initializes the private members of the class.
		*/
		ActionList();

		/** Marked copy constructor as delete. Cannot copy ActionLists.
		*/
		ActionList(const ActionList&) = delete;

		/** Marked assignment operator as delete. Cannot copy ActionLists.
		*/
		ActionList& operator=(const ActionList&) = delete;

		/** Returns the Datum containing Actions.
		*	@returns Returns the Datum containing Actions.
		*/
		Datum& Actions();

		/** Returns the Datum containing Actions.
		*	@returns Returns the Datum containing Actions.
		*/
		const Datum& Actions() const;

		/** Creates a new Action inside the ActionList and returns it.
		*	@param actionClassName The Action class to be created
		*	@param actionInstanceName The name of the Action to be created.
		*	@returns Returns the newly created Action.
		*/
		Action& CreateAction(const std::string& actionClassName, const std::string& actionInstanceName);

		/** Loops through the actions present in the action list and calls Update() on them.
		*	@param WorldState Represents the state of the World. While in this function, updates the Action* of WorldState to this.
		*/
		virtual void Update(WorldState& worldState) override;

		/** Defaulted destructor.
		*/
		virtual ~ActionList() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures() override;
	private:
		/** Cached datum containing Actions.
		*/
		const Datum* mActionsDatum;

		/** The name aside which the Actions are added to the scope.
		*/
		static const std::string sActionsName;
	public:
		RTTI_DECLARATIONS(ActionList, Action);
	};
}