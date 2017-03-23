#pragma once
#include "IXmlParseHelper.h"
#include "Scope.h"

namespace GameEngineLibrary
{
	class XmlParseHelperTable : public IXmlParseHelper
	{
		typedef XmlParseMaster::SharedData SharedData;

	public:
		class SharedDataTable :public SharedData
		{
			//TODO check constructors & destructors
		public:
			SharedDataTable();

			SharedDataTable(const SharedDataTable&) = delete;

			SharedDataTable& operator=(const SharedDataTable&) = delete;

			virtual void Initialize() override;

			virtual SharedData* Clone() const override;

			Scope* GetScope();

			const Scope* GetScope() const;

			void AppendNewScope(const std::string& name);

			void GotoParentScope();

			~SharedDataTable();

		protected:
			virtual void CloneInternalMembers(SharedData* sharedData) const override;

		private:
			Scope* mScope;

			RTTI_DECLARATIONS(SharedDataTable, SharedData);
		};

		XmlParseHelperTable(XmlParseMaster& xmlParseMaster);

		XmlParseHelperTable(const XmlParseHelperTable&) = delete;

		XmlParseHelperTable& operator=(const XmlParseHelperTable&) = delete;

		virtual void Initialize() override;
		
		virtual bool StartElementHandler(SharedData& sharedData, const std::string& name, const Hashmap<std::string, std::string>& attributes) override;

		virtual bool EndElementHandler(SharedData& sharedData, const std::string& name) override;

		virtual void CharacterDataHandler(SharedData& sharedData, const std::string& value, const std::int32_t length, bool isCompleteData) override;

		virtual IXmlParseHelper* Clone() override;

		~XmlParseHelperTable() = default;
	private:
		static const std::string mXmlElementName;

		static const std::string mXmlElementAttributeName;
	};
}