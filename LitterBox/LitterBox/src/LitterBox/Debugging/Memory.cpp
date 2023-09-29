/*!************************************************************************
 \file			Memory.cpp
 \author		Ang Jiawei Jarrett
 \par DP email: a.jiaweijarrett@digipen.edu
 \par Course:	CSD2401A
 \date			22-09-2023
 \brief

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


     \return

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


     \return

    *************************************************************************/
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
