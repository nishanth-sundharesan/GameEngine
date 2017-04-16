/** Shared data for World, Sector and Entity.
*/
#pragma once
#include "SharedDataTable.h"
#include "World.h"
#include "ActionListIf.h"

namespace GameEngineLibrary
{
	/** Shared Data class for World, Entity and Sector.
	*/
	class SharedDataWSE :public XmlParseMaster::SharedData
	{
		typedef XmlParseMaster::SharedData SharedData;
	public:
		/** Zero parameterized constructor. Defaults members of the class.
		*/
		SharedDataWSE();

		/** Marking the copy constructor as delete as creating an exact copy of the SharedDataWSE class would be inappropriate
		*	Note: Clone() can be used to make copies of SharedDataWSE.
		*	@see Clone()
		*/
		SharedDataWSE(const SharedDataWSE&) = delete;

		/** Marking the assignment operator as delete as creating an exact copy of the SharedDataWSE class would be inappropriate
		*	Note: Clone() can be used to make copies of SharedDataWSE.
		*	@see Clone()
		*/
		SharedDataWSE& operator=(const SharedDataWSE&) = delete;

		/** Initializes/Defaults the members of the class.
		*/
		virtual void Initialize() override;

		/** Virtual Clone function which creates copies of SharedDataWSE.
		*	This function is marked as virtual so that the derived classes can Clone their copies in the sense of a virtual constructor.
		*/
		virtual SharedData* Clone() const override;

		/** This function gets called when a start World element is encountered. Creates World Object.
		*	@param name The name of the World.
		*	@exception Throws exception if the World already exists.
		*/
		void CreateWorld(const std::string& name);

		/** Returns the pointer to the World.
		*	Note: Can be nullptr.
		*	@returns Returns the pointer to the World.
		*/
		World* GetWorld();

		/** Returns the pointer to the World.
		*	Note: Can be nullptr.
		*	@returns Returns the pointer to the World.
		*/
		const World* GetWorld() const;

		/** This function gets called when a start Sector element is encountered. Creates Sector Object.
		*	@param name The name of the Sector.
		*	@exception Throws an exception if the World doesn't exist.
		*/
		void CreateSector(const std::string& name);

		/** This function gets called when a start Entity element is encountered. Creates Entity Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Entity.
		*	@exception Throws an exception if the Sector doesn't exist.
		*/
		void CreateEntity(const std::string& className, const std::string& instanceName);

		/** This function gets called when a start ActionList element is encountered. Creates ActionList Object.
		*	@param name The name of the ActionList.
		*	@exception Throws an exception if the ActionList is not present inside Entity element.
		*/
		void CreateActionList(const std::string& name);

		/** This function gets called when a start Action element is encountered. Creates Action Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Action.
		*	@exception Throws exception if the Entity or ActionList doesn't exist.
		*/
		void CreateAction(const std::string& className, const std::string& instanceName);

		/** This function gets called when a start Reaction element is encountered. Creates Reaction Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Reaction.
		*	@param subType The subType filter for the reaction.
		*	@exception Throws exception if the Entity doesn't exist.
		*/
		void CreateReaction(const std::string& className, const std::string& instanceName, const std::string& subType);

		/** This function gets called when a start ActionEvent element is encountered. Creates ActionEvent Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the ActionEvent.
		*	@param subType The subType filter for the EventMessage.
		*	@exception Throws exception if the Entity or Action List doesn't exist.
		*/
		void CreateActionEvent(const std::string& className, const std::string& instanceName, const std::string& subType, const std::string& delay);

		/** This function gets called when a start Then element is encountered. Creates ThenAction Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Action.
		*	@exception Throws exception if the ActionListIf doesn't exist.
		*/
		void AddThenAction(const std::string& className, const std::string& instanceName) const;

		/** This function gets called when a start Else element is encountered. Creates ElseAction Object.
		*	@param className The name of the class of which type the object(Action) has to be instantiated.
		*	@param instanceName The name of the Action.
		*	@exception Throws exception if the ActionListIf doesn't exist.
		*/
		void AddElseAction(const std::string& className, const std::string& instanceName) const;

		/** This function gets called when a start CreateAction element is encountered. Creates ActionCreateAction Object.
		*	@param className The name of the class of which type the object(Action) has to be instantiated.
		*	@param instanceName The name of the ActionCreateAction Object.
		*	@param actionClassName The name of the class i.e of which type the object has to be instantiated inside ActionCreateAction.
		*	@param actionInstanceName The name of the Action which will be created inside ActionCreateAction.
		*	@exception Throws exception if the Entity or ActionList doesn't exist.
		*/
		void AddActionCreateAction(const std::string& className, const std::string& instanceName, const std::string& actionClassName, const std::string& actionInstanceName) const;

		/** This function gets called when a start DeleteAction element is encountered. Creates ActionDeleteAction Object.
		*	@param className The name of the class of which type the object(Action) has to be instantiated.
		*	@param instanceName The name of the ActionDeleteAction object.
		*	@param actionInstanceName The name of the Action which should be deleted inside the ActionDeleteAction.		
		*	@exception Throws exception if the Entity or ActionList doesn't exist.
		*/
		void AddActionDeleteAction(const std::string& className, const std::string& instanceName, const std::string& actionInstanceName) const;

		/** This function gets called when a start If element is encountered. Creates Action Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Action.
		*	@exception Throws exception if the Entity or ActionList doesn't exist.
		*/
		void CreateActionIf(const std::string& className, const std::string& instanceName, const string& lhsComparisonPathValue, const string& rhsComparisonPathValue);

		/** Assigns the comparisons for the ActionListIf.
		*/
		void AssignComparisonsForIf() const;

		/** Creates a new Datum based on the passed parameters.
		*	@param name The name of the Datum to be created.
		*	@param datumType The DatumType for the new Datum.
		*	@param value The value to be inserted in the Datum.(In the form of string)
		*	@exception Throws an exception if the primitive type xml element isn't present inside the World, Sector or Entity element.
		*/
		void AppendPrimitiveData(const std::string& name, const DatumType datumType, const std::string& value) const;

		/** This function gets called when the World's, Sector's or Entity's end xml element is encountered. Points the cached Scope* to its Parent Scope.
		*/
		void GotoParentScope();

		/** Deletes the World if created.
		*/
		virtual ~SharedDataWSE();
	protected:
		/** Clones the internal members of the SharedDataWSE class.
		*	Note: This method gets called from the Clone().
		*	@param sharedData The cloned SharedData pointer.
		*/
		virtual void CloneInternalMembers(SharedData* sharedData) const override;
	private:
		/** Cached World pointer.
		*/
		World* mWorld;

		/** Cached Sector pointer.
		*/
		Sector* mSector;

		/** Cached Entity pointer.
		*/
		Entity* mEntity;

		/** Cached ActionList pointer.
		*/
		ActionList* mActionList;

		/** Cached ActionIf pointer.
		*/
		ActionListIf* mActionListIf;

		/** Cached Scope pointer.
		*/
		Scope* mCurrentScope;

		/** Cached path the left hand side value to compare.
		*/
		std::string mLhsComparisonPathValue;

		/** Cached path the right hand side value to compare.
		*/
		std::string mRhsComparisonPathValue;
	public:
		RTTI_DECLARATIONS(SharedDataWSE, SharedData);
	};
}
