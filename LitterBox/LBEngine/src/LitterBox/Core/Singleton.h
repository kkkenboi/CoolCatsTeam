/*!************************************************************************
 \file				Singleton.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:       CSD2401A
 \date				13/01/2024
 \brief


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
    template <typename T>
    class Singleton
    {
    public:
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

    template <typename T>
    std::shared_ptr<T> Singleton<T>::m_instance;
}
