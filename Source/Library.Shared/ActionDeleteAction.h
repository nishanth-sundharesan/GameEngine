#pragma once
#include "Action.h"

namespace GameEngineLibrary
{
	/** This class deletes Actions at runtime.
	*/
	class ActionDeleteAction :public Action 
	{
	public:
		/** Default constructor. Initializes the private members of the class.
		*/
		ActionDeleteAction();

		/** Sets the name of the Action to delete.
		*	@param actionToDelete The name of the Action to delete.
		*/
		void SetActionToDelete(const std::string& actionToDelete);

		/** Creates actions of the specified name.
		*	@param WorldState Represents the state of the World.
		*/
		virtual void Update(WorldState& worldState) override;

		/** Defaulted destructor.
		*/
		virtual ~ActionDeleteAction() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures() override;
	private:		
		/** The name of the Action to delete.
		*/
		std::string mActionToDelete;
	public:
		RTTI_DECLARATIONS(ActionDeleteAction, Action);
	};

	ActionFactory(ActionDeleteAction);
}
