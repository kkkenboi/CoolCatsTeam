/*!************************************************************************
 \file				ParticleSystem.cpp
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


#include "ParticleSystem.h"
#include "LitterBox/Scene/SceneManager.h"

namespace LB
{
	/*!***********************************************************************
	\brief
	Initialises the ParticleManager class by making sure all the indexes for
	the EmitterPool and the ParticlePool are correct as well as the
	types of emitters
	*************************************************************************/
	void ParticleManager::Initialize() 
	{
		SetSystemName("Particle System");
		mEmitterPoolIndex = static_cast<int>(mEmitterPool.size()) - 1;
		mParticlePoolIndex = static_cast<int>(mParticlePool.size()) - 1;

		SCENEMANAGER->onNewSceneLoad.Subscribe(ClearPool);

		const EmitterType EmitterTypeVector[] = { TRAIL, RADIAL, INVERSERADIAL };
		for (EmitterType emit : EmitterTypeVector)
		{
			mEmitterTypes.emplace_back(std::make_pair(ParticleManager::Instance()->GetEmitterType(emit), emit));
		}
	}

	/*!***********************************************************************
	\brief
	this function spawns the particles that will be used from the ParticlePool,
	either a Trail Particle emission or a Radial Particle emission, using
	the stats of the emitter provided
	*************************************************************************/
	void ParticleManager::Emit(CPParticle* emitter) 
	{

		if (emitter->mEmitterType == TRAIL)
		{
			// Gets a particle from the particle pool to instantiate
			Particle& particle = mParticlePool[mParticlePoolIndex];
			particle.mIsActive = true;
			particle.mPosition = emitter->mEmitterPos;
			particle.mRotation = 0.f;
			particle.mRotationSpeed = emitter->mRotationSpeed;
			particle.mRotationSpeed += RandomRange(emitter->mRotationSpeedVariationMin, emitter->mRotationSpeedVariationMax);

			// Velocity
			particle.mVelocity = emitter->mEmitterVelocity;
			particle.mVelocity.x += RandomRange(emitter->mEmitterVariationMinX, emitter->mEmitterVariationMaxX);
			particle.mVelocity.y += RandomRange(emitter->mEmitterVariationMinY, emitter->mEmitterVariationMaxY);

			// Texture

			// Lifetime
			particle.mLifetime = emitter->mParticleLifetime;
			if (!emitter->mParticleLifetimeRandomness)
			{
				particle.mLifetime += RandomRange(emitter->mParticleLifetimeMin, emitter->mParticleLifetimeMax);
				// Clamp the particle lifetime to 0, as we don't want negative numbers
				if (particle.mLifetime < 0.f)
				{
					particle.mLifetime = 0.f;
				}
			}
			particle.mLifetimeRemaining = emitter->mParticleLifetime;

			// Particle's lifetime delay to show visible
			particle.mLifetimeDelay = RandomRange(emitter->mEmitterLifetimeDelayMin, emitter->mEmitterLifetimeDelayMax);

			// Size
			particle.mSizeBegin = emitter->mEmitterSizeBegin;
			particle.mSize = particle.mSizeBegin;
			particle.mSizeEnd = emitter->mEmitterSizeEnd;


			// Create a GameObject that follows the Particle's current stats
			particle.mGameObj = FACTORY->SpawnGameObject();
			particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
			particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSizeBegin, particle.mSizeBegin});
			particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
			particle.mGameObj->GetComponent<CPRender>()->Initialise();

			// Make the render be invisible if delay is 0>
			if (particle.mLifetimeDelay > 0.f)
			{
				particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(false);
			}
			else 
			{
				particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(true);
			}
			particle.mGameObj->GetComponent<CPRender>()->z_val = emitter->mRender->z_val;

			// Get the texture ID from the emitter

			//First we check if it's using a sprite from a sprite sheet. 
			//Since -1 means we're using a texture and not a sprite sheet,
			if (emitter->mRender->spriteIndex < 0)
			{	//We have to do the standard grabbing of texture from the asset manager stuff
				int textureID = emitter->mRender->texture;
				std::string textureName = ASSETMANAGER->GetTextureName(textureID);
				particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));
			}
			else
			{	//if we do happen to have a sprite index, this means we're using the sprite sheet so
				//we just use the CPRender function to set sprite texture
				particle.mGameObj->GetComponent<CPRender>()->SetSpriteTexture(emitter->mRender->spriteSheetName, emitter->mRender->spriteIndex);
			}
			mParticlePoolIndex = (mParticlePoolIndex - 1 + static_cast<int>(mParticlePool.size())) % static_cast<int>(mParticlePool.size());
		}
		else if (emitter->mEmitterType == RADIAL)
		{
			// Get however many radial num we got
			for (int i = 0; i < emitter->mRadialParticles; ++i)		
			{
				// Get particle from the particle pool
				Particle& particle = mParticlePool[mParticlePoolIndex];
				particle.mIsActive = true;
				particle.mPosition = emitter->mEmitterPos;
				particle.mRotation = 0.f;
				particle.mRotationSpeed = emitter->mRotationSpeed;
				particle.mRotationSpeed += RandomRange(emitter->mRotationSpeedVariationMin, emitter->mRotationSpeedVariationMax);

				// Velocity (calculated in radial form)
				float angle = static_cast<float>(i) / static_cast<float>(emitter->mRadialParticles) * 2.0f * static_cast<float>(PI);
				particle.mVelocity.x = emitter->mEmitterRadialSpeed * cos(angle);
				particle.mVelocity.y = emitter->mEmitterRadialSpeed * sin(angle);

				// Lifetime
				particle.mLifetime = emitter->mParticleLifetime;
				if (!emitter->mParticleLifetimeRandomness)
				{
					particle.mLifetime += RandomRange(emitter->mParticleLifetimeMin, emitter->mParticleLifetimeMax);
					// Clamp the particle lifetime to 0, as we don't want negative numbers
					if (particle.mLifetime < 0.f)
					{
						particle.mLifetime = 0.f;
					}
				}
				particle.mLifetimeRemaining = emitter->mParticleLifetime;

				// Particle's lifetime delay to show visible
				particle.mLifetimeDelay = RandomRange(emitter->mEmitterLifetimeDelayMin, emitter->mEmitterLifetimeDelayMax);


				// Size
				particle.mSizeBegin = emitter->mEmitterSizeBegin;
				particle.mSize = particle.mSizeBegin;
				particle.mSizeEnd = emitter->mEmitterSizeEnd;

				// Create a GameObject
				particle.mGameObj = FACTORY->SpawnGameObject();
				particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
				particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSizeBegin, particle.mSizeBegin});
				particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
				particle.mGameObj->GetComponent<CPRender>()->Initialise();

				// Make the render be invisible if delay is 0>
				if (particle.mLifetimeDelay > 0.f)
				{
					particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(false);
				}
				else
				{
					particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(true);
				}
				particle.mGameObj->GetComponent<CPRender>()->z_val = emitter->mRender->z_val;

				//First we check if it's using a sprite from a sprite sheet. 
				//Since -1 means we're using a texture and not a sprite sheet,
				if (emitter->mRender->spriteIndex < 0)
				{	//We have to do the standard grabbing of texture from the asset manager stuff
					int textureID = emitter->mRender->texture;
					std::string textureName = ASSETMANAGER->GetTextureName(textureID);
					particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));
				}
				else
				{	//if we do happen to have a sprite index, this means we're using the sprite sheet so
					//we just use the CPRender function to set sprite texture
					particle.mGameObj->GetComponent<CPRender>()->SetSpriteTexture(emitter->mRender->spriteSheetName, emitter->mRender->spriteIndex);
				}
				// Move to the next index in the particle pool
				mParticlePoolIndex = (mParticlePoolIndex - 1 + static_cast<int>(mParticlePool.size())) % static_cast<int>(mParticlePool.size());
			}
		}
		else if (emitter->mEmitterType == INVERSERADIAL) 
		{
			// Check if emitter has radial all at once spawning of the circle or not
			if (emitter->mEmitterRadialSpawnCircle)
			{
				// True means we spawn the whole circle at the outer rim and move towards the center
				// and then we despawn the particles
				for (int i = 0; i < emitter->mRadialParticles; ++i) 
				{
					// False means we spawn each particles randomly at the border of the circle
				// and make it move towards the center and despawn it once it reaches the center
					Particle& particle = mParticlePool[mParticlePoolIndex];
					particle.mIsActive = true;
					// Position is away from the center, distance is given using the variable mInvRadialDistance
					// and using a random angle
					float angle = RandomRange(0.f, 2.0f * PI);
					float distance = RandomRange(emitter->mInvRadDistanceMin, emitter->mInvRadDistanceMax);
					particle.mPosition.x = emitter->mEmitterPos.x + distance * cos(angle);
					particle.mPosition.y = emitter->mEmitterPos.y + distance * sin(angle);
					particle.mRotationSpeed = emitter->mRotationSpeed;
					particle.mRotationSpeed += RandomRange(emitter->mRotationSpeedVariationMin, emitter->mRotationSpeedVariationMax);
					particle.mLifetime = emitter->mParticleLifetime;

					if (!emitter->mParticleLifetimeRandomness)
					{
						particle.mLifetime += RandomRange(emitter->mParticleLifetimeMin, emitter->mParticleLifetimeMax);
						// Clamp the particle lifetime to 0, as we don't want negative numbers
						if (particle.mLifetime < 0.f)
						{
							particle.mLifetime = 0.f;
						}
					}

					particle.mLifetimeRemaining = particle.mLifetime;

					// Using this distance we want to calculate the velocity of it
					// from the lifetime that was given
					particle.mVelocity.x = (emitter->mEmitterPos.x - particle.mPosition.x) / particle.mLifetime;
					particle.mVelocity.y = (emitter->mEmitterPos.y - particle.mPosition.y) / particle.mLifetime;
					/*
					// Lifetime delay
					particle.mLifetimeDelay = RandomRange(emitter->mEmitterLifetimeDelayMin, emitter->mEmitterLifetimeDelayMax);
					*/
					// Size
					particle.mSizeBegin = emitter->mEmitterSizeBegin;
					particle.mSize = particle.mSizeBegin;
					particle.mSizeEnd = emitter->mEmitterSizeEnd;


					// Create a GameObject that follows the Particle's current stats
					particle.mGameObj = FACTORY->SpawnGameObject();
					particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
					particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSizeBegin, particle.mSizeBegin});
					particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
					particle.mGameObj->GetComponent<CPRender>()->Initialise();

					// Make the render be invisible if delay is 0>
					if (particle.mLifetimeDelay > 0.f)
					{
						particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(false);
					}
					else
					{
						particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(true);
					}
					particle.mGameObj->GetComponent<CPRender>()->z_val = emitter->mRender->z_val;

					// Get the texture ID from the emitter

					//First we check if it's using a sprite from a sprite sheet. 
					//Since -1 means we're using a texture and not a sprite sheet,
					if (emitter->mRender->spriteIndex < 0)
					{	//We have to do the standard grabbing of texture from the asset manager stuff
						int textureID = emitter->mRender->texture;
						std::string textureName = ASSETMANAGER->GetTextureName(textureID);
						particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));
					}
					else
					{	//if we do happen to have a sprite index, this means we're using the sprite sheet so
						//we just use the CPRender function to set sprite texture
						particle.mGameObj->GetComponent<CPRender>()->SetSpriteTexture(emitter->mRender->spriteSheetName, emitter->mRender->spriteIndex);
					}
					// Update the index
					mParticlePoolIndex = (mParticlePoolIndex - 1 + static_cast<int>(mParticlePool.size())) % static_cast<int>(mParticlePool.size());
				}
			}
			else
			{
				// False means we spawn each particles randomly at the border of the circle
				// and make it move towards the center and despawn it once it reaches the center
				Particle& particle = mParticlePool[mParticlePoolIndex];
				particle.mIsActive = true;
				// Position is away from the center, distance is given using the variable mInvRadialDistance
				// and using a random angle
				float angle = RandomRange(0.f, 2.0f * PI);
				float distance = RandomRange(emitter->mInvRadDistanceMin, emitter->mInvRadDistanceMax);
				particle.mPosition.x = emitter->mEmitterPos.x + distance * cos(angle);
				particle.mPosition.y = emitter->mEmitterPos.y + distance * sin(angle);
				particle.mRotationSpeed = emitter->mRotationSpeed;
				particle.mRotationSpeed += RandomRange(emitter->mRotationSpeedVariationMin, emitter->mRotationSpeedVariationMax);
				particle.mLifetime = emitter->mParticleLifetime;

				if (emitter->mParticleLifetimeRandomness)
				{
					particle.mLifetime += RandomRange(emitter->mParticleLifetimeMin, emitter->mParticleLifetimeMax);
					// Clamp the particle lifetime to 0, as we don't want negative numbers
					if (particle.mLifetime < 0.f)
					{
						particle.mLifetime = 0.f;
					}
				}
				
				particle.mLifetimeRemaining = particle.mLifetime;

				Vec2<float> dir_to_center = Normalise(emitter->mEmitterPos - particle.mPosition);
				// Using this distance we want to calculate the velocity of it
				// from the lifetime that was given
				particle.mVelocity.x = dir_to_center.x * distance / particle.mLifetime;
				particle.mVelocity.y = dir_to_center.y * distance / particle.mLifetime;

				particle.mLifetimeDelay = 0.f;

				// Size
				particle.mSizeBegin = emitter->mEmitterSizeBegin;
				particle.mSize = particle.mSizeBegin;
				particle.mSizeEnd = emitter->mEmitterSizeEnd;

				// Create a GameObject that follows the Particle's current stats
				particle.mGameObj = FACTORY->SpawnGameObject();
				particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
				particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSizeBegin, particle.mSizeBegin});
				particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
				particle.mGameObj->GetComponent<CPRender>()->Initialise();

				// Make the render be invisible if delay is 0>
				if (particle.mLifetimeDelay > 0.f)
				{
					particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(false);
				}
				else
				{
					particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(true);
				}
				particle.mGameObj->GetComponent<CPRender>()->z_val = emitter->mRender->z_val;

				// Get the texture ID from the emitter

				//First we check if it's using a sprite from a sprite sheet. 
				//Since -1 means we're using a texture and not a sprite sheet,
				if (emitter->mRender->spriteIndex < 0)
				{	//We have to do the standard grabbing of texture from the asset manager stuff
					int textureID = emitter->mRender->texture;
					std::string textureName = ASSETMANAGER->GetTextureName(textureID);
					particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));
				}
				else
				{	//if we do happen to have a sprite index, this means we're using the sprite sheet so
					//we just use the CPRender function to set sprite texture
					particle.mGameObj->GetComponent<CPRender>()->SetSpriteTexture(emitter->mRender->spriteSheetName, emitter->mRender->spriteIndex);
				}

				// Update the index
				mParticlePoolIndex = (mParticlePoolIndex - 1 + static_cast<int>(mParticlePool.size())) % static_cast<int>(mParticlePool.size());

			}
		}
	}

	/*!***********************************************************************
	\brief
	Updates all the particles and emitters if the game is not paused and the
	editor is running
	*************************************************************************/
	void ParticleManager::Update()
	{
		if (!TIME->IsPaused() && CORE->IsPlaying())
		{
			UpdateParticles();
			UpdateEmitters();
		}
		WhenCoreNotPlaying();
	}

	/*!***********************************************************************
	\brief
	Makes sure that the EmitterPool all points to nullptrs once the
	ParticleManager is destroyed
	*************************************************************************/
	void ParticleManager::Destroy() 
	{
		// Loop through the EmitterPool and make them all nullptr
		for (int i = 0; i < mEmitterPool.size(); ++i) 
		{
			mEmitterPool[i] = nullptr;
		}
		for (Particle& particle : mParticlePool)
		{
			particle.mIsActive = false;
			if (particle.mGameObj != nullptr)
			{
				GOMANAGER->RemoveGameObject(particle.mGameObj);
			}
			particle.mGameObj = nullptr;
		}
	}

	/*!***********************************************************************
	\brief
	Gets a string for the particle type
	*************************************************************************/
	std::string ParticleManager::GetEmitterType(EmitterType EmitType)
	{
		switch (EmitType)
		{
		case TRAIL:
			return "Trail";
		case RADIAL:
			return "Radial";
		case INVERSERADIAL:
			return "InverseRadial";
		default:
			return "ERROR!";
		}
	}

	/*!***********************************************************************
	\brief
	Returns all of the string and emitter type pairs
	*************************************************************************/
	std::vector<std::pair<std::string, EmitterType>>& ParticleManager::GetVectorEmits() {
		return mEmitterTypes;
	}

	/*!***********************************************************************
	\brief
	Adds an emitter to the emitter pool for later use to allow for the emission
	of particles from that emitter
	*************************************************************************/
	void ParticleManager::AddEmitter(CPParticle* newEmitter) {
		// Loop through the mEmitterPool looking for the first nullptr and place the brand new
		// created emitter here
		for (int i = 0; i < mEmitterPool.size(); ++i) {
			if (mEmitterPool[i] == nullptr) {
				mEmitterPool[i] = newEmitter;
				return;
			}
		}
	}

	/*!***********************************************************************
	\brief
	Removes an emitter from the emitter pool
	*************************************************************************/
	void ParticleManager::RemoveEmitter(CPParticle* findEmitter) {
		for (int i = 0; i < mEmitterPool.size(); ++i) {
			if (mEmitterPool[i] == findEmitter) {
				mEmitterPool[i] = nullptr;
			}
		}
	}

	/*!***********************************************************************
	\brief
	Updates all the particles that are active in the particle pool, updating
	its size and positions
	*************************************************************************/
	void ParticleManager::UpdateParticles() {
		for (Particle& particle : mParticlePool)
		{
			// If particle is not active ignore it
			if (!particle.mIsActive)
			{
				continue;
			}
			//std::cout << particle.mLifetimeRemaining << std::endl;
			// Check if particle needs to be destroyed
			if (particle.mLifetimeRemaining <= 0.f)
			{
				particle.mIsActive = false;
				if (particle.mGameObj != nullptr)
				{
					GOMANAGER->RemoveGameObject(particle.mGameObj);
				}
				particle.mGameObj = nullptr;
				continue;
			}

			// Check if particle should be shown
			if (particle.mLifetimeDelay <= 0.f)
			{
				if (GOMANAGER->IsGameObjectInScene(particle.mGameObj))
				{
					if (particle.mGameObj->GetComponent<CPRender>()->m_active == false)
					{
						particle.mGameObj->GetComponent<CPRender>()->ToggleActiveFlag(true);
					}
				}

				// Update the particle stats
				particle.mLifetimeRemaining -= static_cast<float>(TIME->GetDeltaTime());
				particle.mPosition += particle.mVelocity * static_cast<float>(TIME->GetDeltaTime());
				particle.mRotation += particle.mRotationSpeed * static_cast<float>(TIME->GetDeltaTime());

				// Check if size is enlarging or getting smaller
				// Getting smaller
				if (particle.mSizeBegin > particle.mSizeEnd) {
					// Getting smaller
					float sizeChangePerSecond = (particle.mSizeBegin - particle.mSizeEnd) / particle.mLifetime;
					if (particle.mSize > particle.mSizeEnd) {
						particle.mSize -= sizeChangePerSecond * static_cast<float>(TIME->GetDeltaTime());
						if (particle.mSize <= particle.mSizeEnd)
						{
							particle.mSize = particle.mSizeEnd;
						}
					}
				}
				// Getting larger
				else if (particle.mSizeBegin < particle.mSizeEnd) {
					float sizeChangePerSecond = (particle.mSizeEnd - particle.mSizeBegin) / particle.mLifetime;
					if (particle.mSize < particle.mSizeEnd) {
						particle.mSize += sizeChangePerSecond * static_cast<float>(TIME->GetDeltaTime());
						if (particle.mSize >= particle.mSizeEnd)
						{
							particle.mSize = particle.mSizeEnd;
						}
					}
				}

				// Update the GameObject itself
				// TODO: REFACTOR GO ALIVE CHECKING
				if (GOMANAGER->IsGameObjectInScene(particle.mGameObj) && particle.mGameObj->IsActive() == true) {
					particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
					particle.mGameObj->GetComponent<CPTransform>()->SetRotation(particle.mRotation);
					particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSize, particle.mSize});
				}
			}
			else 
			{
				// Particle lifetime delay still active so reduce it
				particle.mLifetimeDelay -= static_cast<float>(TIME->GetDeltaTime());
			}
		}
	}

	/*!***********************************************************************
	\brief
	Updates all the emitters within the emitter pool
	*************************************************************************/
	void ParticleManager::UpdateEmitters() 
	{
		for (CPParticle* emitter : mEmitterPool) 
		{
			if (emitter != nullptr)
			{
				//std::cout << "Here!\n";
				emitter->Update();
			}
		}
	}

	/*!***********************************************************************
	\brief
	Makes sure all the particles are nullptr when the editor is not in play
	mode
	*************************************************************************/
	void ParticleManager::WhenCoreNotPlaying()
	{
		if (!CORE->IsPlaying()) {
			// Loop through the mParticles and make the GameObj* = nullptr
			for (int i = 0; i < mParticlePool.size(); ++i)
			{
				mParticlePool[i].mGameObj = nullptr;
			}
		}
	}
	/*!***********************************************************************
	\brief
	This function allows to spawn a particle from given parameters
	*************************************************************************/
	void ParticleManager::CreateParticle(Vec2<float> pos, float lifetime,
		float varXMin, float varXMax,
		float varYMin, float varYMax,
		float sizeBegin, float sizeEnd,
		std::string textureName
	)
	{
		// Get particle
		Particle& particle = mParticlePool[mParticlePoolIndex];
		
		particle.mIsActive = true;
		particle.mPosition = pos;
		particle.mRotation = 0.f;

		particle.mVelocity = Vec2<float>{ 0.f,0.f };
		particle.mVelocity.x += RandomRange(varXMin, varXMax);
		particle.mVelocity.y += RandomRange(varYMin, varYMax);

		particle.mLifetime = lifetime;
		particle.mLifetime = lifetime;

		particle.mSizeBegin = sizeBegin;
		particle.mSize = sizeBegin;
		particle.mSizeEnd = sizeEnd;

		particle.mGameObj = FACTORY->SpawnGameObject();
		particle.mGameObj->GetComponent<CPTransform>()->SetPosition(pos);
		particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
		particle.mGameObj->GetComponent<CPRender>()->Initialise();
		particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, 100, 100);

		// Update index
		mParticlePoolIndex = (mParticlePoolIndex - 1 + static_cast<int>(mParticlePool.size())) % static_cast<int>(mParticlePool.size());

	}

	/*!***********************************************************************
	\brief
	This function clears the ParticlePool and EmitterPool
	*************************************************************************/
	void ClearPool(Scene* newScene)
	{
		UNREFERENCED_PARAMETER(newScene);
		ParticleManager::Instance()->Destroy();
	}

	/*!***********************************************************************
	\brief
	Returns a random float from the given min and max
	*************************************************************************/
	float RandomRange(float min, float max) {
		return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
	}

}