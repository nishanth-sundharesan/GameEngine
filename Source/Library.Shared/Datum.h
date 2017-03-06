#pragma once
#include "RTTI.h"
#pragma warning ( push )
#pragma warning ( disable: 4201 )							//Suppressing the warning message "nonstandard extension used : nameless struct / union" in the file"
#include <Glm/vec4.hpp>
#include <Glm/mat4x4.hpp>
#pragma warning ( pop )

namespace GameEngineLibrary
{
	/** A union to represent the type of data Datum is holding
	*/
	enum class DatumType
	{
		UNASSIGNED = 0,
		INT32_T = 1,
		FLOAT = 2,
		STRING = 3,
		GLM_VECTOR4 = 4,
		GLM_MATRIX4X4 = 5,
		POINTER = 6,
		TABLE = 7
	};

	/** Forward declaring the class Scope.
	*/
	class Scope;

	class Datum final
	{
	public:
		/** Zero parameterized constructor. Initializes the private members of the class.
		*/
		Datum();

		/** Copy constructor.
		*	Performs a deep copy of the Datum object.
		*	@param rhs The object of which the deep copy will be made.
		*/
		Datum(const Datum& rhs);

		/** Move constructor.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are returning a temporary object to a permanent object from a function. Example: Creating a stack allocated object from a function and returning it.(Gets called on the return statement)
		*	@param rhs The temporary right hand side object which has to be moved.
		*/
		Datum(Datum&& rhs);
		
#pragma region Overloaded Assignment Operator Declarations
		/** Overloaded assignment operator.
		*	Performs a deep copy of the right hand side object: Datum.
		*	@param rhs It is the right hand side object which will be deep copied to the left hand side object.
		*	@return Returns the deep copied Datum object
		*/
		Datum& operator=(const Datum& rhs);

		/** Move assignment operator.
		*	Moves the temporary object's(right hand side) pointers/primitive data to the left hand side object. Assigns default type to all the members of the class.
		*	Note: This function is called when we are assigning a temporary object to a permanent object. Example: Assigning a stack allocated object from a function which created the object.(Gets called on the assignment statement)
		*/
		Datum& operator=(Datum&& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs value and sets the DatumType to INT32_T if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side value which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const std::int32_t& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs value and sets the DatumType to FLOAT if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side value which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const std::float_t& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs value and sets the DatumType to STRING if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side value which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const std::string& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs value and sets the DatumType to GLM_VECTOR4 if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side value which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const glm::vec4& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs value and sets the DatumType to GLM_MATRIX4X4 if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side value which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const glm::mat4x4& rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs pointer and sets the DatumType to POINTER if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side pointer which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const RTTI* const rhs);

		/** Overloaded assignment operator.
		*	Push backs the rhs pointer and sets the DatumType to TABLE if the DatumType is UNASSIGNED or else replaces the first element of the Datum Object.
		*	@param rhs It is the right hand side pointer which will be Pushed back or Set at the 0th index of the Datum object.
		*	@return Returns the Datum object after Push back or after Set.
		*/
		Datum& operator=(const Scope* const rhs);
#pragma endregion
		/** Clears the entire Datum object.
		*/
		~Datum();

		/** Sets the DatumType of the Datum object.
		*	Note: This has to be called before Push back and it can be called only once.
		*	@params datumType The DatumType of the Datum object.
		*	@exception throws an exception if we trying to assign a different DatumType to the Datum object.
		*/
		void SetType(const DatumType datumType);

		/** Sets the size of the Datum Object.
		*	It reserves memory in advance if the passed size is greater than the capacity.
		*	It instantiates data(with default constructor) inside the Datum if the passed size is greater than its size but less than its capacity.
		*	It truncates the values if the passed size if less than its size.
		*	@params size The size to be assigned to the Datum object.
		*	@exception throws an exception if the Datum stores external data or if the DatumType is UNASSIGNED.
		*/
		void SetSize(const std::uint32_t size);

		/** Returns the DatumType of the Datum object.
		*	@returns Returns the DatumType of the Datum object.
		*/
		DatumType Type() const;

		/** Returns the size of the Datum object.
		*	@return Returns the size of the Datum object.
		*/
		std::uint32_t Size() const;

