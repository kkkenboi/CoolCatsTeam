/*!************************************************************************
 \file				EditorAssets.cpp
 \author(s)			Ang Jiawei Jarrett,Amadeus Chia Jinhan
 \par DP email(s):	a.jiaweijarrett@digipen.edu, amadeusjinhan.chia@digipen.edu
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
#include "EditorInspector.h"
#include "LitterBox/Core/Core.h"

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
		UNREFERENCED_PARAMETER(window);
		int i;
		for (i = 0; i < count; i++)
		{
			//std::cout << std::filesystem::path{ paths[i] } << '\n';
			//std::cout << EDITORASSETS->currentDirectory << '\n';
			try
			{
				std::filesystem::path fileToCopy{ paths[i] };
				std::filesystem::copy_file(fileToCopy, EDITORASSETS->currentDirectory/fileToCopy.filename());
				ReimportAssets();
				//WINDOWSSYSTEM->OnApplicationFocus.Invoke();
			}
			catch (std::filesystem::filesystem_error& e)
			{
				DebuggerLogFormat("Could not copy %s %s", paths[i], e.what());
				//std::cout << "Could not copy " << paths[i] << " " << e.what() << '\n';
			}
		}
	}

	/*!***********************************************************************
	  \brief
	  Constructor for the EditorAssets class. Also sets the default file dir
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
	  Initializes the EditorAssets layer. (Subscribes the callback function)
	  \return
	  Nothing.
	*************************************************************************/
	void EditorAssets::Initialize()
	{
		glfwSetDropCallback(WINDOWSSYSTEM->GetWindow(), drop_callback);
	}

	/*!***********************************************************************
	  \brief
	  Updates the EditorAssets layer. Handles the bulk of the content browser 
	  \return
	  Nothing.
	*************************************************************************/
	void EditorAssets::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());
		//float panelWidth = ImGui::GetContentRegionAvail().x;
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
		//int columnCount{ 5 };
		//int currentCount{ 0 };

		//We iterate though the current directory once again but this time we show if it's NOT a folder
		for (auto& directory : std::filesystem::directory_iterator(currentDirectory))
		{
			if (!directory.is_directory())
			{
				//currentCount++;
				std::string FileName = directory.path().filename().stem().string();
				ImGui::PushID(FileName.c_str());
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				
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
					std::string fileExtension = directory.path().extension().string();
					if (fileExtension != ".prefab" && 
						fileExtension != ".scene" && 
						fileExtension != ".wav" && 
						fileExtension != ".png" && 
						fileExtension != ".shader" && 
						fileExtension != ".otf" && 
						fileExtension != ".ttf")
					{
						DebuggerLog("Invalid file extension " + fileExtension + " was clicked!");
						ImGui::OpenPopup("Error!");
					}
					//Load the properties into the inspector
				}

				//IF IT'S A PREFAB
				if (directory.path().extension().string() == ".prefab")
				{
					//PREFAB DRAG AND DROP TO SCENE
					if (ImGui::BeginDragDropSource())
					{
						//DebuggerLog(directory.path().extension().string());
						ImGui::SetDragDropPayload("PREFAB", FileName.c_str(), FileName.size()+1);
						ImGui::EndDragDropSource();
					}
					//PREFAB EDITOR STUFF
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						// To be changed to a better identifier for scene files
						if (directory.path().filename().string().find("Scene") != std::string::npos)
						{
							if (!CORE->IsPlaying())
								SCENEMANAGER->LoadScene(directory.path().filename().stem().string());
							else
								DebuggerLogWarningFormat("Tried to load new scene from Assets %s while a scene is running.", directory.path().filename().stem().string().c_str());
						}
						else if (directory.path().filename().string().find("SpriteSheet") != std::string::npos) //NEED CHANGE
						{
							INSPECTORSPRITESHEET->LoadSpriteSheet(directory.path().filename().stem().string());
							EDITORINSPECTOR->SetWindowSpriteSheet();
						}
						else //that means it's a prefab instead
						{
							DebuggerLog(directory.path().filename().string());
							
						GameObject* prefab = FACTORY->SpawnGameObject({}, GOSpawnType::FREE_FLOATING);
						JSONSerializer::DeserializeFromFile(FileName.c_str(), *prefab);
						prefab->SetName(FileName.c_str());
						if (prefab->HasComponent<CPRender>()) prefab->GetComponent<CPRender>()->set_active();
						InspectorGameObject::Instance()->UpdateInspectedGO(prefab);
						InspectorGameObject::Instance()->isPrefab = true;
						// To be changed to a better identifier for scene files
					}
				}
				//IF IT IS A SCENE!!!
				if (directory.path().extension().string() == ".scene")
				{
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						if (!CORE->IsPlaying())
							SCENEMANAGER->LoadScene(directory.path().filename().stem().string());
						else
							DebuggerLogWarningFormat("Tried to load new scene from Assets %s while a scene is running.", directory.path().filename().stem().string().c_str());
					}
				}
				if (directory.path().extension().string() == ".png")
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("TEXTURE", FileName.c_str(), FileName.size()+1);
						ImGui::EndDragDropSource();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{

					}

					//if
				}
				if (directory.path().extension().string() == ".wav")
				{
					if (ImGui::BeginDragDropSource())
					{
						ImGui::SetDragDropPayload("AUDIO", FileName.c_str(), FileName.size()+1);
						ImGui::EndDragDropSource();
					}
					if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						AUDIOMANAGER->ToggleSoundPlaying(FileName);
						/*AUDIOMANAGER->StopAllChannels();
						AUDIOMANAGER->PlaySound(FileName);*/
						//Load the properties into the inspector
					}
				}

				


				//IF USER RIGHT CLICKS
				if (ImGui::BeginPopupContextItem())
				{
					//Ui window pops out with the delete button
					ImGui::Text("Options");
					if (ImGui::Button("Delete"))
					{
						//This opens out the delete confirmation if they click delete
						ImGui::OpenPopup("Delete?");
					}
					// Always center this window when appearing
					ImVec2 center = ImGui::GetMainViewport()->GetCenter();
					ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
					//This opens up the confirmation window
					if (ImGui::BeginPopupModal("Delete?", NULL, ImGuiWindowFlags_AlwaysAutoResize))
					{
						ImGui::Text("Are you sure you want to delete this asset?\nThis action CANNOT be undone!");
						ImGui::Separator();

						if (ImGui::Button("Confirm")) 
						{ 
							//CPP function to delete a file from it's file path.
							//We're guaranteed to have a valid filepath so we don't have to check if it exists or not.
							std::remove(directory.path().string().c_str());
							ReimportAssets();	//we have to reimport it to update the asset map
							ImGui::ClosePopupToLevel(0, true);	//this closes ALL windows including the previous pop up
						}
						ImGui::SameLine();
						if (ImGui::Button("Cancel")) {  ImGui::ClosePopupToLevel(0, true); }

						ImGui::SetItemDefaultFocus();
						ImGui::EndPopup();
					}
					ImGui::EndPopup();
				}
				//The name of the folder is without the file extension probably...
				ImGui::Text(directory.path().filename().stem().string().c_str());

				
				// Always center this window when appearing
				ImVec2 center = ImGui::GetMainViewport()->GetCenter();
				ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

				

				if (ImGui::BeginPopupModal("Error!", NULL, ImGuiWindowFlags_AlwaysAutoResize))
				{
					ImGui::Text("File type not recognised!");
					ImGui::Separator();

					if (ImGui::Button("close")) { ImGui::CloseCurrentPopup(); }
					ImGui::SetItemDefaultFocus();
					ImGui::SameLine();
					ImGui::EndPopup();
				}
				ImGui::PopID();

			}
		}
		ImGui::Columns(1);

		ImGui::End();
	}
	
}
