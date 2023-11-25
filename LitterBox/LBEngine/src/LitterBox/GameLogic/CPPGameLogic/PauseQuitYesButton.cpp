#include "Platform/Windows/Windows.h"
#include "PauseQuitYesButton.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Core/Core.h"

namespace LB 
{
	void PauseQuitYesButton::Start() 
	{

		if (GameObj->HasComponent<CPCollider>())
		{
			mCollider = GameObj->GetComponent<CPCollider>();
		}
		else
		{
			mCollider = nullptr;
			return;
		}
	}

	void PauseQuitYesButton::Update()
	{
			Vec2<float> mouse_pos = INPUT->GetMousePos();
		if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

			mouse_pos.y = mouse_pos.y * -1.f + (float)WINDOWSSYSTEM->GetHeight();

			mouse_pos.y *= 900.f / (float)WINDOWSSYSTEM->GetHeight();
			mouse_pos.x *= 1600.f / (float)WINDOWSSYSTEM->GetWidth();
			std::vector<CPCollider*> vec_colliders = COLLIDERS->OverlapCircle(mouse_pos, 1.0f);

			// Loop through the colliders found on mouse click
			for (size_t i = 0; i < vec_colliders.size(); ++i)
			{
				// If we found the block
				if (vec_colliders[i] == mCollider)
				{
					MessageQuit q;
					CORE->BroadcastMessage(&q);
				}
			}
		}
	}

	void PauseQuitYesButton::Destroy()
	{

	}
}