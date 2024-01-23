/*!************************************************************************
 \file				InspectorSpriteSheet.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				11/10/2023
 \brief

 This file contains

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
	class InspectorSpriteSheet : public Layer, public Singleton<InspectorSpriteSheet>
	{
    public:
        /*!***********************************************************************
          \brief

        *************************************************************************/
        void Initialize() override;

        /*!***********************************************************************
          \brief

        *************************************************************************/
        void UpdateLayer() override;

        void SaveSpriteSheet();

        void LoadSpriteSheet(std::string name);

        ////void calPreviewSize(float width, float height, float previewLimit);
        //void PreviewTexture();

        //void SpilttingTheSprites();
        //void createUV(int rows, int cols);

        ////function for generating appropriate size preview
        //void calPreviewSize(float width, float height, float previewLimit);

        //inline unsigned int getNumOfTiles() { return tiles.size(); }
        //inline int getTextureID() { return textureID; }
        //inline auto getMMUV(int index) { return tiles.at(index); }

    private:
        //ImVec4 m_buttonOffColor{ 0.05f, 0.2f, 0.4f, 1.0f };
        //ImVec4 m_buttonOnColor{ 0.2f, 0.6f, 1.0f, 1.0f };

        //std::vector< //tiles
        //    std::pair< //uv
        //    std::pair<float, float>,  //min x,y
        //    std::pair<float, float> //max x,y
        //    >
        //> tiles{};

        SpriteSheet m_inspectedSheet;

        char m_name[256]{};
        int m_slotID;
        /*int m_row{ 0 }, m_col{0};
        int textureID, slotID;
        float textureAspect;
        ImVec2 textureSize;*/
	};

    extern InspectorSpriteSheet* INSPECTORSPRITESHEET;

}