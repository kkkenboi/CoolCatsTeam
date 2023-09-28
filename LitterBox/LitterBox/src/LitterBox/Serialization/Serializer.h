#pragma once
//CRUCIAL FOR RAPID JSON
#include "../../dependencies/RapidJSON/include/rapidjson.h" 
#include "../../dependencies/RapidJSON/include/document.h"

//READ AND WRITING HEADERS FOR JSON
#include "../../dependencies/RapidJSON/include/writer.h"
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

	
	class JSONSerializer
	{
	public:

		JSONSerializer()
		{
			//We instantiate none to be an empty string
			fileDestinationMap[FILEDESTINATION::NONE];
			Document _jsonFile = GetJSONFile("filepaths.json");
			Document::AllocatorType& allocator = _jsonFile.GetAllocator();
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
				for (auto elem : fileDestinationMap)
				{
					//Just to double check that we stored the filepaths to the map correctly
					//DebuggerLog(elem.second);
				}
			}
		}
		
		//This helper function is just to help generate the filepaths.json file
		//For moments when somehow the filepath file is missing and we need to regenerate
		void HelperFunction()
		{
			Document _jsonFile = GetJSONFile("filepaths");
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
	
		//This function should be inside the asset manager in the future
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
		/// @brief Writes data to specified .json filename
		/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
		/// @param fileName Name of the JSON FILE
		/// @param typeToSerialize Type of object that is being serialized
		/// @return true on success, false on fail
		template<typename T>
		bool SerializeToFile(const std::string& fileName, T& typeToSerialize, FILEDESTINATION fileDestination = FILEDESTINATION::NONE)
		{
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
		/// @brief Loads data from specified .json filename
		/// @tparam T HAS TO BE A TYPE THAT IS SERIALIZABLE!!!
		/// @param fileName Name of the JSON FILE
		/// @param typeToDeserialize Out param for your data. 
		template<typename T>
		void DeserializeFromFile(const std::string& fileName, T& typeToDeserialize,FILEDESTINATION filePath = FILEDESTINATION::NONE)
		{
			//Get the file, then deserialize! magic
			std::string fullFilePath{ fileDestinationMap[filePath] + fileName + ".json" };
			DebuggerLog("Getting file from : " + fullFilePath);
			//std::cout << "Joe: " << fileDestinationMap[filePath] + fileName + ".json\n";
			jsonFile = GetJSONFile(fileDestinationMap[filePath] + fileName + ".json");
			typeToDeserialize.Deserialize(jsonFile);
		}
	private:
		//This map is to get the file destination enum from the string
		//e.g 0 : /editorFilePath/
		std::map<FILEDESTINATION, std::string> fileDestinationMap;
		//This map is to map the file name to file path
		//e.g "EDITOR" : "/editorFilePath/"
		std::unordered_map<std::string, std::string> filepathNames;
		/// @brief json object to hold all the json data
		Document jsonFile;
		/// @brief Function to get a json file at the specified filepath
		/// @param filePath filepath the json file is at
		/// @return Returns the data type used to manipulate and serialise/deserialise json data
		Document GetJSONFile(const std::string& filePath)
		{
			Document _jsonFile;
			std::ifstream inputFile(filePath);
			//Assert if it's NOT open
			//DebuggerAssert(inputFile.is_open(), std::string{filePath + " not found!"});
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