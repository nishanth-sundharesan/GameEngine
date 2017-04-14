#pragma once
#include "Reaction.h"

namespace GameEngineLibrary
{
	class ReactionAttributed :public Reaction
	{		
		ReactionAttributed();

		ReactionAttributed(const ReactionAttributed&) = delete;

		ReactionAttributed& operator=(const ReactionAttributed&) = delete;

		virtual void Notify(const EventPublisher& eventPublisher) override;

		virtual ~ReactionAttributed() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures() override;
	private:
		static const std::string sSubTypeAttributeName;

		std::int32_t mSubType;
	public:
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);
	};
}