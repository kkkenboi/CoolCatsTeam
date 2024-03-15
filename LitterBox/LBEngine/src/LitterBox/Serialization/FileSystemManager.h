/*!************************************************************************
 \file				FileSystemManager.h
 \author(s)			Amadeus Chia Jinhan, 
 \par DP email(s):	amadeusjinhan.chia@digipen.edu,
 \par Course:       CSD2401A
 \date				02/11/2023
 \brief				The FileSystem Manager contains the wrapper functions
					for some of the more commonly used functions in 
					std::filesystem. 

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include <filesystem>
#include <string>
#include <vector>
#include <chrono>
namespace LB
{
	class FileSystemManager
	{
	public:
		/*!************************************************************************
		* \brief Construct a new Asset Manager:: Asset Manager object
		* 
		**************************************************************************/
		FileSystemManager();


		/*!************************************************************************
		* \brief This function gets the file path of the file provided
		* example input "joe.png" -> C://Users//joe.png
		* \return std::filesystem::path of the filepath for the file
		**************************************************************************/
		std::filesystem::path GetFilePath(const std::string& fileName);

		/*!************************************************************************
		 * \brief Get the Relative Asset Path object	
		 * 
		 * \param fileName Filename to get
		 * \return std::filesystem::path path of the filepath for the file
		**************************************************************************/
		std::filesystem::path GetRelativeAssetPath(const std::string& fileName);

		/*!************************************************************************
		* \brief Function that gets ALL files of the provided EXTENSION NAME
		*	e.g ".png" -> vector of all filepaths containing pngs
		* \return std::vector<std::filesystem::path> of the filepath for the file
		**************************************************************************/
		std::vector<std::filesystem::path> GetFilesOfType(const std::string& fileType);

		/*!************************************************************************
		* \brief Function that the file time of the specified file
		*	example input "joe.png" -> 48568494
		* \return int File's last time since modified
		**************************************************************************/		
		long long GetFileTime(const std::filesystem::path& filePath);

		/*!************************************************************************
		* \brief Function overload for filetime for strings
		*	example input "joe.png" -> 48568494
		* \return int File's last time since modified
		**************************************************************************/
		long long GetFileTime(const std::string& filePath);

		// std::string GetStemmedName(const std::filesystem::path& filePath);
		// std::string GetExtension(const std::filesystem::path& filePath);
	};
	extern FileSystemManager* FILESYSTEM;
}