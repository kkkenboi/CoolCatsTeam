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
	//bool FILESYSTEM->FileExists(const std::string& fileName)
	//{
	//	for (const auto& directory : std::filesystem::recursive_directory_iterator(std::filesystem::current_path()))
	//	{
	//		if (directory.path().filename().string() == fileName)
	//		{
	//			return true;
	//		}
	//	}
	//	return false;
	//}
	int FileSystemManager::GetFileTime(const std::filesystem::path& filePath)
	{
		std::filesystem::file_time_type lastWriteTime = std::filesystem::last_write_time(filePath);
		return std::chrono::duration_cast<std::chrono::seconds>(lastWriteTime.time_since_epoch()).count();
	}
	int FileSystemManager::GetFileTime(const std::string& filePath)
	{
		std::filesystem::path fp{ filePath };
		return GetFileTime(fp);
	}
	std::string FileSystemManager::GetStemmedName(const std::filesystem::path& filePath)
	{
		return filePath.filename().stem().string();
	}
	std::string FileSystemManager::GetExtension(const std::filesystem::path& filePath)
	{
		return filePath.extension().string();
	}
}