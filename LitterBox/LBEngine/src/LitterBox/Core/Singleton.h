/*!************************************************************************
 \file				Singleton.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				13/01/2024
 \brief

 This file contains the Singleton class that allows for the creation of
 a single instance of a class.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <memory>
#include <type_traits>
#include <LitterBox/Debugging/Debug.h>

namespace LB
{
    /*!***********************************************************************
     \brief
     The singleton class is a template class that allows for the creation of
     a single instance of a class. This is useful for classes that should
     only have one instance, such as a game manager.
    *************************************************************************/
    template <typename T>
    class Singleton
    {
    public:
        /*!***********************************************************************
         \brief
         Returns the instance of the singleton. If the instance does not exist,
         it will be created.
        *************************************************************************/
        static std::shared_ptr<T> Instance()
        {
            if (!m_instance)
            {
                if constexpr (std::is_default_constructible_v<T>)
                {
                    m_instance = std::make_shared<T>();
                }
                else
                {
					DebuggerLogErrorFormat("Singleton %s cannot be default constructed! Initialize with InitializeSingleton().", typeid(m_instance).name());
				}
            }
            return m_instance;
        }

        /*!***********************************************************************
         \brief
         Initializes the singleton with the given arguments. If the instance
         already exists, an error will be logged.
        *************************************************************************/
        template <typename... Args>
        static void InitializeSingleton(Args&&... args)
        {
            if (m_instance) {
                DebuggerLogFormat("Tried to initialize singleton of type %s that already exists!", typeid(m_instance).name());
                return;
            }
            m_instance = std::make_shared<T>(std::forward<Args>(args)...);
        }

    private:
        static std::shared_ptr<T> m_instance;
    };

    // Static member initialization
    template <typename T>
    std::shared_ptr<T> Singleton<T>::m_instance;
}
