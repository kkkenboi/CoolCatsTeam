#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Physics/ColliderManager.h"

namespace LB
{
	class EditorCollisionGrid : public Layer {
	public:
		EditorCollisionGrid(std::string layerName);

		void Initialize() override;

		void UpdateLayer() override;

		void Destroy() override;
	};

	extern EditorCollisionGrid* COLLISIONGRID;
}