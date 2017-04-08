#pragma once
#include "Action.h"

namespace GameEngineLibrary
{
	/** This class creates Actions in runtime.
	*/
	class ActionCreateAction :public Action
	{
	public:
		/** Default constructor. Initializes the private members of the class.				
		*/
		ActionCreateAction();

		/** Sets the class name and instance name of the Actions to be created.
		*	@param className The name of the class i.e of which type the object should be instantiated.
		*	@param instanceName The name to be set to the created Action object.
		*/
		void SetClassAndInstanceName(const std::string& actionClassName, const std::string& actionInstanceName);

		/** Creates actions of the specified class type.
		*	@param WorldState Represents the state of the World.
		*/
		virtual void Update(WorldState& worldState) override;

		/** Defaulted destructor.
		*/
		virtual ~ActionCreateAction() = default;
	protected:
		/** Initializes the the signatures(member variables) of the class.
		*/
		virtual void InitializeSignatures() override;
	private:
		/** The name of the class i.e of which type the Action object will be instantiated.
		*/
		std::string mActionClassName;

		/** The name to be set to the created Action object.
		*/
		std::string mActionInstanceName;
	public:
		RTTI_DECLARATIONS(ActionCreateAction, Action);
	};

	ActionFactory(ActionCreateAction);
}