		/** Clears the entire Datum object.
		*	Note: This function doesn't reset/free the capacity of the Datum object.
		*	@see ShrinkToFit();
		*/
		void Clear();

#pragma region SetStorage
		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is invalid.
		*/
		void SetStorage(const int32_t* const pointerToData, const std::uint32_t size);

		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is mismatched.
		*/
		void SetStorage(const std::float_t* const pointerToData, const std::uint32_t size);

		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is invalid.
		*/
		void SetStorage(const std::string* const pointerToData, const std::uint32_t size);

		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is invalid.
		*/
		void SetStorage(const glm::vec4* const pointerToData, const std::uint32_t size);

		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is invalid.
		*/
		void SetStorage(const glm::mat4x4* const pointerToData, const std::uint32_t size);

		/** Sets the storage of the Datum Object.
		*	Marks the Datum object as EXTERNAL and assigns the pointer and the size.
		*	Note: This is only function which can make the Datum object hold external memory.
		*	@params pointerToData The pointer to the external data.
		*	@params	size The size of the external data.
		*	@exception throws an exception if the Datum object is Internal or if the DatumType is invalid.
		*/
		void SetStorage(const RTTI** const pointerToData, const std::uint32_t size);
#pragma endregion

#pragma region PushBack
		/** Appends/Push backs the data to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The data to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const std::int32_t& value);

		/** Appends/Push backs the data to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The data to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const std::float_t& value);

		/** Appends/Push backs the data to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The data to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const std::string& value);

		/** Appends/Push backs the data to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The data to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const glm::vec4& value);

		/** Appends/Push backs the data to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The data to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const glm::mat4x4& value);

		/** Appends/Push backs the pointer to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The pointer to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const RTTI* const value);

		/** Appends/Push backs the pointer to the Datum object.
		*	Note: PushBacks work only for the INTERNAL Datum types.
		*	@param value The pointer to be appended to the Datum object.
		*	@exception throws an exception if the Datum object holds external data or if the DatumType is invalid.
		*/
		void PushBack(const Scope* const value);
#pragma endregion

#pragma region Remove
		/** Removes the passed scope pointer from the Datum object.
		*	@param value The scope pointer that has to be removed from the Datum object.
		*	@return Returns true of the Remove() was successful, false otherwise.
		*/
		bool Remove(const Scope* const value);
#pragma endregion

#pragma region Overloaded Equality Operators
		/** Overloaded Equality operator.
		*	Checks if both the Datum objects contain the same values.
		*	Note: This function doesn't check for the equality of its internal reserved capacity.
		*	@params rhs The right hand side Datum object to be compared with.
		*	@returns Returns true if both the Datum objects contain the same data, false otherwise.
		*/
		bool operator==(const Datum& rhs) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the passed value.
		*	@params intValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the passed value, false otherwise.
		*/
		bool operator==(const std::int32_t& intValue) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the passed value.
		*	@params floatValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the passed value, false otherwise.
		*/
		bool operator==(const std::float_t& floatValue) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the passed value.
		*	@params stringValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the passed value, false otherwise.
		*/
		bool operator==(const std::string& stringValue) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the passed value.
		*	@params vector4Value The value to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the passed value, false otherwise.
		*/
		bool operator==(const glm::vec4& vector4Value) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the passed value.
		*	@params matrix4x4Value The value to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the passed value, false otherwise.
		*/
		bool operator==(const glm::mat4x4& matrix4x4Value) const;

		/** Overloaded Equality operator.
		*	Checks if the Datum is a scalar(size 1) and contains the content of the passed pointer.
		*	@params rttiPointer The content of the pointer to be compared with the Datum.
		*	@returns Returns true if the Datum contains a single value and it is equal to the content of the passed pointer, false otherwise.
		*/
		bool operator==(const RTTI* const rttiPointer) const;

#pragma endregion

#pragma region Overloaded InEquality Operators
		/** Overloaded Inequality operator.
		*	Checks if both the Datum objects do not contain the same values.
		*	Note: This function doesn't check for the inequality of its internal reserved capacity.
		*	@params rhs The right hand side Datum object to be compared with.
		*	@returns Returns true if both the Datum objects do not contain the same data, false otherwise.
		*/
		bool operator!=(const Datum& rhs) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the passed value.
		*	@params intValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the passed value, false otherwise.
		*/
		bool operator!=(const std::int32_t& intValue) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the passed value.
		*	@params floatValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the passed value, false otherwise.
		*/
		bool operator!=(const std::float_t& floatValue) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the passed value.
		*	@params stringValue The value to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the passed value, false otherwise.
		*/
		bool operator!=(const std::string& stringValue) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the passed value.
		*	@params vector4Value The value to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the passed value, false otherwise.
		*/
		bool operator!=(const glm::vec4& vector4Value) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the passed value.
		*	@params matrix4x4Value The value to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the passed value, false otherwise.
		*/
		bool operator!=(const glm::mat4x4& matrix4x4Value) const;

