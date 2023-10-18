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

using namespace rapidjson;	//makes it easy to use rapidjson stuff

namespace LB
{
	/*!***********************************************************************
	 * \brief Enum class that allows us to tag file destinations to int values
	 * 	(Makes it easier and simpler to store in JSON)
	 *************************************************************************/
	enum class FILEDESTINATION
	{
		NONE = 0,
		EDITOR = 1,
		SCENES,
		PREFAB,
		SCRIPTS,
		ART,
		AUDIO,
		FONT,
		LOGS,
		APPDATA,
		//SHADER
		//BUILDS
	};

	/*!***********************************************************************
	 * \brief 
	 * Class responsible for reading/writing data to JSON file
	 * (Useer does not need to include the .json file extension)
	 *************************************************************************/
	class JSONSerializer
	{
	public:
		/*!***********************************************************************
		 * \brief Construct a new JSONSerializer object
		 * 
		 *************************************************************************/
		JSONSerializer()
		{
			//We instantiate none to be an empty string
			fileDestinationMap[FILEDESTINATION::NONE];
			Document _jsonFile = GetJSONFile("Editor/Jason/filepaths.json");
			//Document::AllocatorType& allocator = _jsonFile.GetAllocator();
			//Avert your gaze! somehow rapidjson only takes in const char*
			//Can't use strings or any other thing for some reason ;__;
			if (_jsonFile.IsObject())
			{
				//The idea here is that we literally just check if the filepath exists
				//The format in the json should be something like
				//"EDITOR" : "/editor/File/Path"
				//Note that in the future, FindMember should be used instead...
				if (_jsonFile.HasMember("EDITOR"))
				{
					filepathNames["EDITOR"] = _jsonFile["EDITOR"].GetString();
					fileDestinationMap[FILEDESTINATION::EDITOR] = _jsonFile["EDITOR"].GetString();
				}
				if (_jsonFile.HasMember("PREFABS"))
				{
					filepathNames["PREFABS"] = _jsonFile["PREFABS"].GetString();
					fileDestinationMap[FILEDESTINATION::PREFAB] = _jsonFile["PREFABS"].GetString();

				}
				if (_jsonFile.HasMember("SCRIPTS"))
				{
					filepathNames["SCRIPTS"] = _jsonFile["SCRIPTS"].GetString();
					fileDestinationMap[FILEDESTINATION::SCRIPTS] = _jsonFile["SCRIPTS"].GetString();

				}
				if (_jsonFile.HasMember("ART"))
				{
					filepathNames["ART"] = _jsonFile["ART"].GetString();
					fileDestinationMap[FILEDESTINATION::ART] = _jsonFile["ART"].GetString();

				}
				if (_jsonFile.HasMember("AUDIO"))
				{
					filepathNames["AUDIO"] = _jsonFile["AUDIO"].GetString();
					fileDestinationMap[FILEDESTINATION::AUDIO] = _jsonFile["AUDIO"].GetString();

				}
				if (_jsonFile.HasMember("FONT"))
				{
					filepathNames["FONT"] = _jsonFile["FONT"].GetString();
					fileDestinationMap[FILEDESTINATION::FONT] = _jsonFile["FONT"].GetString();

				}
				if (_jsonFile.HasMember("LOGS"))
				{
					filepathNames["LOGS"] = _jsonFile["LOGS"].GetString();
					fileDestinationMap[FILEDESTINATION::LOGS] = _jsonFile["LOGS"].GetString();

				}
				if (_jsonFile.HasMember("APPDATA"))
				{
					filepathNames["APPDATA"] = _jsonFile["APPDATA"].GetString();
					fileDestinationMap[FILEDESTINATION::APPDATA] = _jsonFile["APPDATA"].GetString();
				}
				//In the future we will loop through this to assign file paths to their enums
				//for (auto elem : fileDestinationMap)
				//{
				//	//Just to double check that we stored the filepaths to the map correctly
				//	//DebuggerLog(elem.second);
				//}
			}
		}
		
		/*!***********************************************************************
		 * \brief helper function is just to help generate the filepaths.json file
		 * For moments when somehow the filepath file is missing and we need to regenerate
		 *************************************************************************/
		void HelperFunction()
		{
			Document _jsonFile = GetJSONFile("Editor/Jason/filepaths");
			Document::AllocatorType& allocator = _jsonFile.GetAllocator();
			_jsonFile.SetObject();
			_jsonFile.AddMember("EDITOR", "/editor", allocator);
			_jsonFile.AddMember("PREFABS", "/prefabs", allocator);
			_jsonFile.AddMember("SCRIPTS", "/scripts", allocator);
			_jsonFile.AddMember("ART", "/art", allocator);
			_jsonFile.AddMember("AUDIO", "/audio", allocator);
			_jsonFile.AddMember("FONT", "/font", allocator);
			_jsonFile.AddMember("LOGS", "/logs", allocator);
			_jsonFile.AddMember("APPDATA", "%APPDATA%/", allocator);
			_jsonFile.AddMember("SCENES", "/scenes", allocator);
			SaveToJSON("filepaths.json", _jsonFile);
		}

