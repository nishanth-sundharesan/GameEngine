#pragma once
#include <cstdint>
#include "RTTI.h"
//#include "Glm/vec4.hpp"

namespace GameEngineLibrary
{
	enum class DatumType
	{
		INT32_T,
		FLOAT,
		STRING,
		VECTOR,
		MATRIX,
		RTTI,
		UNASSIGNED
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

		void SetStorage(int32_t* pointerToData, uint32_t size);
		void SetStorage(float* pointerToData, uint32_t size);
		void SetStorage(std::string* pointerToData, uint32_t size);

		bool operator==(const Datum& rhs)const;
		bool operator==(const int32_t& intValue)const;
		bool operator==(const float& floatValue)const;
		bool operator==(const std::string& stringValue)const;

		void Set(int32_t value, uint32_t index = 0);
		void Set(float value, uint32_t index = 0);
		void Set(std::string value, uint32_t index = 0);


		template<typename T>
		T Get(uint32_t index = 0) const;

		template<>
		int32_t Get<int32_t>(uint32_t index) const;

		template<>
		float Get<float>(uint32_t index) const;
	private:
		void Reserve(std::uint32_t capacity);

		template<typename T>
		bool PerformDeepSearch(T *lhs, T *rhs, uint32_t size) const;

		/*template<>
		bool Datum::PerformDeepSearch<float>(float* lhs, float *rhs, uint32_t size) const;

		template<>
		bool Datum::PerformDeepSearch<int32_t>(int32_t* lhs, int32_t *rhs, uint32_t size) const;*/

		/*bool PerformDeepSearch(int32_t *lhs, int32_t *rhs, uint32_t size) const;
		bool PerformDeepSearch(float *lhs, float *rhs, uint32_t size) const;
		bool PerformDeepSearch(std::string *lhs, std::string *rhs, uint32_t size) const;*/
	private:
		union DatumValues
		{
			void *voidPointer;
			std::int32_t *intPointer;
			float *floatPointer;
			std::string *stringPointer;
			RTTI ** rttiPointer;
		};

		bool isInternalStorage;
		std::uint32_t mSize;
		std::uint32_t mCapacity;
		DatumValues mDatumValues;
		DatumType mDatumType;
	};

	template<typename T>
	bool Datum::PerformDeepSearch(T* lhs, T *rhs, uint32_t size) const
	{
		for (uint32_t i = 0; i < size; ++i)
		{
			if (*(lhs + i) != *(rhs + i))
			{
				return false;
			}
		}
		return true;
	}

}