		/** Overloaded Inequality operator.
		*	Checks if the Datum is not a scalar(size 1) or doesn't contain the content of the passed pointer.
		*	@params rttiPointer The content of the pointer to be compared with the Datum.
		*	@returns Returns true if the Datum is not a scalar or is not equal to the content of the passed pointer, false otherwise.
		*/
		bool operator!=(const RTTI* const rttiPointer) const;
#pragma endregion

#pragma region Set Method Declarations
		/** Sets the corresponding value at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The data to be set at the specified index.
		*	@param index The index for the value.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const std::int32_t& value, const uint32_t index = 0);

		/** Sets the corresponding value at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The data to be set at the specified index.
		*	@param index The index for the value.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const std::float_t& value, const uint32_t index = 0);

		/** Sets the corresponding value at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The data to be set at the specified index.
		*	@param index The index for the value.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const std::string& value, const uint32_t index = 0);

		/** Sets the corresponding value at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The data to be set at the specified index.
		*	@param index The index for the value.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const glm::vec4& value, const uint32_t index = 0);

		/** Sets the corresponding value at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The data to be set at the specified index.
		*	@param index The index for the value.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const glm::mat4x4& value, const uint32_t index = 0);

		/** Sets the corresponding pointer at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The pointer to the data which has to be stored.
		*	@param index The index for the pointer.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const RTTI* const value, const uint32_t index = 0);

		/** Sets the corresponding pointer at the corresponding index.
		*	Note: This function doesn't resize the Datum. It only replaces the values.
		*	@param value The pointer to the data which has to be stored.
		*	@param index The index for the pointer.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		void Set(const Scope* const value, const uint32_t index = 0);
#pragma endregion

#pragma region Get Method Declarations
		/** Template Specialized Get function.
		*	Returns the data from the Datum object at the specified index.
		*	@param index The index of the data to be returned (defaulted to 0).
		*	@returns Returns the data from the Datum object at the specified index.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		template<typename T>
		T& Get(const std::uint32_t index = 0);

		/** Template Specialized Get function.
		*	Returns the data from the Datum object at the specified index.
		*	@param index The index of the data to be returned (defaulted to 0).
		*	@returns Returns the data from the Datum object at the specified index.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		template<typename K>
		const K& Get(const std::uint32_t index = 0) const;
#pragma endregion

		/** Returns the std::string representation of the data at the specified index.
		*	@param index The index of the data which has to be converted to std::string and returned (defaulted to 0).
		*	@returns Returns the std::string representation of the data at the specified index.
		*	@exception throws an exception if the index is out of range or if the DatumType is UNASSIGNED.
		*/
		std::string ToString(const std::uint32_t index = 0) const;

		/** Converts the inputString to the appropriate type and sets it at the specified index.
		*	@param inputString The input string which has to be converted to the appropriate type.
		*	@param index The index of the Datum object where the converted data has to be placed.
		*	@exception throws an exception if the index is out of range or if the DatumType is UNASSIGNED or POINTER.
		*/
		void SetFromString(const std::string& inputString, const std::uint32_t index = 0);

		/** Shrinks the capacity of the Datum object to the Size of the Datum object.
		*	Note: This function doesn't affect the size of the Datum object.
		*/
		void ShrinkToFit();

