#include "RigidBody.h"

RigidBody::RigidBody(Vec2<float> position, Vec2<float> prevposition, Vec2<float> velocity, Vec2<float> acceleration,
    float rotation, float rotationvelocity, float density, float mass, float invmass, float restitution,
    float area, float friction, bool isstatic, bool isactive, float radius, float width, float height,
    SHAPETYPE shape)
{
    this->mPosition = position;
    this->mPrevPosition = prevposition;

    this->mVelocity = velocity;
    this->mAcceleration = acceleration;

    this->mRotation = rotation;
    this->mRotationalVelocity = rotationvelocity;

    this->mDensity = density;
    this->mMass = mass;
    this->mInvMass = invmass;
    this->mRestitution = restitution;
    this->mArea = area;

    this->mFriction = friction;
    this->isStatic = isstatic;
    this->isActive = isactive;

    this->mRadius = radius;
    this->mWidth = width;
    this->mHeight = height;

    this->mShapeType = shape;

    // Check if static and update the InvMass
    if (!this->isStatic)
    {
        this->mInvMass = 1.f / this->mMass;
    }
    else
    {
        this->mInvMass = 0.f;
    }

    // Check if Box
    if (this->mShapeType == BOX)
    {
        // If it is a box then create the vertices
        // as well as the AABB data
        obj_aabb.m_c = this->mPosition;
        // Max is top right
        obj_aabb.m_max.x = this->mWidth / 2 + this->mPosition.x;
        obj_aabb.m_max.y = this->mHeight / 2 + this->mPosition.y;
        // Min is bottom right
        obj_aabb.m_min.x = this->mPosition.x - this->mWidth / 2;
        obj_aabb.m_min.y = this->mPosition.y - this->mHeight / 2;

        // Create the vertices
        // Untransformed
        CreateBoxVertices(this->mVertices, this->mWidth, this->mHeight);
        // Transformed
        this->UpdateRigidBodyBoxVertices();

    }
    else
    {
        Vec2<float> zeroed;
        zeroed.x = 0.f;
        zeroed.y = 0.f;

        obj_aabb.m_c = zeroed;

        obj_aabb.m_min = zeroed;

        obj_aabb.m_max = zeroed;


        mVertices[0] = zeroed;
        mVertices[1] = zeroed;
        mVertices[2] = zeroed;
        mVertices[3] = zeroed;

        mTransformedVertices[0] = zeroed;
        mTransformedVertices[1] = zeroed;
        mTransformedVertices[2] = zeroed;
        mTransformedVertices[3] = zeroed;

    }
}


void RigidBody::addForce(Vec2<float> force)
{
    this->mVelocity += force;
}

void RigidBody::Move(Vec2<float> vec)
{
    this->mPosition += vec;
}

void RigidBody::MoveTo(Vec2<float> position)
{
    this->mPosition = position;
}

void RigidBody::UpdateRigidBodyBoxVertices() 
{
    PhysicsTransform transform{ this->mPosition, this->mRotation };

    for (int i = 0; i < 4; ++i) {
        // Uses the untransformed vertices as the basis for tranasformation
        Vec2<float> og_vec = this->mVertices[i];
        // Transforming the vertices using trigo formulas
        this->mTransformedVertices[i] = Vec2<float>{
            transform.m_cos * og_vec.x - transform.m_sin * og_vec.y + transform.m_posX,
            transform.m_sin * og_vec.x - transform.m_cos * og_vec.y + transform.m_posY };
    }
}

void RigidBody::UpdateRigidBodyAABB() 
{
    float minX = 100000.f;
    float maxX = -100000.f;
    float minY = 100000.f;
    float maxY = -100000.f;

    if (this->mShapeType == BOX)
    {
        for (int i = 0; i < 4; ++i) 
        {
            // Take the Transformed Vertices and use it as the new AABB
            Vec2<float> vec = this->mTransformedVertices[i];

            if (vec.x < minX) minX = vec.x;
            if (vec.x > maxX) maxX = vec.x;
            if (vec.y < minY) minY = vec.y;
            if (vec.y > maxY) maxY = vec.y;
        }
    }
    else if (this->mShapeType == CIRCLE) 
    {
        // Basically grab the position and make a box using radius as the width and height
        // of the box
        minX = this->mPosition.x - this->mRadius;
        maxX = this->mPosition.x + this->mRadius;
        minY = this->mPosition.y - this->mRadius;
        maxY = this->mPosition.y + this->mRadius;
    }

    this->obj_aabb.m_c = Vec2<float>{ (minX + maxX) / 2.f, (minY + maxY) / 2.f };
    this->obj_aabb.m_max = Vec2<float>{ minX, minY };
    this->obj_aabb.m_min = Vec2<float>{ maxX, maxY };
}

void RigidBody::UpdateRigidBodyPos(float time)
{
    // If static do not move or update the position of the RigidBody
    if (this->isStatic)
    {
        return;
    }

    this->mPosition += this->mVelocity * time;

    this->mRotation += this->mRotationalVelocity * time;

    // Update the TransformedVertices
    // HERE
    this->UpdateRigidBodyBoxVertices();

    // Update the AABB
    // HERE
    this->UpdateRigidBodyAABB();
}

void RigidBody::UpdateRigidBodyVel(float time) 
{
    this->mVelocity *= this->mFriction;
    Vec2<float> zeroed;
    zeroed.x = 0.f;
    zeroed.y = 0.f;
    this->mAcceleration = zeroed;
}

// END OF RIGIDBODY MEMBER FUNCTIONS
// ===========================================

void CreateBoxVertices(Vec2<float>* vertices_arr , float width, float height) 
{
    float left = -width / 2;
    float right = width / 2;
    float top = height / 2;
    float bottom = -height / 2;

    
    vertices_arr[0].x = left;
    vertices_arr[0].y = top;

    vertices_arr[1].x = right;
    vertices_arr[1].y = top;

    vertices_arr[2].x = right;
    vertices_arr[2].y = bottom;

    vertices_arr[3].x = left;
    vertices_arr[3].y = bottom;
}

void CreateBoxVerticesTransformed(Vec2<float>* vertices_arr, Vec2<float> position) 
{
    vertices_arr[0] += position;
    vertices_arr[1] += position;
    vertices_arr[2] += position;
    vertices_arr[3] += position;
}