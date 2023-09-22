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
		/// @brief Writes data to specified .json filename
		/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
		/// @param fileName Name of the JSON FILE
		/// @param typeToSerialize Type of object that is being serialized
		/// @return true on success, false on fail
		template<typename T>
		bool SerializeToFile(std::string fileName, T& typeToSerialize)
		{
			//We need to load the json data from the json file
			jsonFile = GetJSONFile(fileName);
			//Once it has the data, it needs to allocate memory with the allocator
			Document::AllocatorType& allocator = jsonFile.GetAllocator();
			//then we pray to god the T has a serialize function
			if (typeToSerialize.Serialize(jsonFile, allocator))
			{
				//then we save it to file
				SaveToJSON(fileName, jsonFile);
				return true;
			}
			return false;
		}
		/// @brief Loads data from specified .json filename
		/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
		/// @param fileName Name of the JSON FILE
		/// @param typeToDeserialize Out param for your data. 
		template<typename T>
		void DeserializeFromFile(std::string fileName, T& typeToDeserialize)
		{
			//Get the file, then deserialize! magic
			jsonFile = GetJSONFile(fileName);
			typeToDeserialize.Deserialize(jsonFile);
		}
	private:
		/// @brief json object to hold all the json data
		Document jsonFile;
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
		/// @brief helper function to write to file FROM json object
		/// @param filePath filepath the json file is at
		/// @param jsonFileToSave json object that holds the data to be saved
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