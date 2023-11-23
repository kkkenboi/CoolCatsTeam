#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB 
{
	class EditorCollisionLayer : public Layer {
	public:
		EditorCollisionLayer(std::string layerName);

		void Initialize() override;

		void UpdateLayer() override;

		void Destroy() override;

	private:
	};

	extern EditorCollisionLayer* COLLISIONLAYER;
}