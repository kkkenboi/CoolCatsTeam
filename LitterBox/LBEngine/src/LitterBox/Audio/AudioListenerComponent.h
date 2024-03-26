/*!************************************************************************
 * \file				AudioSourceComponent.h
 * \author(s)			Amadeus Chia
 * \par DP email(s):  	amadeusjinhan.chia@digipen.edu
 * \par Course:       	CSD2401A
 * \date				25/11/2023
 * \brief 				This file contains the declarations for the Audio
 * 						Component
 *  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 *  disclosure of this file or its contents without the prior written consent
 *  of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#pragma once
#include "LitterBox/Components/TransformComponent.h"
namespace LB
{
	class CPAudioListener : public IComponent
	{
	public:

		/*!***********************************************************************
		* ICOMPONENT OVERRIDES
		*************************************************************************/
		CPTransform* transform;
		/*!************************************************************************
		 * \brief Destroy the CPAudioSource object
		 *
		**************************************************************************/
		virtual ~CPAudioListener() = default; //First things first...

		/*!************************************************************************
		 * \brief Initialises the audio component
		 *
		**************************************************************************/
		void Initialise() override;

		/*!************************************************************************
		 * \brief Updates trhe audio component
		 *
		**************************************************************************/
		void Update() override;

		/*!************************************************************************
		 * \brief Destroys the audio component
		 * (Currently, this also clears out ALL audio components from the audiomanager)
		**************************************************************************/
		void Destroy() override;

		/*!************************************************************************
		 * \brief Serializes the audio component
		 *
		**************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc) override;

		/*!************************************************************************
		 * \brief Deserializes the audio component
		 *
		**************************************************************************/
		bool Deserialize(const Value& data) override;
		/*!************************************************************************
		 * \brief Get the Type object
		 *
		 * \return ComponentTypeID Label for the AudioComponent
		**************************************************************************/
		static ComponentTypeID GetType()
		{
			return C_CPAudioListener;
		}
	};

}