#include "pch.h"
#include "EditorCollisionLayer.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	EditorCollisionLayer* COLLISIONLAYER = nullptr;

	static float const DropDownWidth = 150.f;

	EditorCollisionLayer::EditorCollisionLayer(std::string layerName) : Layer(layerName)
	{
		if (!COLLISIONLAYER)
		{
			COLLISIONLAYER = this;
		}
		else
		{
			DebuggerLogError("Collision Layer already exists!");
		}
	}

	void EditorCollisionLayer::Initialize()
	{

	}

	size_t currentItem{ 0 };

	void EditorCollisionLayer::UpdateLayer()
	{
		if (ImGui::Begin(GetName().c_str()))
		{
			ImGui::Text("%-19s", "Layer");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(DropDownWidth);
			if (ImGui::BeginCombo("##LayerCollider", COLLIDERS->GetLayerSystem().GetLayerVector()[currentItem].first.c_str()) )
			{
				for (size_t i = 0; i < COLLIDERS->GetLayerSystem().GetLayerVector().size(); ++i)
				{
					bool isSelected = (i == currentItem);
					if (ImGui::Selectable(COLLIDERS->GetLayerSystem().GetLayerVector()[i].first.c_str(), isSelected))
					{
						currentItem = i;
					}
				}
				ImGui::EndCombo();
			}

			for (size_t j = 0; j < COLLIDERS->GetLayerSystem().GetLayerVector().size(); ++j)
			{
				//auto& ref = static_cast<bool>(COLLIDERS->GetLayerSystem().GetLayerMatrix()[i][j]);
				bool state = (COLLIDERS->GetLayerSystem().GetLayerMatrix()[currentItem][j] != 0);
				ImGui::Checkbox(COLLIDERS->GetLayerSystem().GetLayerVector()[j].first.c_str(),
					&state);
			}

		}
		ImGui::End();
	}

	void EditorCollisionLayer::Destroy()
	{

	}
}