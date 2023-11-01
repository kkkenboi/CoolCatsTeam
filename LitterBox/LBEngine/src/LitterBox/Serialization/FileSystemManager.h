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
		FileSystemManager();
		std::filesystem::path GetFilePath(const std::string& fileName);
		std::vector<std::filesystem::path> GetFilesOfType(const std::string& fileType);
		//static bool FileExists(const std::string& fileName);
		int GetFileTime(const std::filesystem::path& filePath);
		int GetFileTime(const std::string& filePath);

		std::string GetStemmedName(const std::filesystem::path& filePath);
		std::string GetExtension(const std::filesystem::path& filePath);
	};
	extern FileSystemManager* FILESYSTEM;
}