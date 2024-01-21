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
#include <LitterBox/Debugging/Debug.h>

namespace LB
{
    template <typename T>
    class Singleton
    {
    public:
        static std::shared_ptr<T> Instance()
        {
            if (!m_instance) m_instance = std::make_shared<T>();
            return m_instance;
        }

        template <typename... Args>
        static void InitializeSingleton(Args&&... args)
        {
            if (m_instance) {
                DebuggerLogError("Tried to initialize singleton of type %s that already exists!", typeid(m_instance).name);
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
