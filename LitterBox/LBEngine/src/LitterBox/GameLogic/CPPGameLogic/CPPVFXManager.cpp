/*!************************************************************************
 * \file				CPPVFXManager.cpp
 * \author(s)			Amadeus Chia 
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD
 * \date				15/03/2024
 * \brief 				Source file for the VFX manager that manages the spawning of vfx in
 * 						in the game
 *  Copyright (C) 2024 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "CPPVFXManager.h"
#include "LitterBox/Factory/Components.h"
#include "LitterBox/Factory/GameObjectFactory.h"
#include <functional>
#include <random>
namespace LB
{
	//Normal override methods that are not used
	void CPPSVFXManager::Start()
	{
		
	}
	void CPPSVFXManager::Update()
	{
		
	}
	void CPPSVFXManager::Destroy()
	{
	}

	/*!************************************************************************
	 * \brief Function to spawn the explosion anim
	 * 
	 * \param pos position to spawn the vfx in
	 * \param scale scale for the explosion vfx
	**************************************************************************/
	void CPPSVFXManager::SpawnExplosion(Vec2<float> pos, float scale)
	{
		GameObject* explosionClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *explosionClone);
		explosionClone->GetComponent<CPTransform>()->SetPosition(pos);
		explosionClone->GetComponent<CPTransform>()->SetScale(Vec2<float>{5.f, 5.f} * scale);
		explosionClone->GetComponent<CPAnimator>()->Play("VFX_Explosion");
		GOMANAGER->RemoveGameObject(explosionClone, 2.f);
	}

	/*!************************************************************************
	 * \brief Function to spawn the hit anim
	 * 
	 * \param pos position to spawn it in
	**************************************************************************/
	void CPPSVFXManager::SpawnHitAnim(Vec2<float> pos)
	{
		GameObject* hitFXClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *hitFXClone);
		hitFXClone->GetComponent<CPTransform>()->SetPosition(pos);
		hitFXClone->GetComponent<CPAnimator>()->Play("VFX_Hit");
		//Setting the scale UNIFORMLY
		std::mt19937 generator(static_cast<unsigned int>(TIME->GetTime()));
		std::uniform_real_distribution<float> uniform_distribution(-0.5f, 0.5f);
		auto rngScaleOffset = std::bind(uniform_distribution, generator);
		Vec2<float> randomScale(1+rngScaleOffset(),1+rngScaleOffset());

		//Setting the rotation randomly
		int rotOffset = (rand() % 360);
		hitFXClone->GetComponent<CPTransform>()->SetScale(randomScale*2);
		hitFXClone->GetComponent<CPTransform>()->SetRotation(static_cast<float>(rotOffset));
		GOMANAGER->RemoveGameObject(hitFXClone, 2.f);
	}

	/*!************************************************************************
	 * \brief function to spawn the poof anim
	 * 
	 * \param pos position to spawn it in
	 * \param scaleMult scale of the poof anim
	**************************************************************************/
	void CPPSVFXManager::SpawnPoofAnim(Vec2<float> pos, float scaleMult)
	{
		GameObject* poofClone = FACTORY->SpawnGameObject();
		JSONSerializer::DeserializeFromFile("VFX Object", *poofClone);
		poofClone->GetComponent<CPTransform>()->SetPosition(pos);
		poofClone->GetComponent<CPTransform>()->SetScale(Vec2<float>(1, 1)*scaleMult);
		poofClone->GetComponent<CPAnimator>()->Play("VFX_Poof");
		GOMANAGER->RemoveGameObject(poofClone, 2.f);

	}
}