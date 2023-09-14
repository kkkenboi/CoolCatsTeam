#pragma once
#include "rapidjson.h"
#include "document.h"
#include "writer.h"
#include "stringbuffer.h"
#include "stream.h"
#include "prettywriter.h"
#include "filereadstream.h"
#include "filewritestream.h"
#include <string>
#include <fstream>

using namespace rapidjson;

namespace Litterbox
{
	class JSONSerializer
	{
	public:
		/// @brief Function to get a json file at the specified filepath
		/// @param filePath filepath the json file is at
		/// @return Returns the data type used to manipulate and serialise/deserialise json data
		Document GetJSONFile(const std::string& filePath)
		{
			Document _jsonFile;
			std::ifstream inputFile(filePath);
			if (!inputFile) {}//throw some error maybe
			std::string jsonString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
			inputFile.close();
			if (_jsonFile.Parse(jsonString.c_str()).HasParseError()) {}
			return _jsonFile;	//this should contain the parsed information
		}
		void SaveToJSON(const std::string& filePath, const Document& jsonFileToSave)
		{
			std::ofstream outputFile(filePath);
			StringBuffer buffer;
			PrettyWriter<StringBuffer> jsonWriter(buffer);
			jsonWriter.SetMaxDecimalPlaces(2); //sets the max dp to 2. tweak this for higher precision!
			jsonFileToSave.Accept(jsonWriter);
			std::string outputString = buffer.GetString();
			outputFile << outputString;
			outputFile.close();
		}
	};
}