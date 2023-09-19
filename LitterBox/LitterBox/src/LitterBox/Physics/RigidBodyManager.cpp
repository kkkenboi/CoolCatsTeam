#pragma once

#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "PhysicsMath.h"


RigidBodyManager::RigidBodyManager(int size) : m_poolSize(size), m_currentIndex(0)
{
    m_rigidBodies = new RigidBody * [size];
    m_rbStates = new bool[size];

    // Initialize all the RigidBody States to false;
    for (int i = 0; i < m_poolSize; ++i)
    {
        m_rigidBodies[i] = nullptr; // Instantiate the array to be all empty
        m_rbStates[i] = false; // Everything is not in use
    }
}

// Destructor
RigidBodyManager::~RigidBodyManager()
{
    for (int i = 0; i < m_poolSize; ++i)
    {
        if (m_rigidBodies[i] != nullptr)
        {
            delete m_rigidBodies[i];
        }
    }
    delete[] m_rigidBodies;
    delete[] m_rbStates;
}

// This functions allows you to add a RigidBody to the pool
void RigidBodyManager::AddRigidBodyToPool(RigidBody* rb)
{
    for (int i = 0; i < m_poolSize; ++i)
    {
        if (m_rigidBodies[i] == nullptr)
        {
            m_rigidBodies[i] = rb;
            m_rbStates[i] = false;
            return;
        }
    }
}

// This allows you to find the RigidBody within an index
RigidBody* RigidBodyManager::GetPooledRigidBody(size_t index)
{
    if (index < 0 || index >= m_poolSize)
    {
        return nullptr;
    }

    return m_rigidBodies[index];
}

// This allows you to find the next nearest unpulled RigidBody in the array
// Returns a pointer to the Rigidbody allowing you to check its calculations
RigidBody* RigidBodyManager::GetPooledRigidBody()
{
    for (int i = 0; i < m_poolSize; ++i)
    {
        size_t index = (m_currentIndex + static_cast<size_t>(i)) % m_poolSize; // Circular index so that we start from the index and loops back until index-1
        if (!m_rbStates[index] && m_rigidBodies[index] != nullptr)
        {
            m_rbStates[index] = true; // This RigidBody is now pulled and allowed to be done work with or calculations with
            m_currentIndex = index;
            return m_rigidBodies[index];
        }
    }
    return nullptr;
}

// This function allows you to return a RigidBody that was pulled previously
// back to the Manager, which means that it is not being calculated on anymore
void RigidBodyManager::ReturnPooledRigidBody(RigidBody* rb)
{
    for (int i = 0; i < m_poolSize; ++i)
    {
        if (m_rigidBodies[i] == rb)
        {
            m_rbStates[i] = false;
            return;
        }
    }
}


// END OF RIGIDBODYMANAGER MEMBER FUNCTIONS
// =======================================================

// Check collisions between two RigidBodies
bool CheckCollisions(RigidBody* bodyA, RigidBody* bodyB, Vec2<float>& normal_out, float& depth_out) {
    normal_out.x = 0.f; // Make it zeroed first, in case of any values beforehand
    normal_out.y = 0.f;
    depth_out = 0.f; // Zeroed in case of previous values

    if (bodyA->mShapeType == BOX)
    {
        if (bodyB->mShapeType == BOX)
        {
            // BOX-BOX
            return CollisionIntersection_BoxBox(bodyA->obj_aabb, bodyA->mVelocity, bodyB->obj_aabb, bodyB->mVelocity);
        }
        else if (bodyB->mShapeType == CIRCLE) {
            // BOX-CIRCLE
            // return CollisionIntersection_BoxCircle(bodyA, bodyB, normal_out, depth_out);
        }
    }
    if (bodyA->mShapeType == CIRCLE)
    {
        if (bodyB->mShapeType == BOX)
        {
            // CIRCLE-BOX
            // return CollisionIntersection_BoxCircle(bodyB, bodyA, normal_out, depth_out);
        }
        if (bodyB->mShapeType == CIRCLE)
        {
            // CIRCLE-CIRCLE
            return CollisionIntersection_CircleCircle(bodyA->mPosition, bodyB->mPosition, bodyA->mRadius, bodyB->mRadius, normal_out, depth_out);
        }
    }
    return false;
}

void ResolveCollisions(RigidBody* bodyA, RigidBody* bodyB, Vec2<float> normal, float depth) {
    // Need to get relative velocity from A to B
    // Due to normal being from A to B
    Vec2<float> relativeVelocity = bodyB->mVelocity - bodyA->mVelocity;

    // If the dot product of relVel and normal is more than 0.f
    if (PHY_MATH::DotProduct(relativeVelocity, normal) > 0.f) {
        return;
    }

    float e = PHY_MATH::FindMin(bodyA->mRestitution, bodyB->mRestitution);

    float j = -(1.f + e) * PHY_MATH::DotProduct(relativeVelocity, normal);
    j /= bodyA->mInvMass + bodyB->mInvMass;

    // Magnitude * Normal to get the impulse given to the objects
    Vec2<float> impulse = normal * j;

    bodyA->mVelocity -= impulse * bodyA->mInvMass;
    bodyB->mVelocity += impulse * bodyB->mInvMass;
}