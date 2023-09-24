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

        template <typename T>
        T* Allocate();

        template <typename T>
        void Deallocate(T* ptr);

        virtual void Destroy() override;

        private:
        std::map<void*, size_t> allocs;
    };

    extern Memory* MEMORY;
}
