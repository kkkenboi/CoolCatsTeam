/*!************************************************************************
 \file				EditorAssets.cpp
 \author(s)			
 \par DP email(s):	
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorAssets.h"

namespace LB
{
	EditorAssets::EditorAssets(std::string layerName) : Layer(layerName) 
	{
		std::filesystem::path AssetsPath{ "Assets" };
		currentDirectory = std::filesystem::current_path() / AssetsPath;
		defaultDirectory = currentDirectory;
	}

	void EditorAssets::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());

		if (currentDirectory != defaultDirectory)
		{
			if (ImGui::Button("Back"))
			{
				currentDirectory = currentDirectory.parent_path();
			}
		}
		for (auto& directory : std::filesystem::directory_iterator(currentDirectory))
		{
			std::string filename = directory.path().filename().string();
			if (directory.is_directory())
			{
				std::string pathName = directory.path().string();
				if (ImGui::Button(filename.c_str()))
				{
					currentDirectory /= directory.path().filename();
				}
			}
			else
			{
				std::string stemFileName = directory.path().filename().stem().string();
				/*if (ImGui::Button(stemFileName.c_str()))
				{

				}*/
				if (ImGui::Button(filename.c_str()))
				{

				}
			}
		}

		ImGui::End();
	}
}
