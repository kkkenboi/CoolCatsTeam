// This system handles the pooling of RigidBodies
// Single responsibility principle

#pragma once

#include <vector>
#include "RigidBody.h"

class PhysicsManager {
    private:
    size_t m_poolSize;
    size_t m_currentIndex; // Holds current index in the pool when checking activeness
    RigidBody** m_rigidBodies;
    bool* m_rbStates; // To track if a RigidBody is in use or not
    
    public:
    // Allocates memory to the pool using size
    // Also sets all the states to false
    PhysicsManager(int size) : m_poolSize(size), m_currentIndex(0) {
        m_rigidBodies = new RigidBodies*[size];
        m_rbStates = new bool[size];
        
        // Initialize all the RigidBody States to false;
        for (int i = 0; i < poolSize; ++i) {
            m_rbStates[i] = false;
        }
    }
}