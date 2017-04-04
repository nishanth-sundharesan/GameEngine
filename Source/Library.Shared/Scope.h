#pragma once
#include "Vector.h"
#include "Hashmap.h"
#include "Datum.h"
#include "RTTI.h"

namespace GameEngineLibrary
{
	class Scope : public RTTI
	{
		/** Scope(Table) stores a Hashmap of std::string and Datum
		*/
		typedef Hashmap<std::string, Datum> ScopeHashmap;

		/** The Order Vector stores pointers to the inserted pairs. Type defining for convenience.
		*/
		typedef std::pair<std::string, Datum> PairType;

		/** Type defining HashmapIterator for convenience.
		*/
		typedef ScopeHashmap::Iterator ScopeHashmapIterator;

	public:
		/** Constructor: Initializes the private members of the class.
		*	@param size The size of the Scope(Table) if it is known in advance. Defaulted to 17
		*/
		explicit Scope(const std::uint32_t size = 17);

		/** Copy constructor: Performs deep copy of the scope.
		*	Note: After deep copying the scope, the parent of the deep copied scope is set to nullptr.
		*	@param rhsScope The right hand scope which has to be deep copied.
		*/
		Scope(const Scope& rhsScope);

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		Scope(Scope&& rhsScope);

		/** Assignment operator: Performs deep copy of the scope.
		*	Note: After deep copying the scope, the parent of the deep copied scope is set to nullptr.
		*	@param rhsScope The right hand scope which has to be deep copied.
		*	@return Returns the deep copied Scope object.
		*/
		Scope& operator=(const Scope& rhsScope);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*/
		Scope& operator=(Scope&& rhsScope);

		/** Clears the entire Scope.
		*/
		virtual ~Scope();

		/** This function takes in the name(key) of the Datum object and returns the pointer to it's associated Datum object.
		*	It returns nullptr if the Datum object was not found.
		*	@param name The name associated with the Datum object.
		*	@return Returns the pointer to the Datum object which is associated with the name. Returns nullptr if the Datum object was not found.
		*/
		Datum* Find(const std::string& name);

		/** This function takes in the name(key) of the Datum object and returns the pointer to it's associated Datum object.
		*	It returns nullptr if the Datum object was not found.
		*	@param name The name associated with the Datum object.
		*	@return Returns the pointer to the Datum object which is associated with the name. Returns nullptr if the Datum object was not found.
		*/
		const Datum* Find(const std::string& name) const;

		/** Searches for the Datum object associated with the specified name.
		*	This function takes in the name and a pointer to the scope pointer, inside which the search happens.
		*	If the scope double pointer is nullptr, then it searches in the current scope ad it's ancestors until it reaches the parent scope.
		*	It returns nullptr if the search fails.
		*	If the search succeeds, it assigns the pointer of the Scope inside which the Datum was found to the passed parameter "scope".
		*	@param name The name associated with the Datum object.
		*	@param scope The pointer to the scope pointer in which the search has to happen.
		*	@return Returns the pointer to the Datum object which is associated with the name. Returns nullptr if the Datum object was not found.
		*/
		Datum* Search(const std::string& name, Scope** scope = nullptr);

		/** Searches for the Datum object associated with the specified name.
		*	This function takes in the name and a pointer to the scope pointer, inside which the search happens.
		*	If the scope double pointer is nullptr, then it searches in the current scope ad it's ancestors until it reaches the parent scope.
		*	It returns nullptr if the search fails.
		*	If the search succeeds, it assigns the pointer of the Scope inside which the Datum was found to the passed parameter "scope".
		*	@param name The name associated with the Datum object.
		*	@param scope The pointer to the scope pointer in which the search has to happen.
		*	@return Returns the pointer to the Datum object which is associated with the name. Returns nullptr if the Datum object was not found.
		*/
		const Datum* Search(const std::string& name, Scope** scope = nullptr) const;

		/** Appends a new record inside the Scope/Table with the associated name.
		*	It then returns a reference to the Datum object after appending.
		*	If the name was already present inside the Scope, then it just returns a reference to the existing Datum associated with that name.
		*	@param name The name for the new record to be created.
		*	@return Returns a reference to the Datum object after appending. If the name was already present, then it returns a reference to the already present Datum.
		*/
		Datum& Append(const std::string& name);

		/** Appends a new scope inside the current scope.
		*	Appends a new record inside the Scope/Table with the associated name. It then creates a new Scope() and inserts it into the newly created Datum object of the newly created record.
		*	If a record of the same name was already present and it was of the type TABLE, then it creates a new Scope() inside that Datum and returns a reference to that newly created scope.
		*	@param name The name for the new record to be created.
		*	@exception throws an exception if the name was already present inside the scope and it wasn't of the type TABLE.
		*	@return Returns a reference to the newly appended scope.
		*/
		Scope& AppendScope(const std::string& name);

		/** This function takes in a reference to the scope, orphans it and Appends the childScope to the current scope.
		*	@param childScope The scope that has to be adopted to the current scope.
		*	@param name The name for the newly adopted scope.
		*	@exception Throws an exception if the scope that the user is trying to adopt is a root scope or if the scope was not found.
		*/
		void Adopt(Scope& childScope, const std::string& name);

