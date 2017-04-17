#pragma once
#include "Scope.h"

namespace GameEngineLibrary
{
	class Attributed :public Scope
	{
	public:
		/** Zero parameterized constructor.
		*	Appends the "this" pointer and adds it to the prescribed attribute list.
		*/
		Attributed();

		/** Copy constructor.
		*	Performs a deep copy of the Attributed instance.
		*	@param rhs The object of which the deep copy will be made.
		*/
		Attributed(const Attributed& rhs);

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		Attributed(Attributed&& rhs);

		/** Overloaded assignment operator.
		*	Performs a deep copy of the right hand side Attributed object.
		*	@param rhs It is the right hand side object which will be deep copied to the left hand side object.
		*	@return Returns the deep copied Attributed.
		*/
		Attributed& operator=(const Attributed& rhs);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*/
		Attributed& operator=(Attributed&& rhs);

		/** Clears the entire Attribute.
		*	Clears the static Hashmap of prescribed attributes if the number of Attributed instances reduces to 0;
		*/
		virtual ~Attributed();

		/** Returns true if an attribute is a Prescribed attribute, false otherwise.
		*	@param name The name of the attribute which has to be checked if it is a Prescribed attribute.
		*	@return Returns true if an attribute is a Prescribed attribute, false otherwise.
		*/
		bool IsPrescribedAttribute(const std::string& name) const;

		/** Returns true if an attribute is an Auxiliary attribute, false otherwise.
		*	@param name The name of the attribute which has to be checked if it is a Auxiliary attribute.
		*	@return Returns true if an attribute is an Auxiliary attribute, false otherwise.
		*/
		bool IsAuxiliaryAttribute(const std::string& name) const;

		/** Returns true if the passed name is an attribute, false otherwise.
		*	@param name The name of the attribute which has to be checked if it is an attribute.
		*	@return Returns true if the passed name is an attribute, false otherwise.
		*/
		bool IsAttribute(const std::string& name) const;

		/** Appends an AuxiliaryAttribute with the passed name.
		*	@param name The name for the AuxiliaryAttribute.
		*	@exception Throws an exception if the passed name is already a name for the Prescribed attribute.
		*	@return Returns the appended Datum.
		*/
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/** Returns the number of the Prescribed attributes.
		*	@return Returns the number of the Prescribed attributes.
		*/
		std::uint32_t AuxiliaryBegin();

		/** Copies all the Auxiliary attributes from the rhsAttribute to this Attribute.
		*	@param rhsAttribute The right hand side Attribute from which all the Auxiliary attributes have to be copied.
		*/
		void CopyAuxiliaryAttributes(const Attributed& rhsAttribute);
	protected:
		/** Adds "this" to the prescribed attribute list.
		*/
		void InitializeSignatures();

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const std::int32_t value);

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const std::float_t value);

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const std::string& value);

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const glm::vec4& value);

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const glm::mat4x4& value);

		/** Adds an internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param value The value of the internal attribute to be added.
		*/
		void AddInternalAttribute(const std::string& name, const RTTI* value);

		/** Adds an array to the internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param intArray The pointer to the int32_t array to be added.
		*	@size size The size of the array to be added.
		*/
		void AddInternalArrayAttribute(const std::string& name, const std::int32_t* intArray, const std::uint32_t size);

		/** Adds an array to the internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param floatArray The pointer to the float_t array to be added.
		*	@size size The size of the array to be added.
		*/
		void AddInternalArrayAttribute(const std::string& name, const std::float_t* floatArray, const std::uint32_t size);

		/** Adds an array to the internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param stringArray The pointer to the std::string array to be added.
		*	@size size The size of the array to be added.
		*/
		void AddInternalArrayAttribute(const std::string& name, const std::string* stringArray, const std::uint32_t size);

		/** Adds an array to the internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param vec4Array The pointer to the glm::vec4 array to be added.
		*	@size size The size of the array to be added.
		*/
		void AddInternalArrayAttribute(const std::string& name, const glm::vec4* vec4Array, const std::uint32_t size);

		/** Adds an array to the internal attribute and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@param mat4x4Array The pointer to the glm::mat4x4 array to be added.
		*	@size size The size of the array to be added.
		*/
		void AddInternalArrayAttribute(const std::string& name, const glm::mat4x4* mat4x4Array, const std::uint32_t size);

		/** Appends a nested scope and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@return Returns the newly appended scope.
		*/
		Scope& AddNestedScopeAttribute(const std::string& name);

		/** Adopts the passed scope to the current scope and adds the passed name to the prescribed attribute list.
		*	@param childScope The scope to be adopted to the current scope.
		*	@param name The name for the internal attribute to be added.
		*/
		void AddNestedScopeAttribute(Scope& childScope, const std::string& name);

		/** Appends an empty Datum, sets the type of Datum to TABLE and adds the passed name to the prescribed attribute list.
		*	@param name The name for the internal attribute to be added.
		*	@returns Returns the empty Datum whose type is set to TABLE.
		*/
		Datum& AddEmptyNestedScopeAttribute(const std::string& name);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const std::int32_t* pointerToData, const std::uint32_t size);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const std::float_t* pointerToData, const std::uint32_t size);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const std::string* pointerToData, const std::uint32_t size);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const glm::vec4* pointerToData, const std::uint32_t size);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const glm::mat4x4* pointerToData, const std::uint32_t size);

		/** Adds an external attribute and adds the passed name to the prescribed attribute list.
		*	Note: This function internally calls the SetStorage() of Datum.
		*	@param name The name for the external attribute to be added.
		*	@param pointerToData The pointer to the data which will be saved in the current scope.
		*	@param size The size of the external storage.
		*/
		void AddExternalAttribute(const std::string& name, const RTTI** pointerToData, const std::uint32_t size);		
	private:
		/** Appends a Datum with the passed name and set's its type with the passed type.
		*	@param name The name of the Datum which will be appended.
		*	@param datumType The DatumType which will be set to the appended Datum.
		*	@return Returns the appended Datum.
		*/
		Datum& AddAttribute(const std::string& name, const DatumType datumType);

		/** Adds the passed name to the Prescribed attribute list.
		*	@param name The name to be added to the Prescribed attribute list.
		*/
		void AddToPrescribedAtributesList(const std::string& name);

		/** Static Hashmap of uint64_t and Vector<string>. Contains the names of the Prescribed attributes for each class inside the Vector<string>.
		*	uint64_t is the TypeInstanceId() of the class, as Attributed is inherited from RTTI.
		*/
		static Hashmap<uint64_t, Vector<string>> sPrescribedAttributes;

		/** A static integer to keep track of the number of Attributed instances and also the number of instances that inherit from the Attributed class.
		*/
		static uint32_t sAttributedCount;
	public:
		RTTI_DECLARATIONS(Attributed, Scope);
	};
}