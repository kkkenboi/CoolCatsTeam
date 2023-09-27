/*!************************************************************************
 \file			Memory.h
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#pragma once

#include <map>
#include "LitterBox/Core/System.h"

namespace LB 
{
    class Memory : public ISystem 
    {
        public:
        Memory();

        void Initialize() override { SetSystemName("Memory System"); }
        void Destroy() override;

        template <typename T>
        T* Allocate();

        template <typename T, typename... Args>
        T* Allocate(Args&&... args);

        template <typename T>
        void Deallocate(T* ptr);

        private:
        std::map<void*, size_t> allocs;
    };

    extern Memory* MEMORY;
}

/***************************************************************************************************
*
* Template functions definitions
*
***************************************************************************************************/

namespace LB
{

    template <typename T>
    T* Memory::Allocate()
    {
        T* ptr = new T;
        allocs[(void*)ptr] = sizeof(T);
        return ptr;
    }

    template <typename T, typename... Args>
    T* Memory::Allocate(Args&&... args)
    {
        T* ptr = new T(std::forward<Args>(args)...);
        allocs[(void*)ptr] = sizeof(T);
        return ptr;
    }

    template <typename T>
    void Memory::Deallocate(T* ptr)
    {
        auto it = allocs.find((void*)ptr);
        if (it != allocs.end()) {
            delete ptr;
            allocs.erase(it);
        }
        else
        {
            std::cerr << "Memory: Tried to dellocate non-existent ptr of size <" << it->second << " bytes>\n";
        }
    }
}
