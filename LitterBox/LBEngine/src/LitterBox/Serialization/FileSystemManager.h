#pragma once
#include <filesystem>
#include <string>
namespace LB
{
	class FileSystemManager
	{
	public:
		static std::filesystem::path GetFilePath(const std::string& fileName);

	};
}