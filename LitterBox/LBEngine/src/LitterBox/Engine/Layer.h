/*!************************************************************************
 \file				Layer.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief	

 This file contains functions declarations of the Layer class that provides 
 a "layer" on the screen which will allow for overlay layers, and additional 
 interfaces such as ImGui to coexist with the editor.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Layer class provides a single layer on the screen that will be constantly
	 updated each frame.
	*************************************************************************/
	class Layer
	{
	public:
		/*!***********************************************************************
		 \brief
		 Creates a layer with supplied name
		*************************************************************************/
		Layer(std::string layerName);

		/*!***********************************************************************
		 \brief
		 Initializes anything inside this function after construction
 		*************************************************************************/
		virtual void Initialize() {}

		/*!***********************************************************************
		 \brief
		 Destroys the layer
		*************************************************************************/
		virtual ~Layer() {}

		/*!***********************************************************************
		 \brief
		 Updates the layer
		*************************************************************************/
		virtual void UpdateLayer() {}

		/*!***********************************************************************
		 \brief
		 Gets the name of the layer
		*************************************************************************/
		std::string const& GetName();

		// Future implementation: 
		// Could add AttachLayer and DetachLayer when working
		// with more than one layer

	private:
		std::string m_Layer;
	};
}

