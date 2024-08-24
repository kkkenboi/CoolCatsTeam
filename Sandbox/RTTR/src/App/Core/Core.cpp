/*!************************************************************************
 \file				Core.cpp
 \author(s)
 \par DP email(s):
 \par Course:
 \date
 \brief

 This file contains

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Core.h"

namespace LB
{
    void LBEngine::Create()
    {
        // Initialize all systems
        for (auto& type : rttr::type::get_types()) {
            if (type.is_derived_from<ISystem>()) {
                std::shared_ptr<ISystem> newSystem = std::make_shared<ISystem>(type.create().get_value<ISystem*>());
                m_systems.push_back(newSystem);

                if (type.is_derived_from<IUpdateSystem>()) {
                    m_updateSystems.push_back(newSystem);
                }
                if (type.is_derived_from<IFixedUpdateSystem>()) {
					m_fixedUpdateSystems.push_back(newSystem);
				}
            }
        }

        // Order the systems

	}
}
