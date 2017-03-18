#pragma once
#include <expat.h>
#include "IXmlParseHelper.h"
#include "Vector.h"
#include "RTTI.h"

namespace GameEngineLibrary
{
	class XmlParseMaster final
	{
	public:
		class SharedData :public RTTI
		{
		public:
			SharedData();

			SharedData(const SharedData&) = delete;

			SharedData& operator=(const SharedData&) = delete;

			virtual SharedData* Clone() const;

			void SetXmlParseMaster(XmlParseMaster* xmlParseMaster);

			XmlParseMaster* GetXmlParseMaster();

			const XmlParseMaster* GetXmlParseMaster() const;

			void IncrementDepth();

			void DecrementDepth();

			int32_t Depth() const;

			virtual ~SharedData() = default;
		private:
			XmlParseMaster* mXmlParseMaster;

			int32_t mDepth;
		public:
			RTTI_DECLARATIONS(SharedData, RTTI);
		};

	public:
		XmlParseMaster(SharedData* sharedData);

		XmlParseMaster(const XmlParseMaster&) = delete;

		XmlParseMaster& operator=(const XmlParseMaster&) = delete;

		~XmlParseMaster();

		XmlParseMaster* Clone() const;

		void AddHelper(IXmlParseHelper& xmlParseHelper);

		bool RemoveHelper(IXmlParseHelper& xmlParseHelper);

		void Parse(const char* rawXmlData, const uint32_t length, const bool isLastXmlChunkData);

		void ParseFromFile(const std::string& fileName);

		string GetFileName() const;

		SharedData* GetSharedData();

		const SharedData* GetSharedData() const;

		void SetSharedData(SharedData* sharedData);
	private:
		static void StartElementHandler(void *userData, const char *name, const char **attributes);

		static void EndElementHandler(void *userData, const char *name);

		static void CharacterDataHandler(void *userData, const char *value, std::int32_t length);
	private:
		XML_Parser mParser;

		string mCurrentParsingFile;

		SharedData* mSharedData;

		IXmlParseHelper* currentXmlParseHelper;

		Vector<IXmlParseHelper*> mXmlParseHelpers;
	};
}