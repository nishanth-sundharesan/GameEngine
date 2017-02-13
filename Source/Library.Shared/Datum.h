#pragma once
#include <cstdint>
#include "RTTI.h"
#pragma warning ( push )
#pragma warning ( disable: 4201 )							//Suppressing the warning message "nonstandard extension used : nameless struct / union" in the file"
#include "../../External/Glm/Glm/vec4.hpp"
#include "../../External/Glm/Glm/mat4x4.hpp"
#pragma warning ( pop )
#include "DefaultReserveStrategy.h"

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

	class Datum
	{
	public:
		Datum();

		Datum(const Datum& rhs);

#pragma region Overloaded Assignment Operator Declarations
		Datum& operator=(const Datum& rhs);
		Datum& operator=(const std::int32_t& rhs);
		Datum& operator=(const float& rhs);
		Datum& operator=(const std::string& rhs);
		Datum& operator=(const glm::vec4& rhs);
		Datum& operator=(const glm::mat4x4& rhs);
#pragma endregion

		~Datum();

		std::uint32_t Size() const;

		DatumType Type() const;

		void SetType(DatumType datumType);

		void SetSize(std::uint32_t size);

		void Clear();

#pragma region SetStorage
		void SetStorage(int32_t* pointerToData, std::uint32_t size);
		void SetStorage(float* pointerToData, std::uint32_t size);
		void SetStorage(std::string* pointerToData, std::uint32_t size);
		void SetStorage(glm::vec4* pointerToData, std::uint32_t size);
		void SetStorage(glm::mat4x4* pointerToData, std::uint32_t size);
#pragma endregion

#pragma region PushBack
		void PushBack(const std::int32_t& value);
		void PushBack(const float& value);
		void PushBack(const std::string& value);
		void PushBack(const glm::vec4& value);
		void PushBack(const glm::mat4x4& value);
#pragma endregion

#pragma region Overloaded Equality Operators
		bool operator==(const Datum& rhs)const;
		bool operator==(const std::int32_t& intValue)const;
		bool operator==(const float& floatValue)const;
		bool operator==(const std::string& stringValue)const;
		bool operator==(const glm::vec4& vector4Value) const;
		bool operator==(const glm::mat4x4& matrix4x4Value) const;
#pragma endregion

#pragma region Overloaded InEquality Operators
		bool operator!=(const Datum& rhs)const;
		bool operator!=(const std::int32_t& intValue)const;
		bool operator!=(const float& floatValue)const;
		bool operator!=(const std::string& stringValue)const;
		bool operator!=(const glm::vec4& vector4Value) const;
		bool operator!=(const glm::mat4x4& matrix4x4Value) const;
#pragma endregion

#pragma region Set Method Declarations
		void Set(const std::int32_t& value, uint32_t index = 0);
		void Set(const float& value, uint32_t index = 0);
		void Set(const std::string& value, uint32_t index = 0);
		void Set(const glm::vec4& value, uint32_t index = 0);
		void Set(const glm::mat4x4& value, uint32_t index = 0);
#pragma endregion

		void ShrinkToFit();

#pragma region Get Method Declarations
		template<typename T>
		T& Get(std::uint32_t index = 0);
		template<> std::int32_t& Get(std::uint32_t index);
		template<> float& Get(std::uint32_t index);
		template<> std::string& Get(std::uint32_t index);
		template<> glm::vec4& Get(std::uint32_t index);
		template<> glm::mat4x4& Get(std::uint32_t index);

		template<typename T>
		const T& Get(std::uint32_t index = 0) const;
		template<> const std::int32_t& Get(std::uint32_t index) const;
		template<> const float& Get<float>(std::uint32_t index) const;
		template<> const std::string& Get(std::uint32_t index) const;
		template<> const glm::vec4& Get(std::uint32_t index) const;
		template<> const glm::mat4x4& Get(std::uint32_t index) const;
#pragma endregion

		std::string ToString(std::uint32_t index = 0) const;
		void SetFromString(std::string inputString, std::uint32_t index = 0) const;

	private:
#pragma region Reserve Method Declarations
		void Reserve(uint32_t capacity);
		/*void Reserve(std::uint32_t capacity, std::int32_t* intPointer);
		void Reserve(std::uint32_t capacity, float* floatPointer);
		void Reserve(std::uint32_t capacity, std::string* stringPointer);
		void Reserve(std::uint32_t capacity, glm::vec4* vector4Pointer);
		void Reserve(std::uint32_t capacity, glm::mat4x4* mat4x4Pointer);*/
#pragma endregion

		template<typename T>
		bool PerformDeepSearch(T *lhs, T *rhs, std::uint32_t size) const;

#pragma region Pushbacking based on each data type (Declarations)
		void PushBackIntData();
		void PushBackFloatData();
		void PushBackStringData();
		void PushBackVec4Data();
		void PushBackMat4x4Data();
#pragma endregion

#pragma region Destructing based on each data type (Declarations)
		void DestructIntData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructFloatData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructStringData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructVec4Data(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructMat4x4Data(std::uint32_t startIndex, std::uint32_t endIndex);
#pragma endregion

		void AssignFunctionalityForEachType();

		union DatumValues
		{
			void* voidPointer;
			std::int32_t* intPointer;
			float* floatPointer;
			std::string* stringPointer;
			glm::vec4* vector4Pointer;
			glm::mat4x4* mat4x4Pointer;
			RTTI** rttiPointer;
		};

		//Private members
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		DatumType mDatumType;
		DatumMemoryType mMemoryType;
		DatumValues mDatumValues;
		DefaultReserveStrategy reserveStrategy;

		typedef void (Datum::*DestructorsForDataType)(std::uint32_t startIndex, std::uint32_t endIndex);
		typedef void (Datum::*PushBackForDataType)();

		const uint32_t mNumberOfTypesSupported = 7;
		uint32_t mDataTypeSizes[7];
		DestructorsForDataType mDestructors[7];
		PushBackForDataType mPushBacks[7];
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