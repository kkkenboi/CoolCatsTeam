#include "pch.h"
#include "EditorPrefabWindow.h"

namespace LB
{
	EditorPrefabWindow::EditorPrefabWindow(std::string layerName) : Layer(layerName)
	{
	}
	void EditorPrefabWindow::Initialize()
	{
	}
	void EditorPrefabWindow::UpdateLayer()
	{
		ImGui::Begin(GetName().c_str());
		ImGui::End();
	}
}