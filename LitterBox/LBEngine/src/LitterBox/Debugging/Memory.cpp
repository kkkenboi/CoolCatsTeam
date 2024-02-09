/*!************************************************************************
 \file				Memory.cpp
 \author(s)			Ang Jiawei Jarrett
 \par DP email(s):	a.jiaweijarrett@digipen.edu
 \par Course:		CSD2451A
 \date				09/02/24
 \brief

 This file tracks allocations and deallocations used by the game engine.

 Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "Memory.h"

namespace LB 
{
    /*!***********************************************************************
    \brief
    Default constructor
    *************************************************************************/
    Memory::Memory() 
    {
    }

    /*!***********************************************************************
    \brief
    Destructor to free any pages that are left in the Memory Manager
    *************************************************************************/
    Memory::~Memory()
    {
        for (auto it = m_memPages.begin(); it != m_memPages.end(); ++it)
        {
            for (auto page : it->second)
            {
                _aligned_free(page);
            }
        }
    }
}
