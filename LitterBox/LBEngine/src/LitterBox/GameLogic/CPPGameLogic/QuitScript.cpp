/*!************************************************************************
 \file                QuitScript.cpp
 \author(s)           Ryan Tan Jian Hao
 \par DP email(s):    ryanjianhao.tan\@digipen.edu
 \par Course:         CSD2401A
 \date                20-11-2023
 \brief

 Source file that contains the definitions for the Quit Menu button script declared
 in Butt.h

 Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
 disclosure of this file or its contents without the prior written consent
 of DigiPen Institute of Technology is prohibited.
**************************************************************************/
#include "Platform/Windows/Windows.h"
#include "QuitScript.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Core/Core.h"
#include "LitterBox/Scene/SceneManager.h"

extern const float deg_to_rads;

namespace LB {
	void QuitScript::Start()
	{
		coll = GameObj->GetComponent<CPCollider>();

		float rot{ GameObj->GetComponent<CPTransform>()->GetRotation() };

		//-------------Rotation matrix values to get the left of the button---------------
		rot_row1.x = cosf(rot);
		rot_row1.y = -sinf(rot);

		rot_row2.x = sinf(rot);
		rot_row2.y = cosf(rot);

		//get the position of the object
		right_side = coll->m_pos;
		Vec2<float> pos_trans{ right_side };

		//move the point to the right of the game object
		right_side.x -= (coll->m_width * 0.5f);
		//bring the point to the origin
		right_side -= pos_trans;

		//rotate the object around the origin
		Vec2<float> tmp{ right_side };
		right_side.x = DotProduct(rot_row1, tmp);
		right_side.y = DotProduct(rot_row2, tmp);

		//move the point back
		right_side = right_side + pos_trans;

		//get a pointer to the hand game object
		for (const auto& e : GOMANAGER->GetGameObjects()) {
			if (e->GetName() != "Hand") {
				continue;
			}
			hand = e;
			break;
		}

		//get pos of hand object
		if (!hand)
			return;


		Vec2<float> hand_size{
			hand->GetComponent<CPRender>()->w * hand->GetComponent<CPTransform>()->GetScale().x * 0.5f,
			//we use a little less than half because there is some extra space on the top right corner
			hand->GetComponent<CPRender>()->h * hand->GetComponent<CPTransform>()->GetScale().y * 0.45f,
		};

		right_side = right_side - hand_size;
		//(hand->GetComponent<CPTransform>()->GetPosition() * hand->GetComponent<CPTransform>()->GetScale() * 0.5f);
		//-------------Rotation matrix values to get the left of the button---------------
	}
	void QuitScript::Update()
	{
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			DebuggerLogWarning("Mouse 1 is pressed!");
			LB::Vec2<float> mouse{ INPUT->GetMousePos() };
			mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
			auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

			DebuggerLogFormat("CLICK POS: %f, %f", mouse.x, mouse.y);

			for (const auto& collider : test) {
				if (coll != collider) {
					continue;
				}

				if (GameObj->GetName() == "Quit") {
					MessageQuit q;
					CORE->BroadcastMessage(&q);
				}
				else if (GameObj->GetName() == "StartGame") {
					SCENEMANAGER->LoadScene("SceneMain");
				}
			}
		}

		//checking if mouse is over the button
		LB::Vec2<float> mouse{ INPUT->GetMousePos() };
		mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
		auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

		for (const auto& collider : test) {
			if (coll == collider) {

				hand->GetComponent<CPTransform>()->SetPosition(right_side);
			}
		}
	}
	void QuitScript::Destroy()
	{
	}
}