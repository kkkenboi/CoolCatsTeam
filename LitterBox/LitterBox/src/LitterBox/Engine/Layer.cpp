/*!************************************************************************
 \file				Layer.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				29/09/2023
 \brief

 This file contains functions defintions of the Layer class that provides
 a "layer" on the screen which will allow for overlay layers, and additional
 interfaces such as ImGui to coexist with the editor.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "pch.h"
#include "Layer.h"

namespace LB
{
	/*!***********************************************************************
	 \brief
	 Creates a layer with supplied name
	*************************************************************************/
	Layer::Layer(std::string layerName) : m_Layer{ layerName }
	{

	}

	/*!***********************************************************************
	 \brief
	 Gets the name of the layer

	 \return
	 Name of the layer
	*************************************************************************/
	std::string const& Layer::GetName()
	{
		return m_Layer;
	}
}
