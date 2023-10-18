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
		DebuggerLogErrorFormat("Unable to find %s!", fileName);
		return std::filesystem::path(fileName);
	}
}