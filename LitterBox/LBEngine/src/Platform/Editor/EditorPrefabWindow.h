#pragma once

#include "Platform/Editor/Editor.h"
#include "LitterBox/Engine/Layer.h"

namespace LB
{
	class EditorPrefabWindow : public Layer
	{
	public:
		EditorPrefabWindow(std::string layerName);
		void Initialize() override;
		void UpdateLayer() override;

	private:

	};
}