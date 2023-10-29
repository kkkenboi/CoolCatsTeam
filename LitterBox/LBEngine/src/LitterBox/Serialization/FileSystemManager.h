#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace LB
{
	class FileSystemManager
	{
	public:
		static std::filesystem::path GetFilePath(const std::string& fileName);
		static std::vector<std::filesystem::path> GetFilesOfType(const std::string& fileType);
		static bool FileExists(const std::string& fileName);
	};
}