		/** Returns the Scope reference from the Datum object at the specified index.
		*	Note: This function internally calls the Get<Scope*>().
		*	@param index The index of the data to be returned.
		*	@returns Returns the Scope reference from the Datum object at the specified index.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		Scope& operator[](const std::uint32_t index);

		/** Returns the Scope reference from the Datum object at the specified index.
		*	Note: This function internally calls the Get<Scope*>().
		*	@param index The index of the data to be returned.
		*	@returns Returns the Scope reference from the Datum object at the specified index.
		*	@exception throws an exception if the index is out of range or if the DatumType is invalid.
		*/
		const Scope& operator[](const std::uint32_t index) const;

	private:
		/** A union to represent the memory type if the Datum.
		*/
		enum class DatumMemoryType
		{
			UNASSIGNED,
			INTERNAL,
			EXTERNAL
		};

		/** This helper function checks if the passed memoryType is the same as the Datum's memory type. If yes then it throws an exception.
		*	@param memoryType The memory type which has to be checked.
		*	@exception throws an exception if the passed memoryType matches with the Datum's memory type.
		*/
		void CheckForTheMemoryType(const DatumMemoryType memoryType) const;

		/** Checks if the passed index is in bounds of the size of the Datum
		*	@param index The index which has to be checked for bounds
		*	@exception throws an exception if the index is out of bounds/range.
		*/
		void CheckForBounds(const std::uint32_t index) const;

		/** This helper function checks if the passed datumType is the same type as that of the Datum object.
		*	@param datumType The datum type which has to be checked.
		*	@exception throws an exception if the specified datumType is not the same type as that of the Datum object.
		*/
		void CheckForDatumType(const DatumType datumType) const;

		/** This helper function checks if the passed datumType is the same type as that of the Datum object and it also checks the bounds of the Datum.
		*	Note: This function internally calls the functions CheckForBounds() and CheckForDatumType();
		*	@param datumType The datum type which has to be checked.
		*	@param index The index which has to be checked for bounds
		*	@exception throws an exception if the index is out of bounds/range.
		*	@exception throws an exception if the specified datumType is not the same type as that of the Datum object.
		*/
		void CheckForBoundsAndDatumType(const DatumType datumType, const std::uint32_t index) const;

		/** Reserves memory for the Datum object.
		*	Reserves the memory for the param "capacity" number of objects in the Datum Object.
		*	Note: For the actual reserve to happen, the specified capacity has to be greater than the existing capacity.
		*	@param capacity The new capacity of the Datum object. This should be greater than the current capacity.
		*/
		void Reserve(const std::uint32_t capacity);

		/** This functions assigns all the function pointers to the function pointer array.
		*/
		void AssignFunctionalityForEachType();

		/** The Reserve Strategy for reserving new capacity.
		*	@param capacity The existing capacity of the Datum object.
		*	@returns Returns the new capacity for the Reserve.
		*/
		std::uint32_t ReserveStrategy(const std::uint32_t capacity) const;

		/** Templated function which checks for the equality of the contents of the two passed pointers.
		*	@param lhs The left hand side pointer pointing to the data.
		*	@param rhs The right hand side pointer pointing to the data.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		template<typename T>
		bool PerformDeepCompare(const T* const lhs, const T* const rhs) const;

#pragma region Pushbacking based on each data type (Declarations)
		/** This function appends the default std::int32_t() to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackIntData(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default std::float_t() to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackFloatData(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default std::string() to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackStringData(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default glm::vec4() to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackVec4Data(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default glm::mat4x4() to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackMat4x4Data(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default null pointer to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackRTTIPointer(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function appends the default null pointer to the Datum object.
		*	Note: The function push backs endIndex - startIndex times. It is named that way to keep consistency with the Destructor functions.
		*	@param startIndex The lower hand value.
		*	@param endIndex The higher hand value.
		*/
		void PushBackScopePointer(const std::uint32_t startIndex, const std::uint32_t endIndex);
#pragma endregion

#pragma region Destructing based on each data type (Declarations)
		/** This function calls the destructor for the all the primitive types (std::int32_t, std::float_t, glm::vec4, glm::mat4x4).
		*	@param startIndex The starting index from where the destructor will be called.
		*	@param endIndex The ending index(exclusive) until which the destructor will be called.
		*/
		void DestructPrimitiveTypes(const std::uint32_t startIndex, const std::uint32_t endIndex);

