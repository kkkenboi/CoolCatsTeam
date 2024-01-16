/*!************************************************************************
 \file				InspectorSpriteSheet.cpp
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

#include <imgui.h>
#include "InspectorSpriteSheet.h"

namespace LB
{
    /*!***********************************************************************
     \brief

    *************************************************************************/
    void InspectorSpriteSheet::Initialize()
    {

    }

    /*!***********************************************************************
     \brief

    *************************************************************************/
    void InspectorSpriteSheet::UpdateLayer()
    {
        // Name
        ImGui::Text("%-17s", "Name");
        ImGui::SameLine();
        if (ImGui::InputText("##Name", m_spriteSheetName, 256))
        {

        }

        // Preview with slices


        // Slice information
        if (ImGui::Button("Add Slice"))
        {
            m_inspectedSheet->Slice({ 0, 0 }, 0, 0);
        }

        int x, y, w, h;
        for (Sprite const& sprite : m_inspectedSheet->Sprites())
        {
            ImGui::Text("Slice %d:", sprite.m_index);
            ImGui::InputInt("X", &x);
            ImGui::SameLine();
            ImGui::InputInt("Y", &y);
            ImGui::SameLine();
            ImGui::InputInt("Width", &w);
            ImGui::SameLine();
            ImGui::InputInt("Height", &h);
        }
    }
}