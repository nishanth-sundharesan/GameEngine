#pragma once
#include "ActionList.h"
#include "ActionCreateAction.h"
#include "ActionDeleteAction.h"

namespace GameEngineLibrary
{
	/** Forward declared Sector class.
	*/
	class Sector;

	/** Entity class containing actions to be performed.
	*/
	class Entity :public Attributed
	{
	public:
		/** Default constructor. Initializes private members of the class.
		*/
		Entity();

		/** Marked copy constructor as delete. Cannot copy Entities.
		*/
		Entity(const Entity&) = delete;

		/** Marked assignment operator as delete. Cannot copy Entities.
		*/
		Entity& operator=(const Entity&) = delete;

		/** Returns the name of the Entity.
		*	@returns Returns the name of the Entity.
		*/
		std::string& Name();

		/** Returns the name of the Entity.
		*	@returns Returns the name of the Entity.
		*/
		const std::string& Name() const;

		/** Sets the name of the Entity.
		*	@param name The new name of the Entity.
		*/
		void SetName(const std::string& name);

		/** Returns the Datum containing Actions.
		*	@returns Returns the Datum containing Actions.
		*/
		Datum& Actions();

		/** Returns the Datum containing Actions.
		*	@returns Returns the Datum containing Actions.
		*/
		const Datum& Actions() const;

		/** Creates a new ActionList inside the Entity and returns it.
		*	@param actionListName The name of the ActionList to be created.
		*	@returns Returns the newly created ActionList.
		*/
		ActionList& CreateActionList(const std::string& actionListName);

		/** Creates a new Action inside the Entity and returns it.
		*	@param actionClassName The Action class to be created
		*	@param actionInstanceName The name of the Action to be created.
		*	@returns Returns the newly created Action.
		*/
		Action& CreateAction(const std::string& className, const std::string& instanceName);						

		/** Returns the reference to the Sector in which the Entity is present.
		*	@returns Returns the reference to the Sector in which the Entity is present.
		*/
		Sector& GetSector();

		/** Returns the reference to the Sector in which the Entity is present.
		*	@returns Returns the reference to the Sector in which the Entity is present.
		*/
		const Sector& GetSector() const;

		/** Updates all the Actions contained in the Entity.
		*	@param WorldState Represents the state of the World. While in this function, updates the Entity* to this of the WorldState.
		*/
		virtual void Update(WorldState& worldState);

		/** Defaulted Entity's destructor.
		*/
		virtual ~Entity() = default;
	protected:
		/** Initializes the signatures(member variables) of the Entity.
		*/
		virtual void InitializeSignatures();
	private:
		/** The name of the Entity.
		*/
		std::string mName;

		/** Cached datum containing Actions.
		*/
		Datum* mActionsDatum;

		/** The name aside which the Actions are added to the scope.
		*/
		static const std::string sActionsName;

		/** The attribute name for the name of the Entity.
		*/
		static const std::string sAttributeName;

		/** Name Of the createAction.
		*/
		static const std::string sActionCreateActionName;
	public:
		RTTI_DECLARATIONS(Entity, Attributed);
	};
}