		/** Returns the pointer to the parent scope.
		*	@return Returns the pointer to the parent scope.
		*/
		Scope* GetParent();

		/** Returns the pointer to the parent scope.
		*	@return Returns the pointer to the parent scope.
		*/
		const Scope* GetParent() const;

		/** Returns a reference to the Datum associated with the "name".
		*	If a Datum was not present with the passed name, then a new record in created and a reference for the newly created Datum is returned.
		*	@param name The name of the record whose datum reference has to be returned.
		*	@return Returns a reference to the Datum associated with the "name".
		*/
		Datum& operator[](const std::string& name);

		/** Returns a reference to the Datum associated with the "name".
		*	@param name The name of the record whose datum reference has to be returned.
		*	@return Returns a reference to the Datum associated with the "name".
		*	@exception Throws an exception if the record with the specified name was not found.
		*/
		const Datum& operator[](const std::string& name) const;

		/** Returns a reference to the Datum present at the specified "index".
		*	@index The index of the record containing the Datum.
		*	@return Returns a reference to the Datum present at the specified "index".
		*	@exception Throws an exception if the specified "index" is out of range.
		*/
		Datum& operator[](const std::uint32_t index);

		/** Returns a reference to the Datum present at the specified "index".
		*	@index The index of the record containing the Datum.
		*	@return Returns a reference to the Datum present at the specified "index".
		*	@exception Throws an exception if the specified "index" is out of range.
		*/
		const Datum& operator[](const std::uint32_t index) const;

		/** Compares two scope objects and returns true if both are equal, false otherwise.
		*	@param rhsScope The right hand side scope object which has to be compared.
		*	@return Returns true if both the scope objects are equal, false other wise.
		*/
		bool operator==(const Scope& rhsScope) const;

		/** Compares two scope objects and returns true if both are not equal, false otherwise.
		*	@param rhsScope The right hand side scope object which has to be compared.
		*	@return Returns true if the scope objects are not equal, false other wise.
		*/
		bool operator!=(const Scope& rhsScope) const;

		/** Finds the name associated with the record containing the passed scope pointer and returns it.
		*	@param scope The scope pointer present in the record whose name has to be found.
		*	@return Returns the name of the record containing the passed scope pointer. Returns empty string if the search failed.
		*/
		std::string FindName(const Scope* scope) const;

		/** Clears the entire scope and the child scopes if present.
		*/
		void Clear();

		/** Returns the std::string representation of the Scope.
		*	@return Returns the std::string representation of the Scope.
		*/
		virtual std::string ToString() const override;

		/** The overridden Equals method from RTTI.h.
		*	Returns true if the passed "rhs" pointer is a scope pointer and the content of it is equal to the current scope.
		*	@param rhs The right hand side scope pointer which has to be checked for equality.
		*	Returns true if the passed "rhs" pointer is a scope pointer and the content of it is equal to the current scope.
		*/
		virtual bool Equals(const RTTI* rhs) const override;
	protected:		
		/** A scope pointer pointing to the parent scope.
		*/
		Scope* mParentScope;

	private:
		/** Appends the passed scope to the current scope.
		*	Appends a new record inside the Scope/Table with the associated name. It then inserts the passed "scope" into the newly created Datum object of the newly created record.
		*	If a record of the same name was already present and it was of the type TABLE, then it inserts the passed "scope" inside that Datum and returns a reference to that newly created scope.
		*	@param name The name for the new record to be created.
		*	@param scope The scope pointer which has to be appended.
		*	@exception throws an exception if the name was already present inside the scope and it wasn't of the type TABLE.
		*	@return Returns a reference to the appended scope.
		*/
		Scope& AppendScope(const string& name, Scope* scope);

		/** Makes the parent scope point to the appropriate child scope.
		*	Note: This function gets called in the move semantics.
		*	@param oldScope Pointer to the old scope which will be searched for and replaced by the "this" pointer.
		*/
		void PointerFixUp(Scope* oldScope);

		/** Assigns the std::string representation of the DatumType into the std::string array.
		*/
		void AssignEnumToString();

		/** Orphans the passed "childScope" pointer from the current scope. i.e removes all it's links to the current scope.
		*	@param childScope The childScope which has to be Orphaned.
		*	@exception Throws an exception if the passed "childScope" is a root scope or if the "childScope" was not found.
		*/
		void Orphan(Scope* childScope);

		/** Performs a deep copy of the passed "rhsScope". This function is called recursively.
		*	Note: This function is called from the Copy constructor and the Assignment operator.
		*	@param rhsScope The right hand side which has to be deep copied.
		*/
		void PerformDeepCopy(const Scope& rhsScope);		

		/** The order vector containing the address of the records appended/inserted.
		*/
		Vector<PairType*> mOrderVector;

		/** The Hashmap containing the appended/inserted records.
		*/
		ScopeHashmap mHashmap;

		/** A temporary/cached Hashmap Iterator.
		*/
		mutable ScopeHashmapIterator mHashmapIterator;

		/** A std::string array containing the std::string representation of the DatumTypes.
		*/
		std::string mEnumNames[8];
	public:
		RTTI_DECLARATIONS(Scope, RTTI);
	};
}