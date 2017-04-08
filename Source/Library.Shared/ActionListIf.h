#pragma once
#include "ActionList.h"

namespace GameEngineLibrary
{
	/** Action class for the If condition.
	*/
	class ActionListIf :public ActionList
	{
	public:
		/** Defaulted constructor. Initializes the private members of the class.
		*/
		ActionListIf();

		/** Marked copy constructor as delete. Cannot copy ActionListIf.
		*/
		ActionListIf(const ActionListIf&) = delete;

		/** Marked assignment operator as delete. Cannot copy ActionListIf.
		*/
		ActionListIf& operator=(const ActionListIf&) = delete;

		/** Compares the two datums and checks which Action to execute (then or else).
		*	@param WorldState Represents the state of the World. While in this function, sets Action* to this of the WorldState.
		*/
		virtual void Update(WorldState& worldState) override;

		/** Sets the two datums to compare.
		*	@param lhsValue The left hand side to compare.
		*	@param rhsValue The right hand side to compare.
		*/
		void SetComparisons(const Datum& lhsValue, const Datum& rhsValue);
		
		/** Sets the Then Action block.
		*	@param thenBlockAction The Then action block to be cached.
		*/
		void SetThenBlockAction(const Action& thenBlockAction);

		/** Sets the Else Action block.
		*	@param elseBlockAction The Else action block to be cached.
		*/
		void SetElseBlockAction(const Action& elseBlockAction);

		/** Defaulted destructor.
		*/
		virtual ~ActionListIf() = default;
	protected:
		/** Initializes the attributes of the class.
		*/
		virtual void InitializeSignatures() override;
	private:
		/** Cached left hand side Datum pointer.
		*/
		const Datum* mLhsValueDatum;

		/** Cached right hand side Datum pointer.
		*/
		const Datum* mRhsValueDatum;

		/** Action pointer to the Then Action.
		*/
		Action* mThenBlockAction;

		/** Action pointer to the Else Action.
		*/
		Action* mElseBlockAction;

		/** The attribute name for the condition.
		*/
		static const std::string sConditionAttributeName;
	public:
		RTTI_DECLARATIONS(ActionListIf, ActionList);
	};

	ActionFactory(ActionListIf);
}