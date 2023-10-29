/*!************************************************************************
 \file				LayerStack.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions defintions of the LayerStack class that
 manages all of the layers, ensuring that all of them are in the right order
 to be updated and shown in the program

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "LayerStack.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Creates a LayerStack
	*************************************************************************/
	LayerStack::LayerStack()
	{
		// Empty by design
	}

	/*!***********************************************************************
	 \brief
	 Destroys a LayerStack
	*************************************************************************/
	LayerStack::~LayerStack()
	{

	}

	/*!***********************************************************************
	 \brief
	 Calls the Initialize function in each layer
	*************************************************************************/
	void LayerStack::InitializeLayers()
	{
		for (std::unique_ptr<Layer>& layer : m_Layers)
		{
			layer->Initialize();
		}
	}

	/*!***********************************************************************
	 \brief
	 Adds a layer to the LayerStack

	 \return
	 Nothing
	*************************************************************************/
	void LayerStack::AddLayer(std::unique_ptr<Layer> layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayerIndex++, std::move(layer));
	}

	/*!***********************************************************************
	 \brief
	 Ensures that the layer is the last layer of the LayerStack

	 \return
	 Nothing
	*************************************************************************/
	void LayerStack::MakeLayerOverlay(std::unique_ptr<Layer> layer)
	{
		std::vector<std::unique_ptr<Layer>>::iterator it = m_Layers.begin();

		// Checks if there is duplicate layers
		while (it != m_Layers.end())
		{
			if (*it == layer)
			{
				std::cerr << "Error: Layer is already within the LayerStack!\n";
			}
		}
		// Else set layer as overlay layer
		m_Layers.emplace_back(std::move(layer));
	}

	/*!***********************************************************************
	 \brief
	 Removes a layer to the LayerStack

	 \return
	 Nothing
	*************************************************************************/
	void LayerStack::RemoveLayer(std::unique_ptr<Layer> layer)
	{
		std::vector<std::unique_ptr<Layer>>::iterator it = m_Layers.begin();

		while (it != m_Layers.end())
		{
			if (*it == layer)
			{
				m_Layers.erase(it);
				--m_LayerIndex;
			}
		}
	}

	void LayerStack::Destroy()
	{
		for (std::unique_ptr<Layer>& layer : m_Layers)
		{
			std::cout << layer->GetName() << std::endl;
			layer->Destroy();
		}
	}


	/*!***********************************************************************
	 \brief
	 Provides the start of the LayerStack

	 \return
	 Iterator to the start of the LayerStack
	*************************************************************************/
	std::vector<std::unique_ptr<Layer>>::iterator LayerStack::begin()
	{
		return m_Layers.begin();
	}

	/*!***********************************************************************
	 \brief
	 Provides the end of the LayerStack

	 \return
	 Iterator to the end of the LayerStack
	*************************************************************************/
	std::vector<std::unique_ptr<Layer>>::iterator LayerStack::end()
	{
		return m_Layers.end();
	}
}


