#pragma once
#include <cstdint>
#include "RTTI.h"
#include "../../External/Glm/Glm/vec4.hpp"
#include "../../External/Glm/Glm/mat4x4.hpp"
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
		void PushBack(std::int32_t& value);
		void PushBack(float& value);
		void PushBack(std::string& value);
		void PushBack(glm::vec4& value);
		void PushBack(glm::mat4x4& value);
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
		void Set(std::int32_t& value, uint32_t index = 0);
		void Set(float& value, uint32_t index = 0);
		void Set(std::string& value, uint32_t index = 0);
		void Set(glm::vec4& value, uint32_t index = 0);
		void Set(glm::mat4x4& value, uint32_t index = 0);
#pragma endregion

		void ShrinkToFit();

		template<typename T>
		T Get(std::uint32_t index = 0) const;

		template<> std::int32_t Get(std::uint32_t index) const;
		template<> float Get<float>(std::uint32_t index) const;
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

		/*bool PerformDeepSearch(int32_t *lhs, int32_t *rhs, uint32_t size) const;
		bool PerformDeepSearch(float *lhs, float *rhs, uint32_t size) const;
		bool PerformDeepSearch(std::string *lhs, std::string *rhs, uint32_t size) const;*/
	private:
		/*template<class T>
		class TestClass
		{
		public:
			static T test;
		};*/



		void DestructIntData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructFloatData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructStringData(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructVec4Data(std::uint32_t startIndex, std::uint32_t endIndex);
		void DestructMat4x4Data(std::uint32_t startIndex, std::uint32_t endIndex);

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

		/*bool isInternalStorage;*/
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		DatumMemoryType mMemoryType;
		DatumValues mDatumValues;
		DatumType mDatumType;
		DefaultReserveStrategy reserveStrategy;

		typedef void (Datum::*DestructorsForDataType)(std::uint32_t startIndex, std::uint32_t endIndex);

		const uint32_t mNumberOfTypesSupported = 7;
		uint32_t mDataTypeSizes[7];
		DestructorsForDataType mDestructors[7];
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