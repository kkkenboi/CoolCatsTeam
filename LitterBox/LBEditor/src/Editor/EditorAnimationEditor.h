#pragma once

#include "Editor.h"
#include "Litterbox/Engine/Layer.h"

namespace LB {
	class EditorAnimationEditor : public Layer {
	public:
		/*!***********************************************************************
		  \brief
		  Constructor for the EditorAnimationEditor class.
		  \return
		  Nothing.
		*************************************************************************/
		EditorAnimationEditor(std::string layerName);

		/*!***********************************************************************
		  \brief
		  Updates the EditorAnimationEditor layer.
		  \return
		  Nothing.
		*************************************************************************/
		void UpdateLayer() override;

		/*!***********************************************************************
		  \brief
		  Destroys the EditorAnimationEditor layer.
		  \return
		  Nothing.
		*************************************************************************/
		void Destroy() {};
	};
}