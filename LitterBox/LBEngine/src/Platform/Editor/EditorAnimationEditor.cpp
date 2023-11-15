#include "EditorAnimationEditor.h"

LB::EditorAnimationEditor::EditorAnimationEditor(std::string layerName) : Layer(layerName)
{
	
}

//test listing for list box
const char* list[] = { "First texture", "second texture" };
int currItem{ 0 };

void LB::EditorAnimationEditor::UpdateLayer()
{
	ImGui::Begin(GetName().c_str());

	ImGui::BeginChild("AnimationEditor");
	ImVec2 wsize = ImGui::GetWindowSize();

	ImGui::Combo("Texture", &currItem, list, IM_ARRAYSIZE(list));

	ImGui::EndChild();

	ImGui::End();
}