		/** This function calls the destructor for the type std::string from startIndex to endIndex.
		*	@param startIndex The starting index from where the destructor will be called.
		*	@param endIndex The ending index(exclusive) until which the destructor will be called.
		*/
		void DestructStringData(const std::uint32_t startIndex, const std::uint32_t endIndex);
#pragma endregion

#pragma region Converting each data type to std::string(Declarations)
		/** This function is called from ToString() when the DatumType is UNASSIGNED.
		*	This function throws an exception whenever it is called.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*/
		void ToStringUnassigned(const std::uint32_t index, std::string& convertedString) const;

		/** This function converts the data(std::int32_t) present at the specified index to std::string and returns it.
		*   This function is called from ToString() when the DatumType is INT32_T.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*	@returns Returns the string representation of the int32_t value.
		*/
		void ToStringInt(const std::uint32_t index, std::string& convertedString) const;

		/** This function converts the data(std::float_t) present at the specified index to std::string and returns it.
		*   This function is called from ToString() when the DatumType is FLOAT.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*	@returns Returns the string representation of the float_t value.
		*/
		void ToStringFloat(const std::uint32_t index, std::string& convertedString) const;

		/** This function returns the std::string data present at the specified index.
		*   This function is called from ToString() when the DatumType is STRING.
		*	@param index The index of the data inside the Datum object which has to be returned.
		*	@returns Returns the std::string data present at the specified index.
		*/
		void ToStringString(const std::uint32_t index, std::string& convertedString) const;

		/** This function converts the data(glm::vec4) present at the specified index to std::string and returns it.
		*   This function is called from ToString() when the DatumType is GLM_VECTOR4.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*	@returns Returns the string representation of the glm::vec4 value.
		*/
		void ToStringVec4(const std::uint32_t index, std::string& convertedString) const;

		/** This function converts the data(glm::mat4x4) present at the specified index to std::string and returns it.
		*   This function is called from ToString() when the DatumType is GLM_MATRIX4X4.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*	@returns Returns the string representation of the glm::mat4x4 value.
		*/
		void ToStringMat4x4(const std::uint32_t index, std::string& convertedString) const;

		/** This function converts the data present at the specified index to std::string and returns it. It internally calls the ToString() of RTTI class.
		*   This function is called from ToString() when the DatumType is POINTER.
		*	@param index The index of the data inside the Datum object which has to be converted to std::string.
		*	@returns Returns the string representation of the content of the pointer.
		*/
		void ToStringRTTIPointer(const std::uint32_t index, std::string& convertedString) const;
#pragma endregion

#pragma region Set from std::string to specific data type Declarations
		/** Throws an exception when trying to set from string to an UNASSIGNED Datum.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the data has to be set in the Datum.
		*/
		void SetFromStringUnassigned(const std::string& inputString, const std::uint32_t index);

		/** Converts std::string to std::int32_t and sets it at the specified index.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringInt(const std::string& inputString, const std::uint32_t index);

		/** Converts std::string to std::float_t and sets it at the specified index.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringFloat(const std::string& inputString, const std::uint32_t index);

		/** Sets the inputString at the specified index.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be set.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringString(const std::string& inputString, const std::uint32_t index);

		/** Converts std::string to glm::vec4 and sets it at the specified index.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringVec4(const std::string& inputString, const std::uint32_t index);

		/** Converts std::string to glm::mat4x4 and sets it at the specified index.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringMat4x4(const std::string& inputString, const std::uint32_t index);

		/** Throws an exception when trying to set from string to a POINTER Datum.
		*	Note: This function gets called from SetFromString()
		*	@param inputString The input string which has to be converted.
		*	@param index The index where the date has to be set in the Datum.
		*/
		void SetFromStringRTTIPointer(const std::string& inputString, const std::uint32_t index);

#pragma endregion		