		/*!***********************************************************************
		 * \brief 
		 * This function should be inside the asset manager in the future
		 * Currently, it "converts" the enum to string 
		 *************************************************************************/
		std::string FileEnumToString(FILEDESTINATION fileEnum)
		{
			switch (fileEnum)
			{
			case FILEDESTINATION::EDITOR:
				return "EDITOR";
			case FILEDESTINATION::SCENES:
				return "SCENES";
			case FILEDESTINATION::PREFAB:
				return "PREFABS";
			case FILEDESTINATION::SCRIPTS:
				return "SCRIPTS";
			case FILEDESTINATION::ART:
				return "ART";
			case FILEDESTINATION::AUDIO:
				return "AUDIO";
			case FILEDESTINATION::FONT:
				return "FONT";
			case FILEDESTINATION::LOGS:
				return "LOGS";
			case FILEDESTINATION::APPDATA:
				return "APPDATA";
			default:
				//DebuggerLogWarning("FILEDESTINATION DOES NOT EXIST!");
				return "";
			}
		}

		/*!***********************************************************************
		 * \brief 
		 * Grabs the json file specified (user does not need to add .json to filename)
		 * and then serializes data into it
		 * If there isn't one, it creates a new jsonfile and then serializes data into it
		 *************************************************************************/
		template<typename T>
		bool SerializeToFile(const std::string& fileName, T& typeToSerialize, FILEDESTINATION fileDestination = FILEDESTINATION::NONE)
		{
			/// @brief Writes data to specified .json filename
			/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
			/// @param fileName Name of the JSON FILE
			/// @param typeToSerialize Type of object that is being serialized
			/// @return true on success, false on fail
			//We need to load the json data from the json file
			jsonFile = GetJSONFile(fileName);
			//Once it has the data, it needs to allocate memory with the allocator
			Document::AllocatorType& allocator = jsonFile.GetAllocator();
			//then we pray to god the T has a serialize function
			if (typeToSerialize.Serialize(jsonFile, allocator))
			{
				//then we save it to file
				SaveToJSON(fileDestinationMap[fileDestination] + fileName + ".json", jsonFile);
				return true;
			}
			return false;
		}

		/*!***********************************************************************
		 * \brief 
		 *  Grabs the json file specified (user does not need to add .json to filename)
		 * and then deserializes it into the type.
		 *************************************************************************/
		template<typename T>
		void DeserializeFromFile(const std::string& fileName, T& typeToDeserialize,FILEDESTINATION filePath = FILEDESTINATION::NONE)
		{
			/// @brief Loads data from specified .json filename
			/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
			/// @param fileName Name of the JSON FILE
			/// @param typeToDeserialize Out param for your data. 
			//Get the file, then deserialize! magic
			std::string fullFilePath{ fileDestinationMap[filePath] + fileName + ".json" };
			DebuggerLog("Getting file from : " + fullFilePath);
			//std::cout << "Joe: " << fileDestinationMap[filePath] + fileName + ".json\n";
			jsonFile = GetJSONFile(fileDestinationMap[filePath] + fileName + ".json");
			typeToDeserialize.Deserialize(jsonFile);
		}

		/*!***********************************************************************
		 * \brief 
		 * Helper function to grab json data from file specified.
		 *************************************************************************/
		Document GetJSONFile(const std::string& filePath)
		{
			/// @brief Function to get a json file at the specified filepath
			/// @param filePath filepath the json file is at
			/// @return Returns the data type used to manipulate and serialise/deserialise json data
			Document _jsonFile;
			std::ifstream inputFile(filePath);
			//Assert if it's NOT open
			DebuggerAssert(inputFile.is_open(), std::string{filePath + " not found!"});
			std::string jsonString((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
			inputFile.close();
			if (_jsonFile.Parse(jsonString.c_str()).HasParseError()) { }
			return _jsonFile;	//this should contain the parsed information
		}

		/*!***********************************************************************
		 * \brief 
		 * Helper function to save json data into the specified json file
		 *************************************************************************/
		void SaveToJSON(const std::string& filePath, const Document& jsonFileToSave)
		{
			/// @brief helper function to write to file FROM json object
			/// @param filePath filepath the json file is at
			/// @param jsonFileToSave json object that holds the data to be saved
			std::ofstream outputFile(filePath);
			DebuggerAssert(outputFile.is_open(), std::string{ filePath + " not found!" });
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
		 * This map is to get the file destination enum from the string
		 * e.g 0 : /editorFilePath/
		 *************************************************************************/
		std::map<FILEDESTINATION, std::string> fileDestinationMap;

		/*!***********************************************************************
		 * \brief 
		 * This map is to map the file name to file path
		 * e.g "EDITOR" : "/editorFilePath/"
		 *************************************************************************/
		std::unordered_map<std::string, std::string> filepathNames;


		/*!***********************************************************************
		 * \brief 
		 * json object to hold all the json data
		 *************************************************************************/
		Document jsonFile;
		
		
	};
}