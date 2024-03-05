/*!************************************************************************
 \file				ParticleComponent.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2451A
 \date				09-02-2024
 \brief
  This file contains the ParticleManager class which contains
  all the implementation of the class' functions. It handles
  the Particle Pool and the Emitter Pool by updating the
  positions and stats of Particles that were emitted by an
  Emitter.

  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "ParticleComponent.h"
#include "Litterbox/Animation/ParticleSystem.h"
#include "LitterBox/Factory/GameObjectManager.h"

namespace LB {
	/*!***********************************************************************
	\brief
	Initialises some of the variables in the CPParticle class as well
	as adding the newly created CPParticle into the Emitter Pool within
	the Particle Manager
	*************************************************************************/
	void CPParticle::Initialise() 
	{
		mTransform = gameObj->GetComponent<CPTransform>();

		if (gameObj->HasComponent<CPRender>()) 
		{
			mRender = gameObj->GetComponent<CPRender>();
		}

		mEmitterType = TRAIL;
		
		mEmitterPos = mTransform->GetPosition();

		mEmitterLifetimeRemaining = mEmitterLifetime;

		ParticleManager::Instance()->AddEmitter(this);
	}

	/*!***********************************************************************
		\brief
		Updates the CPParticle's position and other stats
	*************************************************************************/
	void CPParticle::Update()
	{
		// Check if deletion will happen
		if (mWillDelete) 
		{
			if (mEmitterDeletionLifetimeRemaining <= 0.f) 
			{
				Destroy();
				GOMANAGER->RemoveGameObject(this->gameObj);
			}
			else 
			{
				mEmitterDeletionLifetimeRemaining -= TIME->GetDeltaTime();
			}
		}

		if (gameObj->HasComponent<CPRender>())
		{
			mRender = gameObj->GetComponent<CPRender>();
		}

		mEmitterPos = mTransform->GetPosition();
		
		if (mEmitterLifetimeRemaining > 0.f && mIsActive)
		{
			if (mIsLooping == false)
			{
				mEmitterLifetimeRemaining -= static_cast<float>(TIME->GetDeltaTime());
			}
			mTimeSinceLastEmit += static_cast<float>(TIME->GetDeltaTime());
		}

		if (mEmitterLifetimeRemaining <= 0.f) 
		{
			mIsActive = false;
			mEmitterLifetimeRemaining = mEmitterLifetime;
		}

		if (mIsActive)
		{
			if (mTimeSinceLastEmit >= mEmitterRate) {
				if (mRender != nullptr) {
					ParticleManager::Instance()->Emit(this);
				}
				mTimeSinceLastEmit = 0.f;
			}
		}
	}

	/*!***********************************************************************
		\brief
		Removes the emitter from the Emitter Pool in the ParticleManager class
	*************************************************************************/
	void CPParticle::Destroy()
	{
		// Remove it from the EmitterPool
		mTransform = nullptr;
		mRender = nullptr;
		ParticleManager::Instance()->RemoveEmitter(this);
	}

	/*!***********************************************************************
		\brief
		Gets the ComponentTypeID of the CPParticle class
	*************************************************************************/
	ComponentTypeID CPParticle::GetType() 
	{
		return C_CPParticle;
	}

	/*!***********************************************************************
		\brief
		Serializes the CPParticle component
	*************************************************************************/
	bool CPParticle::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		data.AddMember("EmitterType", static_cast<int>(mEmitterType), alloc);
		data.AddMember("EmitterRate", mEmitterRate, alloc);
		data.AddMember("RadialNum", mRadialParticles, alloc);
		Value VelocityValue;
		if (mEmitterVelocity.Serialize(VelocityValue, alloc))
		{
			data.AddMember("EmitterVelocity", VelocityValue, alloc);
		}
		data.AddMember("VelocityMinX", mEmitterVariationMinX, alloc);
		data.AddMember("VelocityMaxX", mEmitterVariationMaxX, alloc);
		data.AddMember("VelocityMinY", mEmitterVariationMinY, alloc);
		data.AddMember("VelocityMaxY", mEmitterVariationMaxY, alloc);
		data.AddMember("RadialSpeed", mEmitterRadialSpeed, alloc);
		data.AddMember("SizeBegin", mEmitterSizeBegin, alloc);
		data.AddMember("SizeEnd", mEmitterSizeEnd, alloc);
		data.AddMember("EmitterLifetime", mEmitterLifetime, alloc);
		data.AddMember("ParticleLifetime", mParticleLifetime, alloc);
		data.AddMember("EmitterActive", mIsActive, alloc);
		data.AddMember("EmitterLooping", mIsLooping, alloc);

		return true;
	}

	/*!***********************************************************************
		\brief
		Deserializes the CPParticle component
	*************************************************************************/
	bool CPParticle::Deserialize(const Value& data)
	{
		mEmitterType = static_cast<EmitterType>(data["EmitterType"].GetInt());
		mEmitterRate = data["EmitterRate"].GetFloat();
		mRadialParticles = data["RadialNum"].GetInt();
		const Value& velocityValue = data["EmitterVelocity"];
		mEmitterVelocity.Deserialize(velocityValue);
		mEmitterVariationMinX = data["VelocityMinX"].GetFloat();
		mEmitterVariationMaxX = data["VelocityMaxX"].GetFloat();
		mEmitterVariationMinY = data["VelocityMinY"].GetFloat();
		mEmitterVariationMaxY = data["VelocityMaxY"].GetFloat();
		mEmitterRadialSpeed = data["RadialSpeed"].GetFloat();
		mEmitterSizeBegin = data["SizeBegin"].GetFloat();
		mEmitterSizeEnd = data["SizeEnd"].GetFloat();
		mEmitterLifetime = data["EmitterLifetime"].GetFloat();
		mParticleLifetime = data["ParticleLifetime"].GetFloat();
		mIsActive = data["EmitterActive"].GetBool();
		mIsLooping = data["EmitterLooping"].GetBool();

		return true;
	}

	/*!***********************************************************************
		\brief
		Returns the current emitter type of the CPParticle component as a string
	*************************************************************************/
	std::string CPParticle::GetEmitterType()
	{
		switch (mEmitterType)
		{
		case TRAIL:
			return "Trail";
		case RADIAL:
			return "Radial";
		default:
			return "ERROR!";
		}
	}

	/*!***********************************************************************
		\brief
		Restarts the lifetime remaining of the CPParticle component
	*************************************************************************/
	void CPParticle::RestartLifetime() 
	{
		mEmitterLifetimeRemaining = mEmitterLifetime;
	}

	void CPParticle::SetDeletion(bool state)
	{
		mWillDelete = state;
	}

	void CPParticle::SetDeletionTime(float time) 
	{
		mEmitterDeletionLifetime = time;
		mEmitterDeletionLifetimeRemaining = mEmitterDeletionLifetime;
	}
}