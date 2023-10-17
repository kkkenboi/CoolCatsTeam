/*!************************************************************************
 \file				Memory.h
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2401A
 \date				22/09/23
 \brief

 This file tracks allocations and deallocations used by the game engine.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#pragma once

#include <map>
#include "LitterBox/Core/System.h"

namespace LB 
{
    /*!***********************************************************************
     \brief
     Memory class is an Engine system meant to overwrite new and delete, this
     way the system can track alloc/deallocs and memory leaks
    *************************************************************************/
    class Memory : public ISystem 
    {
    public:
        /*!***********************************************************************
        \brief
        Default constructor, ensures this system is a singleton
        *************************************************************************/
        Memory();

        /*!***********************************************************************
        \brief
        Sets up the name for profiling
        *************************************************************************/
        void Initialize() override { SetSystemName("Memory System"); }

        /*!***********************************************************************
        \brief
        Frees any memory allocated that as not been freed, also prints a warning!
        *************************************************************************/
        void Destroy() override;

        /*!***********************************************************************
        \brief
        Allocates memory for any object of T
        *************************************************************************/
        template <typename T>
        T* Allocate();

        /*!***********************************************************************
        \brief
        Allocates memory for any object of T with parameterized construction
        *************************************************************************/
        template <typename T, typename... Args>
        T* Allocate(Args&&... args);

        /*!***********************************************************************
        \brief
        Frees the memory for any pointer T
        *************************************************************************/
        template <typename T>
        void Deallocate(T* ptr);

    private:
        /*!***********************************************************************
        \brief
        Contains all pointers to memory allocated using Allocate
        *************************************************************************/
        std::map<void*, size_t> allocs;
    };

    // A pointer to the system object in the core engine made to be singleton
    extern Memory* MEMORY;
}

/***************************************************************************************************
*
* Template functions definitions
*
***************************************************************************************************/

namespace LB
{
    /*!***********************************************************************
    \brief
    Allocates memory for any object of T
    *************************************************************************/
    template <typename T>
    T* Memory::Allocate()
    {
        T* ptr = new T;
        allocs[(void*)ptr] = sizeof(T);
        return ptr;
    }

    /*!***********************************************************************
    \brief
    Allocates memory for any object of T with parameterized construction
    *************************************************************************/
    template <typename T, typename... Args>
    T* Memory::Allocate(Args&&... args)
    {
        T* ptr = new T(std::forward<Args>(args)...);
        allocs[(void*)ptr] = sizeof(T);
        return ptr;
    }

    /*!***********************************************************************
    \brief
    Frees the memory for any pointer T
    *************************************************************************/
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
