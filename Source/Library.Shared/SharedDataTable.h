#pragma once
#include "Scope.h"
#include "XmlParseMaster.h"

namespace GameEngineLibrary
{
	/** Shared Data class for scope
	*/
	class SharedDataTable :public XmlParseMaster::SharedData
	{		
		typedef XmlParseMaster::SharedData SharedData;		
	public:
		/** Zero parameterized constructor which defaults private members.	
		*/
		SharedDataTable();

		/** Marking the copy constructor as delete as creating an exact copy of the SharedDataTable class would be inappropriate
		*	Note: Clone() can be used to make copies of SharedDataTable.
		*	@see Clone()
		*/
		SharedDataTable(const SharedDataTable&) = delete;

		/** Marking the assignment operator as delete as creating an exact copy of the SharedDataTable class would be inappropriate
		*	Note: Clone() can be used to make copies of SharedDataTable.
		*	@see Clone()
		*/
		SharedDataTable& operator=(const SharedDataTable&) = delete;

		/** Initializes/Defaults the members of the class.
		*/
		virtual void Initialize() override;

		/** Virtual Clone function which creates copies of SharedDataTable.
		*	This function is marked as virtual so that the derived classes can Clone their copies in the sense of a virtual constructor.
		*/
		virtual SharedData* Clone() const override;

		/** Returns the pointer to the Scope associated with the class.
		*	@returns Returns the pointer to the Scope associated with the class.
		*/
		Scope* GetScope();

		/** Returns the pointer to the Scope associated with the class.
		*	@returns Returns the pointer to the Scope associated with the class.
		*/
		const Scope* GetScope() const;

		/** Creates a new scope if called for the first time, otherwise appends scope to the existing scope.
		*	@param name The name of the scope to be appended.
		*/
		void AppendNewScope(const std::string& name);

		/** Makes the current scope point to the parent scope.
		*/
		void GotoParentScope();		

		/** Destructor, deletes the dynamically allocated private member variables.
		*/
		virtual ~SharedDataTable();
	protected:
		/** Clones the internal members of the SharedDataTable class.
		*	Note: This method gets called from the Clone().
		*	@param sharedData The cloned SharedData pointer.
		*/
		virtual void CloneInternalMembers(SharedData* sharedData) const override;
	private:
		/** Shared Scope pointer;
		*/
		Scope* mScope;
	public:
		RTTI_DECLARATIONS(SharedDataTable, SharedData);
	};
}