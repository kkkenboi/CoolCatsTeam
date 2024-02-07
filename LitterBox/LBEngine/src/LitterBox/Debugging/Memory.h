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

#include "LitterBox/Core/System.h"

#include <typeindex>
#include <cstdlib>
#include <cstring>
#include <list>
#include <map>

constexpr unsigned int PAGE_SIZE = 4096;

namespace LB
{
    /*!***********************************************************************
     \brief
     The Memory Class is a memory manager that allocates and deallocates memory
     mainly for IComponents.
     
     Users that want to allocate will need to use,
     'Memory::Instance()->Allocate<T>();'

     and to deallocate will use
     'Memory::Instance()->Deallocate(ptr);'

     Users are responsible for deallocating if they have allocated for memory.
    *************************************************************************/
    class Memory : public ISystem, public Singleton<Memory>
    {
    public:

        /*!***********************************************************************
        \brief
        Default constructor
        *************************************************************************/
        Memory();

        /*!***********************************************************************
        \brief
        Destructor to free any pages that are left in the Memory Manager
        *************************************************************************/
        ~Memory();

        /*!***********************************************************************
        \param 
        
        \brief
        Allocate the memory within the Memory Manager by checking if there is a 
        page to fit the component, else it will create a new page and provide a
        pointer to the given memory.

        Additionally, the allocate function will allocate in place of the initial
        allocation of the page to the heap, thus ensuring that the memory is 
        in a contiguous manner.

        \return
        A pointer to the allocated memory.
        *************************************************************************/
        template <typename T>
        T* Allocate()
        {
            if (m_memPools[typeid(T)].empty())
            {
                AllocatePage<T>();
            }

            // Removing an available block from the Memory Pool
            T* newObject = reinterpret_cast<T*>(m_memPools[typeid(T)].front());
            m_memPools[typeid(T)].pop_front();

            // From that removed block, replace the block with the memory that
            // we want the heap to use, with our preallocated memory
            newObject = new(newObject) T();

            return newObject;
        }

        /*!***********************************************************************
        \brief
        Allocate the memory within the Memory Manager by checking if there is a
        page to fit the component, else it will create a new page and provide a
        pointer to the given memory.

        Additionally, the allocate function will allocate in place of the initial
        allocation of the page to the heap, thus ensuring that the memory is
        in a contiguous manner.

        A parameterized version to construct parameterized.

        \return
        A pointer to the allocated memory.
        *************************************************************************/
        template <typename T, typename... Args>
        T* Allocate(Args&&... args)
        {
            if (m_memPools[typeid(T)].empty())
            {
                AllocatePage<T>();
            }

            // Removing an available block from the Memory Pool
            T* newObject = reinterpret_cast<T*>(m_memPools[typeid(T)].front());
            m_memPools[typeid(T)].pop_front();

            // From that removed block, replace the block with the memory that
            // we want the heap to use, with our preallocated memory
            newObject = new(newObject) T(std::forward<Args>(args)...);
            return newObject;
        }

        /*!***********************************************************************
        \brief
        Deallocates the pointer that was previously allocated by returning the 
        memory back into the Memory Pool.
        As the Memory Pool is based off on a list of pointers, the internal
        memory is still contiguous and thus it will be fine to just add onto the
        Memory Pool List.

        \return
        Nothing.
        *************************************************************************/
        template <typename T>
        void Deallocate(T* ptr)
        {
            // This destructor is to ensure that any objects that are newed in
            // the construction of the object in Allocate have been properly
            // deleted by the destruction
            ptr->~T();

            // Return the pointer back to the Memory Pool
            m_memPools[typeid(T)].push_back(reinterpret_cast<char*>(ptr));
        }

    private:

        /*!***********************************************************************
        \brief
        When the Memory Manager starts to allocate, it will check if there is
        enough blocks to allocate away in the Memory Pool. If not, this function
        will be called to add another page, which will then add more blocks into
        the Memory Pool.

        \return
        Nothing.
        *************************************************************************/
        template <typename T>
        void AllocatePage()
        {
            // We use _aligned_malloc to ensure that the page itself is aligned by the
            // page size to reduce the number of operations done
            char* newPage = reinterpret_cast<char*>(_aligned_malloc(PAGE_SIZE, PAGE_SIZE));
            memset(newPage, 0, PAGE_SIZE);

            // Update the Memory Pages
            m_memPages[typeid(T)].push_back(newPage);

            // Create new entries into our Memory Pool, each of size T, which is exactly how
            // much we need
            for (int byte{ sizeof(newPage) }; byte + sizeof(T) <= PAGE_SIZE; byte += sizeof(T))
            {
                m_memPools[typeid(T)].push_back(newPage + byte);
            }
        }

        std::map<std::type_index, std::list<char*>> m_memPools;
        std::map<std::type_index, std::list<char*>> m_memPages;
    };
}
