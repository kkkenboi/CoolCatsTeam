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

// DEBUG : TO REMOVE
#include <iostream>

namespace LB
{
    void LBEngine::Create()
    {
        // Initialize all systems
        for (auto& type : rttr::type::get_types()) 
        {
            if (!type.is_class()) continue;

            if (type.is_derived_from<ISystem>() && (type.get_name() != "ISystem" && type.get_name() != "IFixedUpdateSystem" && type.get_name() != "IUpdateSystem"))
            {
                std::shared_ptr<ISystem> newSystem{ type.create().get_value<std::shared_ptr<ISystem>>() };

                m_systems.push_back(newSystem);
                if (type.is_derived_from<IUpdateSystem>()) 
                {
                    m_updateSystems.push_back(newSystem);
                }
                if (type.is_derived_from<IFixedUpdateSystem>()) 
                {
					m_fixedUpdateSystems.push_back(newSystem);
				}
            }
        }

        // Order the systems

	}

    void LBEngine::Initialize()
	{
		
	}

    void LBEngine::Update()
    {

    }

    void LBEngine::Destroy()
    {

    }
}
