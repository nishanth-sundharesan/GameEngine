#pragma once
#include "Sector.h"

namespace GameEngineLibrary
{
	class World final :public Attributed
	{
	public:
		//TODO Can be made const std::string&
		std::string Name();

		void SetName(const std::string& name);

		Datum& Sectors();

		const Datum& Sectors() const;

		Sector& CreateSector(const std::string& instanceName);

		void Update();

	private:
		std::string mName;

		Datum tempDatum;

		Sector tempSector;
	};
}