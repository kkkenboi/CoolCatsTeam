#include "LitterBox/Serialization/FileSystemManager.h"
#include "LitterBox/Debugging/Debug.h"

namespace LB
{
	std::filesystem::path FileSystemManager::GetFilePath(const std::string& fileName)
	{
		std::filesystem::path currentPath = std::filesystem::current_path();
		std::filesystem::path filePath{ fileName };
		for (const auto& directory : std::filesystem::recursive_directory_iterator(currentPath))
		{
			if (std::filesystem::exists(directory / filePath))
			{
				return directory/filePath;
			}
		}
		DebuggerLogErrorFormat("Unable to find %s!", fileName.c_str());
		//In the cases where we can't find it, we just return the currentdirector/filename
		//This is so that we can use this function to create files that don't exist yet
		return currentPath/filePath;
	}

	////Function to get all filepaths of a certain filetype and return it as a vector
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
	bool FileSystemManager::FileExists(const std::string& fileName)
	{
		for (const auto& directory : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
		{
			if (directory.path().filename().string() == fileName)
			{
				return true;
			}
		}
		return false;
	}
}