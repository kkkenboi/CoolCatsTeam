#pragma once
#include "RigidBody.h"

class RigidBodyManager {
private:
    size_t m_poolSize;
    size_t m_currentIndex; // Holds current index in the pool when checking activeness
    RigidBody** m_rigidBodies; // Array of pointers to RigidBodies
    bool* m_rbStates; // To track if a RigidBody is in use or not (pulled out of the pool)

public:

    // Allocates memory to the pool using size
    // Also sets all the states to false
    // Constructor
    RigidBodyManager(int size);

    // Destructor
    ~RigidBodyManager();

    // This functions allows you to add a RigidBody to the pool
    void AddRigidBodyToPool(RigidBody* rb);

    // This allows you to find the RigidBody within an index
    RigidBody* GetPooledRigidBody(size_t index);

    // This allows you to find the next nearest unpulled RigidBody in the array
    // Returns a pointer to the Rigidbody allowing you to check its calculations
    RigidBody* GetPooledRigidBody();

    // This function allows you to return a RigidBody that was pulled previously
    // back to the Manager, which means that it is not being calculated on anymore
    void ReturnPooledRigidBody(RigidBody* rb);

};

// Check collisions between two RigidBodies
bool CheckCollisions(RigidBody* bodyA, RigidBody* bodyB, Vec2<float>& normal_out, float& depth_out);

void ResolveCollisions(RigidBody* bodyA, RigidBody* bodyB, Vec2<float> normal, float depth);