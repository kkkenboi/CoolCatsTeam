#pragma once

#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "PhysicsMath.h"
#include "LitterBox/Engine/Time.h"

namespace LB
{
    RigidBodyManager* PHYSICS = nullptr;

    RigidBodyManager::RigidBodyManager()
    {
        //INPUT->SubscribeToKey(func, KeyCode::KEY_W, KeyEvent)


        if (!PHYSICS)
        {
            PHYSICS = this;
        }
        else
        {
            std::cerr << "Physics already exists\n";
        }

        // NEED TO DEFINE A POOL SIZE ANOTHER TIME INSTEAD
        // OF HARDCODING IT HERE
        constexpr int POOL_SIZE = 3000;
        m_poolSize = POOL_SIZE;
        m_currentIndex = 0;
        m_rigidBodies = new CPRigidBody * [3000];
        m_rbStates = new bool[3000];

        // Initialize all the RigidBody States to false
        for (int i = 0; i < POOL_SIZE; ++i)
        {
            m_rigidBodies[i] = nullptr; // All rigidbodies in arr empty first
            m_rbStates[i] = false; // Everything is not in use
        }

        std::cout << "Physics initialized\n";
    }

    RigidBodyManager::RigidBodyManager(int size) : m_poolSize(size), m_currentIndex(0)
    {
        m_rigidBodies = new CPRigidBody * [size];
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
    void RigidBodyManager::AddRigidBodyToPool(CPRigidBody* rb)
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
    CPRigidBody* RigidBodyManager::GetPooledRigidBody(size_t index)
    {
        if (index < 0 || index >= m_poolSize)
        {
            return nullptr;
        }

        return m_rigidBodies[index];
    }

    // This allows you to find the next nearest unpulled RigidBody in the array
    // Returns a pointer to the Rigidbody allowing you to check its calculations
    CPRigidBody* RigidBodyManager::GetPooledRigidBody()
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
    void RigidBodyManager::ReturnPooledRigidBody(CPRigidBody* rb)
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

    void RigidBodyManager::RBSystemSteps()
    {
        //std::cout << "JOE IS RBSYSTEM\n";
        // ==================
        // Movement Step
        // ==================
        for (size_t i = 0; i < m_poolSize; ++i)
        {
            if (m_rigidBodies[i] != nullptr)
            {
                m_rigidBodies[i]->FixedUpdate();
            }
        }

        // ==================
        // Collision Step
        // ==================
        LB::Vec2<float> normal_out{ 0.f , 0.f };
        float depth_out{ 0.f };

        for (size_t i = 0; i < m_poolSize; ++i)
        {
            //std::cout << "JOE IS HERE\n";
            CPRigidBody* bodyA = m_rigidBodies[i];

            for (size_t j = i + 1; j < m_poolSize; ++j)
            {
                //std::cout << "JOE IS IN SECOND BODY\n";
                CPRigidBody* bodyB = m_rigidBodies[j];

                if (bodyA == bodyB)
                {
                    continue;
                }

                // If either rigidBody is not instantiated
                if (bodyA == nullptr || bodyB == nullptr)
                {
                    continue;
                }

                // If both bodies are static
                if (bodyA->isStatic == true && bodyB->isStatic == true)
                {
                    continue;
                }

                // Normal here is moving B away from A
                if (CheckCollisions(bodyA, bodyB, normal_out, depth_out))
                {
                    //std::cout << "JOE HIT\n";
                    //std::cout << "BodyA Prev POS: " << bodyA->mPosition.x << " , " << bodyA->mPosition.y << std::endl;
                    //std::cout << "BodyB Prev POS: " << bodyB->mPosition.x << " , " << bodyB->mPosition.y << std::endl;

                    // Debug View
                    DEBUG->DrawBox(bodyA->mPosition, bodyA->mWidth, bodyA->mHeight, Vec4<float>{ 1.0f, 0.f, 0.f, 0.f });
                    DEBUG->DrawBox(bodyB->mPosition, bodyB->mWidth, bodyB->mHeight, Vec4<float>{ 1.0f, 0.f, 0.f, 0.f });

                    LB::Vec2<float>inverse_normal{ -normal_out.x, -normal_out.y };
                    if (bodyA->isStatic)
                    {
                        bodyB->Move(normal_out * depth_out);
                    }
                    else if (bodyB->isStatic)
                    {
                        bodyA->Move(inverse_normal * depth_out);
                    }
                    else
                    {
                        bodyA->Move(inverse_normal * depth_out);
                        bodyB->Move(normal_out * depth_out);
                    }
                    /*
                    ResolveCollisions(bodyA, bodyB, normal_out, depth_out);
                    */
                    //std::cout << "COLLISION RESOLVED" << std::endl;
                    //std::cout << "BodyA After POS: " << bodyA->mPosition.x << " , " << bodyA->mPosition.y << std::endl;
                    //std::cout << "BodyB After POS: " << bodyB->mPosition.x << " , " << bodyB->mPosition.y << std::endl;
                }


            }

        }
    }
    // ======================================
    // ISystem function overrides
    // ======================================
    void RigidBodyManager::Initialize()
    {
        SetSystemName("Physics System");
    }

    void RigidBodyManager::FixedUpdate()
    {
        RBSystemSteps();
    }

    // END OF RIGIDBODYMANAGER MEMBER FUNCTIONS
    // =======================================================

    // Check collisions between two RigidBodies
    // Normal is pushing bodyB away from bodyA
    bool CheckCollisions(CPRigidBody* bodyA, CPRigidBody* bodyB, LB::Vec2<float>& normal_out, float& depth_out) {
        normal_out.x = 0.f; // Make it zeroed first, in case of any values beforehand
        normal_out.y = 0.f;
        depth_out = 0.f; // Zeroed in case of previous values

        if (bodyA->mShapeType == BOX)
        {
            //std::cout << "JOE IS BOX\n";
            if (bodyB->mShapeType == BOX)
            {
                //std::cout << "BOTH JOES ARE BOXES\n";
                // A - B
                // BOX-BOX
                return CollisionIntersection_BoxBox_SAT(bodyA->mTransformedVertices, bodyB->mTransformedVertices, normal_out, depth_out);
                //return CollisionIntersection_BoxBox(bodyA->obj_aabb, bodyA->mVelocity, bodyB->obj_aabb, bodyB->mVelocity, TIME->GetFixedDeltaTime(), normal_out, depth_out);
            }
            else if (bodyB->mShapeType == CIRCLE) {
                // A - B
                // BOX-CIRCLE
                // normal here is pushing B away from A
                bool result = CollisionIntersection_CircleBox_SAT(bodyB->mPosition, bodyB->mRadius, bodyA->mTransformedVertices, normal_out, depth_out);
                // The normal given is inverse bodyB is circle and bodyA is box
                // So normal given is circle away from box, therefore need to inverse it
                normal_out.x = -normal_out.x;
                normal_out.y = -normal_out.y;
                return result;
            }
        }
        if (bodyA->mShapeType == CIRCLE)
        {
            if (bodyB->mShapeType == BOX)
            {
                // A - B
                // CIRCLE-BOX
                // normal here is pushing box away from circle
                bool result = CollisionIntersection_CircleBox_SAT(bodyA->mPosition, bodyA->mRadius, bodyB->mTransformedVertices, normal_out, depth_out);
                return result;
            }
            if (bodyB->mShapeType == CIRCLE)
            {
                // A - B 
                // CIRCLE-CIRCLE
                return CollisionIntersection_CircleCircle(bodyA->mPosition, bodyB->mPosition, bodyA->mRadius, bodyB->mRadius, normal_out, depth_out);
            }
        }
        return false;
    }

    void ResolveCollisions(CPRigidBody* bodyA, CPRigidBody* bodyB, LB::Vec2<float> normal, float depth) {
        // Need to get relative velocity from A to B
        // Due to normal being from A to B
        //std::cout << "bodyA vel: " << bodyA->mVelocity.x << " , " << bodyA->mVelocity.y << std::endl;
        //std::cout << "bodyB vel: " << bodyB->mVelocity.x << " , " << bodyB->mVelocity.y << std::endl;
        LB::Vec2<float> relativeVelocity = bodyB->mVelocity - bodyA->mVelocity;

        // If the dot product of relVel and normal is more than 0.f
        if (PHY_MATH::DotProduct(relativeVelocity, normal) > 0.f) {
            return;
        }

        float e = PHY_MATH::FindMin(bodyA->mRestitution, bodyB->mRestitution);

        float j = -(1.f + e) * PHY_MATH::DotProduct(relativeVelocity, normal);
        j /= bodyA->mInvMass + bodyB->mInvMass;

        // Magnitude * Normal to get the impulse given to the objects
        LB::Vec2<float> impulse = normal * j;

        bodyA->mVelocity -= impulse * bodyA->mInvMass;
        bodyB->mVelocity += impulse * bodyB->mInvMass;

        //std::cout << "bodyA vel: " << bodyA->mVelocity.x << " , " << bodyA->mVelocity.y << std::endl;
        //std::cout << "bodyB vel: " << bodyB->mVelocity.x << " , " << bodyB->mVelocity.y << std::endl;
    }

    void MoveUp()
    {

        //std::cout << "IN MOVEUP" << std::endl;
        CPRigidBody* temp_mainchar;
        if (temp_mainchar = PHYSICS->GetPooledRigidBody(0))
        {
            //std::cout << "TRYING TO MOVEUP" << std::endl;
            temp_mainchar->addForce(Vec2<float>{0.f, 10.f});
            //std::cout <<  "MC Vel: " << temp_mainchar->mVelocity.x << " , " << temp_mainchar->mVelocity.y << std::endl;
        }
    }

    void MoveDown()
    {
        CPRigidBody* temp_mainchar;
        if (temp_mainchar = PHYSICS->GetPooledRigidBody(0))
        {
            temp_mainchar->addForce(Vec2<float>{0.f, -10.f});
        }
    }

    void MoveLeft()
    {
        CPRigidBody* temp_mainchar;
        if (temp_mainchar = PHYSICS->GetPooledRigidBody(0))
        {
            temp_mainchar->addForce(Vec2<float>{-10.f, 0.f});
        }
    }

    void MoveRight()
    {
        CPRigidBody* temp_mainchar;
        if (temp_mainchar = PHYSICS->GetPooledRigidBody(0))
        {
            temp_mainchar->addForce(Vec2<float>{10.f, 0.f});
        }
    }

} // Namespace LB