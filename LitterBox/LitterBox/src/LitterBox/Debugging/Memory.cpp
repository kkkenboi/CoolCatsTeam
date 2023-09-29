/*!************************************************************************
 \file				Memory.cpp
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

#include "Memory.h"

namespace LB 
{
    Memory* MEMORY = nullptr;

    /*!***********************************************************************
    \brief
    Default constructor, ensures this system is a singleton
    *************************************************************************/
    Memory::Memory() 
    {
        if (!MEMORY)
            MEMORY = this;
        else
            std::cerr << "Memory System already exist" << std::endl;
    }

    /*!***********************************************************************
    \brief
    Frees any memory allocated that as not been freed, also prints a warning!
    *************************************************************************/
    void Memory::Destroy()
    {
        if (!allocs.empty())
        {
            DebuggerLogWarning("Internal Memory Leak!");
            for (auto const& entry : allocs) {
                std::cerr << "Size: " << entry.second << " bytes\n";
            }
        }
    }

}
