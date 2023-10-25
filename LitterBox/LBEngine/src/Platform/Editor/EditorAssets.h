/*!************************************************************************
 \file				EditorAssets.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "Platform/Editor/Editor.h"
#include "Litterbox/Engine/Layer.h"
#include <filesystem>
#include <string>
namespace LB
{
	class EditorAssets : public Layer
	{
	public:
		EditorAssets(std::string layerName);

		void UpdateLayer() override;
	private:
		std::filesystem::path defaultDirectory;			//default directory 
		std::filesystem::path currentDirectory;			//keeps track of the current directory we're in
		std::string folderPathName;
		std::filesystem::path relativePath;			//relative pathname of the current directory
	};
}
