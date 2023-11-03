/*!************************************************************************
 \file				EditorAssets.h
 \author(s)			Ang Jiawei Jarrett, Amadeus Chia Jinhan
 \par DP email(s):	a.jiaweijarrett@digipen.edu, amadeusjinhan.chia@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This header file contains functions declarations for the assets layer of the
 Editor. This is to allow the programmer to browse through the assets to be used
 within the game/scene view.

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

		/*!***********************************************************************
		  \brief
		  Constructor for the EditorAssets class. Also sets the default file dir
		  \return
		  Nothing.
		*************************************************************************/
		EditorAssets(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Initializes the EditorAssets layer. (Subscribes the callback function)
		  \return
		  Nothing.
		*************************************************************************/
		void Initialize() override;

		/*!***********************************************************************
		  \brief
		  Updates the EditorAssets layer. Handles the bulk of the content browser 
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorAssets layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() {}

		std::filesystem::path currentDirectory;			//keeps track of the current directory we're in
		std::filesystem::path defaultDirectory;			//default directory 
	private:
		std::string folderPathName;
		std::filesystem::path relativePath;			//relative pathname of the current directory
	};
	extern EditorAssets* EDITORASSETS;

	/*!***********************************************************************
	  \brief
	  Callback function for handling dropped files when files are dropped 
	  onto the application window.
	  \return
	  Nothing.
	*************************************************************************/
	void drop_callback(GLFWwindow* window, int count, const char** paths);


}
