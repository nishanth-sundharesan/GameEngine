#pragma once
#include <expat.h>
#include "IXmlParseHelper.h"
#include "Vector.h"

namespace GameEngineLibrary
{
	class XmlParseMaster
	{
	public:
		class SharedData
		{
		public:
			SharedData();

			SharedData* Clone();

			void SetXmlParseMaster();

			XmlParseMaster* GetXmlParseMaster();

			void IncrementDepth();

			void DecrementDepth();

			int32_t Depth();
		private:
			XmlParseMaster* xmlParseMaster;

			int32_t depth;
		};

	public:
		XmlParseMaster(SharedData* sharedData);

		~XmlParseMaster();

		XmlParseMaster* Clone();

		void AddHelper(IXmlParseHelper* xmlParseHelper);

		bool RemoveHelper(IXmlParseHelper* xmlParseHelper);

		void Parse(char* rawXmlData, uint32_t length, bool isLastXmlChunkData);

		void ParseFromFile(char* fileName);

		char* GetFileName();

		SharedData* GetSharedData();

		void SetSharedData(SharedData* sharedData);
	private:

		static void StartElementHandler(void *userData, const char *name, const char **atts);

		static void EndElementHandler(void *userData, const char *name);

		static void CharDataHandler();
	private:
		XML_Parser mParser;

		char* mCurrentParsingFile;

		SharedData* mSharedData;

		Vector<IXmlParseHelper*> mXmlParseHelpers;
	};
}