		/** The Union which holds the pointers to various supported data types.
		*/
		union DatumValues
		{
			void* voidPointer;
			std::int32_t* intPointer;
			std::float_t* floatPointer;
			std::string* stringPointer;
			glm::vec4* vector4Pointer;
			glm::mat4x4* mat4x4Pointer;
			Scope** tablePointer;
			RTTI** rttiPointer;
		};

#pragma region Search based on each data type(Declarations)
		/** This function will be called when two Datums of type UNASSIGNED are compared.
		*	This function will always return true.
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Always return true.
		*/
		bool PerformVoidDeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type INT32_T are compared.
		*	This function will internally call the templated PerformDeepCompare();
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformIntDeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type FLOAT are compared.
		*	This function will internally call the templated PerformDeepCompare();
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformFloatDeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type STRING are compared.
		*	This function will internally call the templated PerformDeepCompare();
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformStringDeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type GLM_VECTOR4 are compared.
		*	This function will internally call the templated PerformDeepCompare();
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformVec4DeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type GLM_MATRIX4X4 are compared.
		*	This function will internally call the templated PerformDeepCompare();
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformMat4x4DeepCompare(const DatumValues& rhsDatumValues) const;

		/** This function will be called when two Datums of type POINTER are compared.
		*	This function will internally call the .Equals() of the RTTI class.
		*	@param rhsDatumValues The right hand side DatumValue which will be compared and checked for equality.
		*	@returns Returns true if both the pointers contain the same data.
		*/
		bool PerformRTTIPointerDeepCompare(const DatumValues& rhsDatumValues) const;
#pragma endregion

#pragma region Perform deep copy for each type Implementations
		/** Performs deep copy for the unassigned type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyUnassigned(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the std::int32_t type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyInt(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the std::float_t type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyFloat(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the std::string type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyString(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the glm::vec4 type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyVec4(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the glm::mat4x4 type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyMat4x4(const DatumValues& rhsDatumValues, const std::uint32_t size);

		/** Performs deep copy for the POINTER type.
		*	@param rhsDatumValues The right hand side DatumValue which will be copied.
		*	@param mSize The length of the data which has to be copied
		*/
		void PerformDeepCopyRTTIPointer(const DatumValues& rhsDatumValues, const std::uint32_t size);
#pragma endregion

		/** The size of the Datum object.
		*/
		std::uint32_t mSize;

		/** The capacity of the Datum object.
		*/
		std::uint32_t mCapacity;

		/** The type of the Datum object.
		*/
		DatumType mDatumType;

		/** The memory type of the Datum object.
		*/
		DatumMemoryType mMemoryType;

		/** The instance of the union which holds the pointers to various supported data types.
		*/
		DatumValues mDatumValues;

		typedef void (Datum::*DestructorsForDataType)(const std::uint32_t startIndex, const std::uint32_t endIndex);
		typedef void (Datum::*PushBackForDataType)(const std::uint32_t startIndex, const std::uint32_t endIndex);
		typedef bool (Datum::*PerformDeepCompareForDataType)(const DatumValues& rhsDatumValues) const;
		typedef void (Datum::*ToStringForDataType)(const std::uint32_t index, std::string& convertedString) const;
		typedef void (Datum::*SetFromStringForDataType)(const std::string& inputString, const std::uint32_t index);
		typedef void (Datum::*PerformDeepCopyForDataType)(const DatumValues& rhsDatumValues, const std::uint32_t mSize);

		enum class NumberOfDatumTypes
		{
			SIZE = 8
		};

		/** An unsigned integer array containing the sizes of the various supported data types.
		*/
		uint32_t mDataTypeSizes[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to the destructor functions of each supported data type.
		*/
		DestructorsForDataType mDestructors[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to the push back functions of each supported data type.
		*/
		PushBackForDataType mPushBacks[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to the search functions of each supported data type.
		*/
		PerformDeepCompareForDataType mPerformDeepCompare[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to the ToString functions of each supported data type.
		*/
		ToStringForDataType mToString[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to the Set from string functions which convert each specific data to std::string.
		*/
		SetFromStringForDataType mSetFromString[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];

		/** Function pointer array containing pointers to functions which perform deep copy based on their specific type.
		*/
		PerformDeepCopyForDataType mPerformDeepCopy[static_cast<uint32_t>(NumberOfDatumTypes::SIZE)];
	};

	template<typename T>
	bool Datum::PerformDeepCompare(const T* const lhs, const T* const rhs) const
	{
		for (std::uint32_t i = 0; i < mSize; ++i)
		{
			if (*(lhs + i) != *(rhs + i))
			{
				return false;
			}
		}
		return true;
	}
}