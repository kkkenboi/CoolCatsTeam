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
            if (type.is_derived_from<ISystem>() && (type.get_name() != "ISystem" && type.get_name() != "IFixedUpdateSystem" && type.get_name() != "IUpdateSystem"))
            {
                rttr::variant test{ type.create() };
                std::cout << "Coming " << test.get_type().get_name() << " done" << std::endl;

                /*std::shared_ptr<ISystem> newSystem{ std::shared_ptr<ISystem>(test.get_value<ISystem*>()) };

                m_systems.push_back(newSystem);
                if (type.is_derived_from<IUpdateSystem>()) 
                {
                    m_updateSystems.push_back(newSystem);
                }
                if (type.is_derived_from<IFixedUpdateSystem>()) 
                {
					m_fixedUpdateSystems.push_back(newSystem);
				}*/
            }
        }

        // Order the systems
	}

    void LBEngine::Initialize()
	{
		
	}

    void LBEngine::Update()
    {
        // DEBUG print names
        std::cout << m_systems.size() << std::endl;
  //      for (auto& system : m_systems)
		//{
  //          std::cout << "Type: " << typeid(system).name() << std::endl;
		//}
    }

    void LBEngine::Destroy()
    {

    }
}
