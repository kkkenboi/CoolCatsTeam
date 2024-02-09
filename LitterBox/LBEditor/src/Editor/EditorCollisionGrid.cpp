/*!************************************************************************
 \file				EditorCollisionGrid.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:       CSD2401A
 \date				09/02/2024
 \brief

 This file contains the Editor things for the CollisionGrid in ImGui

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "EditorCollisionGrid.h"

namespace LB
{
	EditorCollisionGrid* COLLISIONGRID = nullptr;
	/*!***********************************************************************
	  \brief
	  Constructs the EditorCollisionGrid
	*************************************************************************/
	EditorCollisionGrid::EditorCollisionGrid(std::string layerName) : Layer(layerName)
	{
		if (!COLLISIONGRID)
		{
			COLLISIONGRID = this;
		}
		else
		{
			DebuggerLogError("Collision Grid already exists!");
		}
	}
	/*!***********************************************************************
	  \brief
	  Initializes the EditorCollisionGrid
	*************************************************************************/
	void EditorCollisionGrid::Initialize()
	{

	}
	/*!***********************************************************************
	  \brief
	  Updates the EditorCollisionGrid
	*************************************************************************/
	void EditorCollisionGrid::UpdateLayer()
	{
		if (ImGui::Begin(GetName().c_str()))
		{
			ImGui::Text("%-19s", "Rows");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150.f);
			int stored_rows = COLLIDERS->GetGridSystem().GetRows();
			ImGui::DragInt("##Rows", &COLLIDERS->GetGridSystem().GetRowsRef(), 0.1f, 1, 10, "%d");
			if (stored_rows != COLLIDERS->GetGridSystem().GetRows()) {
				COLLIDERS->GetGridSystem().NeedsUpdate();
			}

			ImGui::Text("%-19s", "Columns");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(150.f);
			int stored_columns = COLLIDERS->GetGridSystem().GetColumns();
			ImGui::DragInt("##Columns", &COLLIDERS->GetGridSystem().GetColumnsRef(), 0.1f, 1, 10, "%d");
			if (stored_columns != COLLIDERS->GetGridSystem().GetColumns()) {
				COLLIDERS->GetGridSystem().NeedsUpdate();
			}

		}
		ImGui::End();
	}
	/*!***********************************************************************
	  \brief
	  Destroys the EditorCollisionGrid class
	*************************************************************************/
	void EditorCollisionGrid::Destroy()
	{

	}

	
}