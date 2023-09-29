/*!************************************************************************
 \file				LayerStack.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				This file contains functions declarations of the
					LayerStack class that manages all of the layers,
					ensuring that all of them are in the right order to be
					updated and shown in the program

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Engine/Layer.h"

namespace LB 
{
	/*!***********************************************************************
	 \brief
	 LayerStack class provides functions to add, remove layers, as well as
	 making it the overlay layer.
	*************************************************************************/
	class LayerStack
	{
	public:

		/*!***********************************************************************
		 \brief
		 Creates a LayerStack
		*************************************************************************/
		LayerStack();

		/*!***********************************************************************
		 \brief
		 Destroys a LayerStack
		*************************************************************************/
		~LayerStack();

		/*!***********************************************************************
		 \brief
		 Adds a layer to the LayerStack
		*************************************************************************/
		void AddLayer(Layer* layer);

		/*!***********************************************************************
		 \brief
		 Ensures that the layer is the last layer of the LayerStack
		*************************************************************************/
		void MakeLayerOverlay(Layer* layer);

		/*!***********************************************************************
		 \brief
		 Removes a layer to the LayerStack
		*************************************************************************/
		void RemoveLayer(Layer* layer);

		/*!***********************************************************************
		 \brief
		 Provides the start of the LayerStack
		*************************************************************************/
		std::vector<Layer*>::iterator begin();
		/*!***********************************************************************
		 \brief
		 Provides the end of the LayerStack
		*************************************************************************/
		std::vector<Layer*>::iterator end();

	private:
		std::vector<Layer*> m_Layers{};
		unsigned int		m_LayerIndex{};
	};
}


