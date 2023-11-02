/*!************************************************************************
 \file				LayerStack.h
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief				
 
 This file contains functions declarations of the LayerStack class that 
 manages all of the layers, ensuring that all of them are in the right order 
 to be updated and shown in the program

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once
#include "LitterBox/Engine/Layer.h"
#include <memory>

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
		 Calls the Initialize function in each layer
		*************************************************************************/
		void InitializeLayers();

		/*!***********************************************************************
		 \brief
		 Adds a layer to the LayerStack
		*************************************************************************/
		void AddLayer(std::shared_ptr<Layer> layer);

		/*!***********************************************************************
		 \brief
		 Ensures that the layer is the last layer of the LayerStack
		*************************************************************************/
		void MakeLayerOverlay(std::shared_ptr<Layer> layer);

		/*!***********************************************************************
		 \brief
		 Removes a layer to the LayerStack
		*************************************************************************/
		void RemoveLayer(std::shared_ptr<Layer> layer);

		void Destroy();

		/*!***********************************************************************
		 \brief
		 Provides the start of the LayerStack
		*************************************************************************/
		std::vector<std::shared_ptr<Layer>>::iterator begin();
		/*!***********************************************************************
		 \brief
		 Provides the end of the LayerStack
		*************************************************************************/
		std::vector<std::shared_ptr<Layer>>::iterator end();

		std::vector<std::shared_ptr<Layer>> m_Layers{};
	private:
		unsigned int		m_LayerIndex{};
	};
}


