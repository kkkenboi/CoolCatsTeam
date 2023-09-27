/*!************************************************************************
 \file			Memory.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

**************************************************************************/

#include "Memory.h"

namespace LB 
{
    Memory* MEMORY = nullptr;

    Memory::Memory() 
    {
        if (!MEMORY)
            MEMORY = this;
        else
            std::cerr << "Memory System already exist" << std::endl;


    }

    void Memory::Destroy()
    {
        if (!allocs.empty())
        {
            std::cerr << "Memory Leak!\n";
            for (auto const& entry : allocs) {
                std::cerr << "Size: " << entry.second << " bytes\n";
            }
        }
    }

}
