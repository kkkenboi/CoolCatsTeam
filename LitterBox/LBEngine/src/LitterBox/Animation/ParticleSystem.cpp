#include "ParticleSystem.h"

namespace LB
{

	void ParticleManager::Initialize() 
	{
		SetSystemName("Particle System");
		std::cout << "Particle System Initialized\n";
		mEmitterPoolIndex = mEmitterPool.size() - 1;
		mParticlePoolIndex = mParticlePool.size() - 1;

		const EmitterType EmitterTypeVector[] = { TRAIL, RADIAL };
		for (EmitterType emit : EmitterTypeVector)
		{
			mEmitterTypes.emplace_back(std::make_pair(ParticleManager::Instance()->GetEmitterType(emit), emit));
		}
	}

	// Spawns a particle from an emitters configs
	void ParticleManager::Emit(CPParticle* emitter) 
	{

		if (emitter->mEmitterType == TRAIL)
		{
			// Gets a particle from the particle pool to instantiate
			Particle& particle = mParticlePool[mParticlePoolIndex];
			particle.mIsActive = true;
			particle.mPosition = emitter->mEmitterPos;
			particle.mRotation = 0.f;

			// Velocity
			particle.mVelocity = emitter->mEmitterVelocity;
			particle.mVelocity.x += RandomRange(emitter->mEmitterVariationMinX, emitter->mEmitterVariationMaxX);
			particle.mVelocity.y += RandomRange(emitter->mEmitterVariationMinY, emitter->mEmitterVariationMaxY);

			// Texture

			// Lifetime
			particle.mLifetime = emitter->mParticleLifetime;
			particle.mLifetimeRemaining = emitter->mParticleLifetime;

			// Size
			particle.mSizeBegin = emitter->mEmitterSizeBegin;
			particle.mSize = particle.mSizeBegin;
			particle.mSizeEnd = emitter->mEmitterSizeEnd;

			mParticlePoolIndex = (mParticlePoolIndex - 1 + mParticlePool.size()) % mParticlePool.size();

			// Create a GameObject that follows the Particle's current stats
			particle.mGameObj = FACTORY->SpawnGameObject();
			particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
			particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
			particle.mGameObj->GetComponent<CPRender>()->Initialise();
			// Get the texture ID from the emitter
			int textureID = emitter->mRender->texture;
			std::string textureName = ASSETMANAGER->GetTextureName(textureID);
			//std::cout << emitter.mRender->w << '\n';
			//std::cout << emitter.mRender->h << '\n';
			//std::cout << ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second << std::endl;
			//std::cout << ASSETMANAGER->GetTextureName(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second) << std::endl;
			particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));
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

				// Velocity (calculated in radial form)
				float angle = static_cast<float>(i) / static_cast<float>(emitter->mRadialParticles) * 2.0f * PI; // PI is assumed to be defined
				particle.mVelocity.x = emitter->mEmitterRadialSpeed * cos(angle);
				particle.mVelocity.y = emitter->mEmitterRadialSpeed * sin(angle);

				// Lifetime
				particle.mLifetime = emitter->mParticleLifetime;
				particle.mLifetimeRemaining = emitter->mParticleLifetime;

				// Size
				particle.mSizeBegin = emitter->mEmitterSizeBegin;
				particle.mSize = particle.mSizeBegin;
				particle.mSizeEnd = emitter->mEmitterSizeEnd;

				// Create a GameObject
				particle.mGameObj = FACTORY->SpawnGameObject();
				particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
				particle.mGameObj->AddComponent(C_CPRender, FACTORY->GetCMs()[C_CPRender]->Create());
				particle.mGameObj->GetComponent<CPRender>()->Initialise();

				// Get the texture ID from the emitter
				int textureID = emitter->mRender->texture;
				std::string textureName = ASSETMANAGER->GetTextureName(textureID);
				particle.mGameObj->GetComponent<CPRender>()->UpdateTexture(ASSETMANAGER->Textures[ASSETMANAGER->assetMap[textureName]].second, static_cast<int>(emitter->mRender->w), static_cast<int>(emitter->mRender->h));

				// Move to the next index in the particle pool
				mParticlePoolIndex = (mParticlePoolIndex - 1 + mParticlePool.size()) % mParticlePool.size();
			}
		}
	}


	void ParticleManager::Update()
	{
		if (!TIME->IsPaused() && CORE->IsPlaying())
		{
			UpdateParticles();
			UpdateEmitters();
		}
		WhenCoreNotPlaying();
	}

	void ParticleManager::Destroy() 
	{
		// Loop through the EmitterPool and make them all nullptr
		for (int i = 0; i < mEmitterPool.size(); ++i) 
		{
			mEmitterPool[i] = nullptr;
		}
	}

	std::string ParticleManager::GetEmitterType(EmitterType EmitType)
	{
		switch (EmitType)
		{
		case TRAIL:
			return "Trail";
		case RADIAL:
			return "Radial";
		default:
			return "ERROR!";
		}
	}

	std::vector<std::pair<std::string, EmitterType>>& ParticleManager::GetVectorEmits() {
		return mEmitterTypes;
	}

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

	void ParticleManager::RemoveEmitter(CPParticle* findEmitter) {
		for (int i = 0; i < mEmitterPool.size(); ++i) {
			if (mEmitterPool[i] == findEmitter) {
				mEmitterPool[i] = nullptr;
			}
		}
	}

	void ParticleManager::UpdateParticles() {
		for (Particle& particle : mParticlePool)
		{
			if (!particle.mIsActive)
			{
				continue;
			}

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

			// Update the particle stats
			particle.mLifetimeRemaining -= TIME->GetDeltaTime();
			particle.mPosition += particle.mVelocity * TIME->GetDeltaTime();
			particle.mRotation += 0.01f * TIME->GetDeltaTime();


			// Check if size is enlarging or getting smaller
			if (particle.mSizeBegin > particle.mSizeEnd) {
				// Getting smaller
				float sizeChangePerSecond = (particle.mSizeBegin - particle.mSizeEnd) / particle.mLifetime;
				if (particle.mSize > particle.mSizeEnd) {
					particle.mSize -= sizeChangePerSecond * TIME->GetDeltaTime();
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
					particle.mSize += sizeChangePerSecond * TIME->GetDeltaTime();
					if (particle.mSize >= particle.mSizeEnd) 
					{
						particle.mSize = particle.mSizeEnd;
					}
				}
			}

			// Update the GameObject itself
			if (particle.mGameObj != nullptr && particle.mGameObj->IsActive() == true) {
				particle.mGameObj->GetComponent<CPTransform>()->SetPosition(particle.mPosition);
				particle.mGameObj->GetComponent<CPTransform>()->SetRotation(particle.mRotation);
				particle.mGameObj->GetComponent<CPTransform>()->SetScale(Vec2<float>{particle.mSize, particle.mSize});
			}
		}
	}

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

	float RandomRange(float min, float max) {
		return min + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * (max - min);
	}

}