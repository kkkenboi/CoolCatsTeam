/*!************************************************************************
 \file				GameObjClicker.cpp
 \author(s)			Kenji Brannon Chong
 \par DP email(s):	kenjibrannon.c@digipen.edu
 \par Course:       CSD2401A
 \date				28/10/2023
 \brief

 This file implements the GameObject function that selects objects 
 in the SceneView.

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "LitterBox/Components/RenderComponent.h"
#include "LitterBox/Factory/GameObjectManager.h"
#include "LitterBox/Physics/PhysicsMath.h"
#include "LitterBox/Physics/Collisions.h"
#include "Math.h"

namespace LB 
{
	/*!***********************************************************************
	 * \brief
	 * Returns the GameObject that the clicker has clicked on
	 *************************************************************************/
	GameObject* CheckMousePosGameObj(Vec2<float> const& position)
	{
		float length = 1.0f;
		Vec2<float> temp_normal{ 0.f, 0.f };
		float temp_depth = 0.f;

		std::vector<GameObject*> temp_gameobj_vec;

		for (size_t i = 0; i < GOMANAGER->GetGameObjects().size(); ++i)
		{
			// Found the RenderComponent
			if (GOMANAGER->GetGameObjects()[i]->HasComponent<CPRender>())
			{
				// Grab the size of it and make an AABB using its size
				AABB temp_aabb_gameobj;
				float temp_width = GOMANAGER->GetGameObjects()[i]->GetComponent<CPRender>()->w;
				float temp_height = GOMANAGER->GetGameObjects()[i]->GetComponent<CPRender>()->h;
				temp_aabb_gameobj.m_c = GOMANAGER->GetGameObjects()[i]->GetComponent<CPTransform>()->GetPosition();
				temp_aabb_gameobj.m_max = Vec2<float>
				{
					temp_aabb_gameobj.m_c.x + (temp_width / 2.f),
					temp_aabb_gameobj.m_c.y + (temp_height / 2.f) 
				};
				temp_aabb_gameobj.m_min = Vec2<float>
				{
					temp_aabb_gameobj.m_c.x - (temp_width / 2.f),
					temp_aabb_gameobj.m_c.y - (temp_height / 2.f)
				};

				AABB temp_aabb_mouse;
				temp_aabb_mouse.m_c = position;
				temp_aabb_mouse.m_max = Vec2<float>
				{
					position.x + length / 2.f,
					position.y + length / 2.f
				};
				temp_aabb_mouse.m_min = Vec2<float>
				{
					position.x - length / 2.f,
					position.y - length / 2.f
				};

				// temp_aabb now the data of the GameObj
				if (CollisionIntersection_BoxBox(
					temp_aabb_mouse, Vec2<float>{0.f, 0.f},
					temp_aabb_gameobj, Vec2<float>{0.f, 0.f},
					0.1f, temp_normal, temp_depth))
				{
					temp_gameobj_vec.push_back(GOMANAGER->GetGameObjects()[i]);
				}
			}
		}

		// No objects determined
		if (!temp_gameobj_vec.size())
		{
			return nullptr;
		}

		float close_distance{ 1000000.f };
		int gameobj_num{ 0 };
		// Now the temp_gameobj_vec has all the game objs
		// For now will return the GameObj closest to the MousePos
		for (size_t i = 0; i < temp_gameobj_vec.size(); ++i)
		{
			if (PHY_MATH::Distance(position, temp_gameobj_vec[i]->GetComponent<CPTransform>()->GetPosition()) < close_distance ) {
				gameobj_num = i;
			}
		}

		return temp_gameobj_vec[gameobj_num];
	}
}
