/*!************************************************************************
 \file				EditorAssets.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				16/10/23
 \brief

 This source file contains functions definitions for the assets layer of the
 Editor. This is to allow the programmer to browse through the assets to be used
 within the game/scene view.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorAssets.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{
	EditorAssets* EDITORASSETS{ nullptr };
	//forward declaration

	/*!***********************************************************************
	  \brief
	  Callback function for handling dropped files when files are dropped
	  onto the application window.
	  \return
	  Nothing.
	*************************************************************************/
	void drop_callback(GLFWwindow* window, int count, const char** paths)
	{
		int i;
		for (i = 0; i < count; i++)
		{
			std::cout << std::filesystem::path{ paths[i] } << '\n';
			std::cout << EDITORASSETS->currentDirectory << '\n';
			try
			{
				//fs::copy_file("sandbox/abc", "sandbox/def");
				std::filesystem::path fileToCopy{ paths[i] };
				std::filesystem::copy_file(fileToCopy, EDITORASSETS->currentDirectory/fileToCopy.filename());
				ReimportAssets();
				//WINDOWSSYSTEM->OnApplicationFocus.Invoke();
			}
			catch (std::filesystem::filesystem_error& e)
			{
				std::cout << "Could not copy " << paths[i] << " " << e.what() << '\n';
			}
		}
	}

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorAssets class.
	  \return
	  Nothing.
	*************************************************************************/
	EditorAssets::EditorAssets(std::string layerName) : Layer(layerName)
	{
		if (!EDITORASSETS)
			EDITORASSETS = this;
		else
			DebuggerLogError("Editor Asset already exist!");


		std::filesystem::path assetPath{ "Assets" };
		currentDirectory = std::filesystem::current_path() / assetPath;
		defaultDirectory = currentDirectory;
		folderPathName = currentDirectory.filename().string();

	}

	/*!***********************************************************************
	  \brief
	  Initializes the EditorAssets layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorAssets::Initialize()
	{
		glfwSetDropCallback(WINDOWSSYSTEM->GetWindow(), drop_callback);

	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorAssets layer.
	  \return
	  Nothing.
	*************************************************************************/
	void EditorAssets::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());
		float panelWidth = ImGui::GetContentRegionAvail().x;
		ImGui::Text(folderPathName.c_str());	//this puts the text for the filepath "Assets/Textures/fhksjfh"
		ImGui::SameLine(0,69.f);
		ImGui::Text("Files");
		ImGui::Columns(2, "Folders", true);		//We set 2 columns
		ImGui::SetColumnWidth(0, 100.f);		//and make the width fixed at 100
		//Then we loop through all the folders in the current directory
		for (auto& directory : std::filesystem::directory_iterator(currentDirectory))
		{
			//We want the filenames for the button
			std::string filename = directory.path().filename().string();
			if (directory.is_directory())	//if it's a folder we wanna show it. Files will be dealt with later (in next column)
			{
				//Relative path needs the FULL path and the current path and it'll "minus" them off, giving us what we want
				relativePath = std::filesystem::relative(directory.path(), std::filesystem::current_path());
				if (ImGui::Button(filename.c_str())) //the button name will be the folder names
				{
					//when we click into it, we want to change the text, and also go into the actual folder
					folderPathName = relativePath.string();
					currentDirectory /= directory.path().filename();	//basically does currentPath/folderName
				}
			}
		}
		//Then if we're NOT in the default directory we need to be able to go back
		if (currentDirectory != defaultDirectory)
		{
			if (ImGui::Button("Back"))
			{
				//We go back 1, which is the parent path. We also set the string again
				currentDirectory = currentDirectory.parent_path();
				relativePath = std::filesystem::relative(currentDirectory, std::filesystem::current_path());
				folderPathName = relativePath.string();
			}
		}
		//Now we deal with the files in the next column
		ImGui::NextColumn();
		//Might need to make like a new column count thing??? hmm...
		int columnCount{ 5 };
		int currentCount{ 0 };

		//We iterate though the current directory once again but this time we show if it's NOT a folder
		for (auto& directory : std::filesystem::directory_iterator(currentDirectory))
		{
			if (!directory.is_directory())
			{
				currentCount++;
				std::string FileName = directory.path().filename().stem().string();
				ImGui::PushID(FileName.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				//ImGui::ImageButton((ImTextureID)ASSETMANAGER->GetTextureIndex(directory.path().filename().stem().string().c_str()), { 64,64 }, { 0,1 }, { 1,0 });
				if (directory.path().extension().string() == ".png")
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex(directory.path().filename().stem().string()))), {64,64}, {0,1}, {1,0});
				else if (directory.path().extension().string() == ".wav")
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex("wav"))), {64,64}, {0,1}, {1,0});
				else 
				ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<uint64_t>(ASSETMANAGER->GetTextureIndex("FileIcon"))), {64,64}, {0,1}, {1,0});
				//DebuggerLogFormat("Texture ID : %d", ASSETMANAGER->GetTextureIndex("cat"));
				//DebuggerLogFormat("Cast Texture ID : %d", *(ImTextureID)ASSETMANAGER->GetTextureIndex("run"));
				ImGui::PopStyleColor();
				//ImGui::Button(FileName.c_str(), { 64,64 });
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					DebuggerLog("Clicked on " + FileName);
					//Load the properties into the inspector
				}

				if (directory.path().extension().string() == ".json")
				{
					if (ImGui::BeginDragDropSource())
					{
						//DebuggerLog(directory.path().extension().string());
						ImGui::SetDragDropPayload("PREFAB", FileName.c_str(), FileName.size());
						ImGui::EndDragDropSource();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						// To be changed to a better identifier for scene files
						if (directory.path().filename().string().find("Scene") != std::string::npos)
						{
							SCENEMANAGER->LoadScene(directory.path().filename().stem().string());
						}
					}
				}
				if (directory.path().extension().string() == ".png")
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("TEXTURE", FileName.c_str(), FileName.size());
						ImGui::EndDragDropSource();
					}
				}
				if (directory.path().extension().string() == ".wav")
				{
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						AUDIOMANAGER->PlaySound(FileName);
						//Load the properties into the inspector
					}
				}

				//The name of the folder is without the file extension probably...
				ImGui::Text(directory.path().filename().stem().string().c_str());

				ImGui::PopID();

			}
		}
		ImGui::Columns(1);


		ImGui::End();
	}
	
}
