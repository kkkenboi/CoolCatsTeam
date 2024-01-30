#include "pch.h"
#include "EditorCollisionGrid.h"

namespace LB
{
	EditorCollisionGrid* COLLISIONGRID = nullptr;

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

	void EditorCollisionGrid::Initialize()
	{

	}

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

	void EditorCollisionGrid::Destroy()
	{

	}

	
}