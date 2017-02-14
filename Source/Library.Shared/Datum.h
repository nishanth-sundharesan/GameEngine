#pragma once
#include <cstdint>
#include "RTTI.h"
#pragma warning ( push )
#pragma warning ( disable: 4201 )							//Suppressing the warning message "nonstandard extension used : nameless struct / union" in the file"
#include "../../External/Glm/Glm/vec4.hpp"
#include "../../External/Glm/Glm/mat4x4.hpp"
#pragma warning ( pop )

namespace GameEngineLibrary
{
	enum class DatumType
	{
		UNASSIGNED = 0,
		INT32_T = 1,
		FLOAT = 2,
		STRING = 3,
		GLM_VECTOR4 = 4,
		GLM_MATRIX4X4 = 5,
		POINTER = 6
	};

	enum class DatumMemoryType
	{
		UNASSIGNED,
		INTERNAL,
		EXTERNAL
	};

	class Datum final
	{
	public:
		Datum();

		Datum(const Datum& rhs);

#pragma region Overloaded Assignment Operator Declarations
		Datum& operator=(const Datum& rhs);
		Datum& operator=(const std::int32_t& rhs);
		Datum& operator=(const std::float_t& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4x4& rhs);
		Datum& operator=(const RTTI* rhs);
#pragma endregion

		~Datum();

		void SetType(DatumType datumType);

		void SetSize(std::uint32_t size);

		DatumType Type() const;

		std::uint32_t Size() const;
		
		void Clear();

#pragma region SetStorage
		void SetStorage(int32_t* pointerToData, std::uint32_t size);
		void SetStorage(std::float_t* pointerToData, std::uint32_t size);
		void SetStorage(std::string* pointerToData, std::uint32_t size);
		void SetStorage(glm::vec4* pointerToData, std::uint32_t size);
		void SetStorage(glm::mat4x4* pointerToData, std::uint32_t size);
		void SetStorage(RTTI** pointerToData, std::uint32_t size);
#pragma endregion

#pragma region PushBack
		void PushBack(const std::int32_t& value);
		void PushBack(const std::float_t& value);
		void PushBack(const std::string& value);
		void PushBack(const glm::vec4& value);
		void PushBack(const glm::mat4x4& value);
		void PushBack(const RTTI* value);
#pragma endregion

#pragma region Overloaded Equality Operators
		bool operator==(const Datum& rhs)const;
		bool operator==(const std::int32_t& intValue)const;
		bool operator==(const std::float_t& floatValue)const;
		bool operator==(const std::string& stringValue)const;
		bool operator==(const glm::vec4& vector4Value) const;
		bool operator==(const glm::mat4x4& matrix4x4Value) const;
		bool operator==(const RTTI* rttiPointer) const;

#pragma endregion

#pragma region Overloaded InEquality Operators
		bool operator!=(const Datum& rhs)const;
		bool operator!=(const std::int32_t& intValue)const;
		bool operator!=(const std::float_t& floatValue)const;
		bool operator!=(const std::string& stringValue)const;
		bool operator!=(const glm::vec4& vector4Value) const;
		bool operator!=(const glm::mat4x4& matrix4x4Value) const;
		bool operator!=(const RTTI* rttiPointer) const;
#pragma endregion

#pragma region Set Method Declarations
		void Set(const std::int32_t& value, uint32_t index = 0);
		void Set(const std::float_t& value, uint32_t index = 0);
		void Set(const std::string& value, uint32_t index = 0);
		void Set(const glm::vec4& value, uint32_t index = 0);
		void Set(const glm::mat4x4& value, uint32_t index = 0);
		void Set(const RTTI* value, uint32_t index = 0);
#pragma endregion
		
#pragma region Get Method Declarations
		template<typename T>
		T& Get(std::uint32_t index = 0);

		template<typename K>
		const K& Get(std::uint32_t index = 0) const;
#pragma endregion

		std::string ToString(std::uint32_t index = 0) const;
		void SetFromString(std::string& inputString, std::uint32_t index = 0);

		void ShrinkToFit();

	private:
		void Reserve(uint32_t capacity);
		void AssignFunctionalityForEachType();
		uint32_t ReserveStrategy(std::uint32_t capacity);

		template<typename T>
		bool PerformDeepSearch(T *lhs, T *rhs, std::uint32_t size) const;

#pragma region Search based on each data type(Declarations)
		bool PerformVoidSearch(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformIntSearch(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformFloatSearch(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformStringSearch(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformVec4Search(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformMat4x4Search(void* lhs, void* rhs, std::uint32_t size) const;
		bool PerformRTTIPointerSearch(void* lhs, void* rhs, std::uint32_t size) const;
#pragma endregion

#pragma region Pushbacking based on each data type (Declarations)
		void PushBackIntData(std::uint32_t startIndex, std::uint32_t endIndex);
		void PushBackFloatData(std::uint32_t startIndex, std::uint32_t endIndex);
		void PushBackStringData(std::uint32_t startIndex, std::uint32_t endIndex);
		void PushBackVec4Data(std::uint32_t startIndex, std::uint32_t endIndex);
		void PushBackMat4x4Data(std::uint32_t startIndex, std::uint32_t endIndex);
		void PushBackRTTIPointer(std::uint32_t startIndex, std::uint32_t endIndex);
#pragma endregion

#pragma region Destructing based on each data type (Declarations)	
		void DestructIntData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructFloatData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructStringData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructVec4Data(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructMat4x4Data(std::uint32_t startIndex, std::uint32_t endIndex);
#pragma endregion

#pragma region Converting each data type to std::string(Declarations)
		std::string ToStringUnassigned(std::uint32_t index) const;
		std::string ToStringInt(std::uint32_t index) const;
		std::string ToStringFloat(std::uint32_t index) const;
		std::string ToStringString(std::uint32_t index) const;
		std::string ToStringVec4(std::uint32_t index) const;
		std::string ToStringMat4x4(std::uint32_t index) const;
		std::string ToStringRTTIPointer(std::uint32_t index) const;
#pragma endregion
		
		union DatumValues
		{
			void* voidPointer;
			std::int32_t* intPointer;
			std::float_t* floatPointer;
			std::string* stringPointer;
			glm::vec4* vector4Pointer;
			glm::mat4x4* mat4x4Pointer;
			RTTI** rttiPointer;
		};

		std::uint32_t mSize;
		std::uint32_t mCapacity;
		DatumType mDatumType;
		DatumMemoryType mMemoryType;
		DatumValues mDatumValues;

		typedef void (Datum::*DestructorsForDataType)(std::uint32_t startIndex, std::uint32_t endIndex);
		typedef void (Datum::*PushBackForDataType)(std::uint32_t startIndex, std::uint32_t endIndex);
		typedef bool (Datum::*PerformSearchForDataType)(void* lhs, void* rhs, std::uint32_t size) const;
		typedef std::string(Datum::*ToStringForDataType)(std::uint32_t index) const;

		uint32_t mDataTypeSizes[7];
		DestructorsForDataType mDestructors[7];
		PushBackForDataType mPushBacks[7];
		PerformSearchForDataType mPerformSearch[7];
		ToStringForDataType mToString[7];
	};

	template<typename T>
	bool Datum::PerformDeepSearch(T* lhs, T* rhs, std::uint32_t size) const
	{
		for (std::uint32_t i = 0; i < size; ++i)
		{
			if (*(lhs + i) != *(rhs + i))
			{
				return false;
			}
		}
		return true;
	}
}