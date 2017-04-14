#pragma once
#include "Action.h"

namespace GameEngineLibrary
{
	class ActionEvent :public Action
	{
	public:		
		ActionEvent();

		ActionEvent(const ActionEvent&) = delete;

		ActionEvent& operator=(const ActionEvent&) = delete;

		virtual void Update(WorldState& worldState) override;

		virtual ~ActionEvent() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures();
	private:
		static const std::string sDelayAttributeName;

		static const std::string sSubTypeAttributeName;

		std::int32_t mDelay;

		std::int32_t mSubType;
	public:
		RTTI_DECLARATIONS(ActionEvent, Action);
	};
}
