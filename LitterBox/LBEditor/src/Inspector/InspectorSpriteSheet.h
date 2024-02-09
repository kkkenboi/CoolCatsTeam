/*!************************************************************************
 \file				InspectorSpriteSheet.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains the class is where it updates the spritesheet, split, 
 save, etc the spritesheet in the inspector. Declarations.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include "LitterBox/Core/Singleton.h"
#include "Litterbox/Engine/Layer.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Animation/SpriteSheet.h"

namespace LB
{
    /*!***********************************************************************
      \brief
      InspectorSpriteSheet Class that inherits layer and singleton.
    *************************************************************************/
	class InspectorSpriteSheet : public Layer, public Singleton<InspectorSpriteSheet>
	{
    public:
        /*!***********************************************************************
          \brief
          Initialize function, where it init the textureID, slotID, textureAspect,
          textureSize
        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief
         Where ImGui will be to show on the Inspector, runs the splitting sprite
         function
        *************************************************************************/
        void UpdateLayer() override;

        /*!***********************************************************************
          \brief
         Saving the SpriteSheet, where it serialise the spritesheet
        *************************************************************************/
        void SaveSpriteSheet();

        /*!***********************************************************************
          \brief
         Loads the AssetManager and deserailise the spritesheet
        *************************************************************************/
        void LoadSpriteSheet(std::string name);

        /*!***********************************************************************
          \brief
         Shows Imgui to allow user to manual and auto slice (auto slice for now),
         user can set the amount of rows and cols for how many sprites they want
         to split into from the spritesheet
        *************************************************************************/
        void SpilttingTheSprites();

        /*!***********************************************************************
          \brief
         Creating the UV of the spritesheet
        *************************************************************************/
        void createUV(int rows, int cols);


    private:
        SpriteSheet m_inspectedSheet;

        std::string m_fileName{};
        char m_name[256]{}; //
        int m_textureID{ 0 }; // Getting the texture ID
        int m_sliceX{ 0 }, m_sliceY{ 0 }, m_sliceWidth{ 0 }, m_sliceHeight{ 0 }; //in update layer, set x and y, width and height
        int m_row{ 0 }, m_col{0}; //set row and col

        int m_spriteX{}, m_spriteY{}, m_spriteW{}, m_spriteH{};


        int textureID{}, slotID{}; //textureID ad slotID
        float textureAspect{}; //Aspect of the ratio
        ImVec2 textureSize{}; //Texture Size
	};
}