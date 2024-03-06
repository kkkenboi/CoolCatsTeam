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

namespace LB 
{

	/*!***********************************************************************
	\brief
	 Start function gets collider of object and gets the position that is left
	 of the collider. This is for the hand object to use to position properly.

	 NOTE: The game object MUST HAVE the collider component, behaviour if not
	 is undefined
	*************************************************************************/
	void QuitScript::Start()
	{
		coll = GameObj->GetComponent<CPCollider>();

		float rot{ GameObj->GetComponent<CPTransform>()->GetRotation() * deg_to_rads };

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

		GameObject* screen{ nullptr };

		//get a pointer to the hand game object
		for (const auto& e : GOMANAGER->GetGameObjects()) {
			//get pointer to main menu image
			if (e->GetName() == "Main Menu") {
				screen = e;
				continue;
			}
			if (e->GetName() == "Line") {
				line = e;
				continue;
			}
			//get pointer to hand object
			if (e->GetName() == "Hand") {
				hand = e;
				continue;
			}
			if (e->GetName() == "Transition Curtain")
			{
				curtain = e;
				continue;
			}
		}

		//get pos of hand object
		if (!hand || !screen)
			return;

		Vec2<float> hand_size{
			hand->GetComponent<CPRender>()->w * hand->GetComponent<CPTransform>()->GetScale().x * 0.5f,
			//we use a little less than half because there is some extra space on the top right corner
			hand->GetComponent<CPRender>()->h * hand->GetComponent<CPTransform>()->GetScale().y * 0.45f,
		};

		right_side = right_side - hand_size;
		//-------------Rotation matrix values to get the left of the button---------------


		//get all the quit confirmation stuff
		// Grab All The GOs associated with the Pause Menu
		for (GameObject* GO : GOMANAGER->GetGameObjects())
		{
			// Confirm Menu
			if (GO->GetName() == "PauseMenuQuitConfirmTexture")
			{
				ConfirmMenuTexture = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmYesObject")
			{
				ConfirmMenuYesButton = GO;
			}
			if (GO->GetName() == "PauseMenuQuitConfirmNoObject")
			{
				ConfirmMenuNoButton = GO;
			}
		}
		time = 0.f;
	}

	float bezier(float x)
	{
		constexpr float n1 = 7.5625f;
		constexpr float d1 = 2.75f;

		if (x < 1.f / d1) {
			return n1 * x * x;
		}
		else if (x < 2.f / d1) {
			return n1 * (x -= 1.5f / d1) * x + 0.75f;
		}
		else if (x < 2.5f / d1) {
			return n1 * (x -= 2.25f / d1) * x + 0.9375f;
		}
		else if (x > 1.f) {
			return 1.f;
		}
		else {
			return n1 * (x -= 2.625f / d1) * x + 0.984375f;
		}

		//return t * t * (3.f - 2.f * t);
	}
	/*!***********************************************************************
	\brief
	 Update function will checl to see if the mouse is hovering over the collider
	 and update the hand object to follow the button the mouse is hovering over.

	 The function also checks which mouse button is pressed and performs the
	 appropriate function.
	*************************************************************************/
	void QuitScript::Update()
	{
		if (animFlag)
		{
			Vec2 pos{ curtain->GetComponent<CPTransform>()->GetPosition() };
			if (time <= 1.1f)
			{
				pos.x = 2880.f - 1920.f * bezier(time);
				curtain->GetComponent<CPTransform>()->SetPosition(pos);
				time += TIME->GetDeltaTime();
			}
			else
			{
				SCENEMANAGER->LoadScene("SceneTut");
			}
			return;
		}
		if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
		{
			DebuggerLogWarning("Mouse 1 is pressed!");
			LB::Vec2<float> mouse{ INPUT->GetMousePos() };
			mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
			auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

			DebuggerLogFormat("CLICK POS: %f, %f", mouse.x, mouse.y);

			for (const auto& collider : test) {
				if (coll != collider) {
					continue;
				}

				DebuggerLogFormat("BUTTON CLICK");
				if (GameObj->GetName() == "Quit") {
					//------------------------------------------Move over the quit confirmation game objects----------------------------
					if (INPUT->IsKeyTriggered(KeyCode::KEY_MOUSE_1))
					{
						ConfirmMenuTexture->GetComponent<CPTransform>()->SetPosition(Vec2<float>{960.f, 540.f});
						ConfirmMenuYesButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{715.f, 420.f});
						ConfirmMenuNoButton->GetComponent<CPTransform>()->SetPosition(Vec2<float>{1200.f, 420.f});
					}
					//------------------------------------------Move over the quit confirmation game objects----------------------------
				}
				else if (GameObj->GetName() == "StartGame") {
					animFlag = true;
					//SCENEMANAGER->LoadScene("SceneMain");
				}
				else if (GameObj->GetName() == "Settings") {
					WINDOWSSYSTEM->toggleFullScreen();
				}
				else if (GameObj->GetName() == "Controls") {
					SCENEMANAGER->LoadScene("SceneHowTo");
				}
			}
		}
		static bool trig{ false };
		if (INPUT->IsKeyTriggered(KeyCode::KEY_O) && !trig) {
			SCENEMANAGER->LoadScene("SceneMain");
			trig = true;
		}

		//checking if mouse is over the button
		LB::Vec2<float> mouse{ INPUT->GetMousePos() };
		mouse.y = mouse.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();
		mouse.y *= 1080.f / (float)WINDOWSSYSTEM->GetHeight();
		mouse.x *= 1920.f / (float)WINDOWSSYSTEM->GetWidth();
		auto test = COLLIDERS->OverlapCircle(mouse, 1.f);

		for (const auto& collider : test) {
			if (coll == collider) {

				hand->GetComponent<CPTransform>()->SetPosition(right_side);
				float yOffset = coll->m_heightUnscaled / 2.f;
				line->GetComponent<CPTransform>()->SetPosition(coll->GetTransform()->GetPosition() - Vec2<float>{0,yOffset});

			}
		}
	}
	/*!***********************************************************************
	\brief
	 Empty by design
	*************************************************************************/
	void QuitScript::Destroy() { }
}