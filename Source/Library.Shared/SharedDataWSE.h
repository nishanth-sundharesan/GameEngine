/** Shared data for World, Sector and Entity.
*/
#pragma once
#include "SharedDataTable.h"
#include "World.h"

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
		*	@exception Throws exception if the World doesn't exist.
		*/
		void CreateSector(const std::string& name);

		/** This function gets called when a start Entity element is encountered. Creates Entity Object.
		*	@param className The name of the class of which type the object has to be instantiated.
		*	@param instanceName The name of the Entity.
		*	@exception Throws exception if the Sector doesn't exist.
		*/
		void CreateEntity(const std::string& className, const std::string& instanceName);

		/** Creates a new Datum based on the passed parameters.
		*	@param name The name of the Datum to be created.
		*	@param datumType The DatumType for the new Datum.
		*	@param value The value to be inserted in the Datum.(In the form of string)
		*	@exception Throws an exception if the primitive type xml element isn't present inside the World, Sector or Entity element.
		*/
		void AppendPrimitiveData(const std::string& name, const DatumType datumType, const std::string& value);

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

		/** Cached Scope pointer.
		*/
		Scope* mCurrentScope;
	public:
		RTTI_DECLARATIONS(SharedDataWSE, SharedData);
	};
}
