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
        T* Allocate() {
            T* ptr = new T;
            allocations[ptr] = sizeof(T);
            return ptr;
        }

        template <typename T>
        void Deallocate(T* ptr) {
            auto it = allocations.find(ptr);
            if (it != allocations.end()) {
                delete ptr;
                allocations.erase(it);
            }
        }

        ~MemoryManager() {
            // Print information about remaining allocations
            if (!allocations.empty()) {
                std::cout << "Memory Leak Detected!" << std::endl;
                for (const auto& entry : allocations) {
                    std::cout << "Size: " << entry.second << " bytes" << std::endl;
                }
            }
        }

        private:
        std::map<void*, size_t> allocations;
    };

}
