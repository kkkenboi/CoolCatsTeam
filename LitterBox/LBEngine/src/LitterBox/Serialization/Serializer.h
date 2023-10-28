/*!************************************************************************
 \file				Serializer.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains the implementation for Serializing and
					Deserializing data to JSON.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
//CRUCIAL FOR RAPID JSON
#include "../../dependencies/RapidJSON/include/rapidjson.h" 
#include "../../dependencies/RapidJSON/include/document.h"

//READ AND WRITING HEADERS FOR JSON
#pragma warning(push, 0)
#include "../../dependencies/RapidJSON/include/writer.h"
#pragma warning(pop)
#include "../../dependencies/RapidJSON/include/stringbuffer.h"
#include "../../dependencies/RapidJSON/include/stream.h"
#include "../../dependencies/RapidJSON/include/prettywriter.h"
#include "../../dependencies/RapidJSON/include/filereadstream.h"
#include "../../dependencies/RapidJSON/include/filewritestream.h"

//UTILITY HEADERS FOR FILE IO
#include <string>
#include <fstream>
#include <unordered_map> //used to store the filepath maps
#include "LitterBox/Debugging/Debug.h"	//used to log any errors
#include "LitterBox/Serialization/FileSystemManager.h"
using namespace rapidjson;	//makes it easy to use rapidjson stuff

namespace LB
{

	/*!***********************************************************************
	 * \brief 
	 * Class responsible for reading/writing data to JSON file
	 * (Useer does not need to include the .json file extension)
	 *************************************************************************/
	class JSONSerializer
	{
	public:
		
		
		/*!***********************************************************************
		 * \brief 
		 * Grabs the json file specified (user does not need to add .json to filename)
		 * and then serializes data into it
		 * If there isn't one, it creates a new jsonfile and then serializes data into it
		 *************************************************************************/
		template<typename T>
		static bool SerializeToFile(const std::string& fileName, T& typeToSerialize)
		{
			/// @brief Writes data to specified .json filename
			/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
			/// @param fileName Name of the JSON FILE
			/// @param typeToSerialize Type of object that is being serialized
			/// @return true on success, false on fail
			//We need to load the json data from the json file
			Document jsonFile = GetJSONFile(FileSystemManager::GetFilePath(fileName + ".json").string());
			//Once it has the data, it needs to allocate memory with the allocator
			Document::AllocatorType& allocator = jsonFile.GetAllocator();
			//then we pray to god the T has a serialize function
			if (typeToSerialize.Serialize(jsonFile, allocator))
			{
				//then we save it to file
				SaveToJSON(FileSystemManager::GetFilePath(fileName).string(), jsonFile);
				return true;
			}

			return false;
		}

		/*!***********************************************************************
		 * \brief 
		 *  Grabs the json file specified (User doesn't need to type the whole path,, or .json, just filename)
		 * and then deserializes it into the type.
		 *************************************************************************/
		template<typename T>
		static void DeserializeFromFile(const std::string& fileName, T& typeToDeserialize)
		{
			/// @brief Loads data from specified .json filename
			/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
			/// @param fileName Name of the JSON FILE
			/// @param typeToDeserialize Out param for your data. 
			//Get the file, then deserialize! magic
			DebuggerLog("Getting file from : " + fileName);
			//std::cout << "Joe: " << fileDestinationMap[filePath] + fileName + ".json\n";
			Document jsonFile = GetJSONFile(FileSystemManager::GetFilePath(fileName+".json").string());
			typeToDeserialize.Deserialize(jsonFile);
		}

		/*!***********************************************************************
		 * \brief 
		 * Helper function to grab json data from file specified.
		 * (ONLY WORKS WITH ABSOLUTE FILE PATHS!! DOES NOT USE FILE MANAGER SYSTEM!)
		 *************************************************************************/
		static Document GetJSONFile(const std::string& filePath)
		{
			/// @brief Function to get a json file at the specified filepath
			/// @param filePath filepath the json file is at
			/// @return Returns the data type used to manipulate and serialise/deserialise json data
			Document _jsonFile;
			std::ifstream inputFile(filePath);
			//Assert if it's NOT open
			//DebuggerAssert(inputFile.is_open(), std::string{filePath + " not found!"});
			std::string jsonString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
			inputFile.close();
			if (_jsonFile.Parse(jsonString.c_str()).HasParseError()) { }
			return _jsonFile;	//this should contain the parsed information
		}

		/*!***********************************************************************
		 * \brief 
		 * Helper function to save json data into the specified json file
		 * (ONLY WORKS WITH ABSOLUTE FILE PATHS!! DOES NOT USE FILE MANAGER SYSTEM!)
		 *************************************************************************/
		static void SaveToJSON(const std::string& filePath, const Document& jsonFileToSave)
		{
			/// @brief helper function to write to file FROM json object
			/// @param filePath filepath the json file is at
			/// @param jsonFileToSave json object that holds the data to be saved
			std::ofstream outputFile(filePath);
			DebuggerAssert(outputFile.is_open(), std::string{ filePath + " not found while trying to save!" });
			StringBuffer buffer;
			PrettyWriter<StringBuffer> jsonWriter(buffer);
			jsonWriter.SetMaxDecimalPlaces(2); //sets the max dp to 2. tweak this for higher precision!
			jsonFileToSave.Accept(jsonWriter);
			std::string outputString = buffer.GetString();
			outputFile << outputString;
			outputFile.close();
		}
	private:

		/*!***********************************************************************
		 * \brief 
		 * json object to hold all the json data
		 *************************************************************************/
		//static Document jsonFile;
		
		
	};
}