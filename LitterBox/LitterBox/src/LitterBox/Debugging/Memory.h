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
    class Memory : ISystem 
    {
        public:
        virtual std::string GetName() { return "Memory System"; }

        template <typename T>
        T* Allocate() 
        {
            T* ptr = new T;
            allocs[(void*)ptr] = sizeof(T);
            return ptr;
        }

        template <typename T>
        void Deallocate(T* ptr) 
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

        virtual void Destroy() override 
        {
            if (!allocs.empty()) 
            {
                std::cerr << "Memory Leak!\n";
                for (auto const& entry : allocs) {
                    std::cerr << "Size: " << entry.second << " bytes\n";
                }
            }
        }

        private:
        std::map<void*, size_t> allocs;
    };

}
