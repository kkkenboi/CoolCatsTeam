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

    private:
        char m_spriteSheetName[256]{};
        SpriteSheet* m_inspectedSheet;
	};
}