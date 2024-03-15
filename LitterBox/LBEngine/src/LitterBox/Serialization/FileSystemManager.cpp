/*!************************************************************************
 \file				FileSystemManager.cpp
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				02/11/2023
 \brief				The FileSystem Manager contains the implementations of
					the FileSystem Manager functions 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Serialization/FileSystemManager.h"
#include "LitterBox/Debugging/Debug.h"

namespace LB
{
	FileSystemManager* FILESYSTEM = nullptr;
	FileSystemManager::FileSystemManager()
	{
		if (!FILESYSTEM) FILESYSTEM = this;
		else std::cerr << "File System Manager already exists" << std::endl;
	}

	/*!************************************************************************
	 * \brief This function gets the file path of the file provided
	 * example input "joe.png" -> C://Users//joe.png
	* \return std::filesystem::path of the filepath for the file
	**************************************************************************/
	std::filesystem::path FileSystemManager::GetFilePath(const std::string& fileName)
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path filePath{ fileName };
		for (const auto& directory : std::filesystem::recursive_directory_iterator(currentPath))
		{
			/*if (std::filesystem::exists(directory / filePath))
			{
				return directory/filePath;
			}*/
			if (directory.path().filename().stem().string() == fileName)
			{
				return directory.path();
			}
		}
		DebuggerLogErrorFormat("Unable to find %s!", fileName.c_str());
		//In the cases where we can't find it, we just return the currentdirector/filename
		//This is so that we can use this function to create files that don't exist yet
		return currentPath/filePath;
	}

	/*!************************************************************************
	 * \brief Function that gets the relative path of the file
	 * 
	 * \param fileName filename to get
	 * \return std::filesystem::path path of the filepath for the file
	**************************************************************************/
	std::filesystem::path FileSystemManager::GetRelativeAssetPath(const std::string& fileName)
	{
		//We get the assets directory, then we save it to the correct filepath
		//User will need to make sure the filepath is complete with the file extn. 
		std::filesystem::path defaultFilepathDirectory{ std::filesystem::current_path() / std::filesystem::path("Assets") };
		std::filesystem::path filePath{ fileName };
		return defaultFilepathDirectory/filePath;
	}


	/*!************************************************************************
	 * \brief Function that gets ALL files of the provided EXTENSION NAME
	 *	e.g ".png" -> vector of all filepaths containing pngs
	 * \return std::vector<std::filesystem::path> of the filepath for the file
	 **************************************************************************/
	std::vector<std::filesystem::path> FileSystemManager::GetFilesOfType(const std::string& fileType)
	{
		std::vector<std::filesystem::path> temp;
		std::filesystem::path currentPath = std::filesystem::current_path();
		for (const auto& directory : std::filesystem::recursive_directory_iterator(currentPath))
		{
			if (directory.path().filename().extension().string() == fileType)
			{
				//std::cout << "Found file : " << directory.path().filename().string() << '\n';
				temp.push_back(directory.path());
			}
		}
		return temp;
	}

	/*!************************************************************************
	 * \brief Function that the file time of the specified file
	 *	example input "joe.png" -> 48568494
	 * \return int File's last time since modified
	 **************************************************************************/
	long long FileSystemManager::GetFileTime(const std::filesystem::path& filePath)
	{
		std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(filePath);
		return (std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch())).count();
	}

	/*!************************************************************************
	 * \brief Function overload for filetime for strings
	 *	example input "joe.png" -> 48568494
	 * \return int File's last time since modified
	 **************************************************************************/
	long long FileSystemManager::GetFileTime(const std::string& filePath)
	{
		std::filesystem::path fp{ filePath };
		return GetFileTime(fp);
	}


	// /*!************************************************************************
	//  * \brief Function to get the stemmed name of a filepath
	//  *	example input "joe.png" -> 48568494
	//  * \return int File's last time since modified
	//  **************************************************************************/
	// std::string FileSystemManager::GetStemmedName(const std::filesystem::path& filePath)
	// {
	// 	return filePath.filename().stem().string();
	// }

	// /*!************************************************************************
	//  * \brief Function overload for filetime for strings
	//  *	example input "joe.png" -> 48568494
	//  * \return int File's last time since modified
	//  **************************************************************************/
	// std::string FileSystemManager::GetExtension(const std::filesystem::path& filePath)
	// {
	// 	return filePath.extension().string();
	// }
}