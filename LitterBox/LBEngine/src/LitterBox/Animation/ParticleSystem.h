/*!************************************************************************
 \file				ParticleSystem.h
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


#include "LitterBox/Utils/Math.h"
#include "LitterBox/Components/ParticleComponent.h"
#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include <cstdlib>
#include <ctime>
#include "LitterBox/Engine/Time.h"
#include "LitterBox/Serialization/AssetManager.h"
#include "LitterBox/Core/Core.h"

#pragma once

namespace LB {
	/*!***********************************************************************
	\brief
	This is a container that stores all the information needed for a particle
	*************************************************************************/
	class Particle {
	public:
		Vec2<float> mPosition;
		Vec2<float> mVelocity;
		float mRotation;
		float mRotationSpeed;

		float mSize;
		float mSizeBegin;
		float mSizeEnd;

		float mLifetime;
		float mLifetimeRemaining;

		float mLifetimeDelay;


		GameObject* mGameObj;

		bool mIsActive;
	};

	class ParticleManager : public ISystem, public Singleton<ParticleManager>
	{
	public:

		// ISystem stuff
		/*!***********************************************************************
		\brief
		Initialises the ParticleManager class by making sure all the indexes for
		the EmitterPool and the ParticlePool are correct as well as the
		types of emitters
		*************************************************************************/
		void Initialize() override;
		/*!***********************************************************************
		\brief
		Updates all the particles and emitters if the game is not paused and the
		editor is running
		*************************************************************************/
		void Update() override;
		/*!***********************************************************************
		\brief
		this function spawns the particles that will be used from the ParticlePool,
		either a Trail Particle emission or a Radial Particle emission, using
		the stats of the emitter provided
		*************************************************************************/
		void Destroy() override;
		/*!***********************************************************************
		\brief
		this function spawns the particles that will be used from the ParticlePool,
		either a Trail Particle emission or a Radial Particle emission, using
		the stats of the emitter provided
		*************************************************************************/
		void Emit(CPParticle* emitter);
		/*!***********************************************************************
		\brief
		Returns all of the string and emitter type pairs
		*************************************************************************/
		std::vector<std::pair<std::string, EmitterType>>& GetVectorEmits();
		/*!***********************************************************************
		\brief
		Gets a string for the particle type
		*************************************************************************/
		std::string GetEmitterType(EmitterType EmitType);
		/*!***********************************************************************
		\brief
		Adds an emitter to the emitter pool for later use to allow for the emission
		of particles from that emitter
		*************************************************************************/
		void AddEmitter(CPParticle* emitter);
		/*!***********************************************************************
		\brief
		Removes an emitter from the emitter pool
		*************************************************************************/
		void RemoveEmitter(CPParticle* emitter);
		/*!***********************************************************************
		\brief
		Updates all the particles that are active in the particle pool, updating
		its size and positions
		*************************************************************************/
		void UpdateParticles();
		/*!***********************************************************************
		\brief
		Updates all the emitters within the emitter pool
		*************************************************************************/
		void UpdateEmitters();
		/*!***********************************************************************
		\brief
		Makes sure all the particles are nullptr when the editor is not in play
		mode
		*************************************************************************/
		void WhenCoreNotPlaying();

		void CreateParticle(Vec2<float> pos, float lifetime, 
			float varXMin, float varXMax,
			float varYMin, float varYMax,
			float sizeBegin, float sizeEnd,
			std::string textureName
			);
	private:

		// Postion, Velocity, Variation, Texture, SizeBegin

		std::array<CPParticle*, 100> mEmitterPool{};
		std::array<Particle, 1000> mParticlePool{};
		int mEmitterPoolIndex{};
		int mParticlePoolIndex{};

		std::vector<std::pair<std::string, EmitterType>> mEmitterTypes{};
	};

	void ClearPool(Scene* newScene);

	/*!***********************************************************************
	\brief
	Returns a random float from the given min and max
	*************************************************************************/
	float RandomRange(float min, float max);
}
