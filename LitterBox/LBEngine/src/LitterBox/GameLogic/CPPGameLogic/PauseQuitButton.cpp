#include "Platform/Windows/Windows.h"
#include "PauseQuitButton.h"
#include "LitterBox/Engine/Input.h"
#include "LitterBox/Physics/ColliderManager.h"
#include "LitterBox/Core/Core.h"

namespace LB 
{
	void PauseQuitButton::Start() 
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

	void PauseQuitButton::Update()
	{
		Vec2<float> mouse_pos = INPUT->GetMousePos();
		DEBUG->DrawCircle(mouse_pos, 1.0f, Vec4<float>{0.0f, 0.0f, 0.5f, 1.0f});
		if (INPUT->IsKeyPressed(KeyCode::KEY_MOUSE_1))
		{
			Vec2<float> current_pos = GameObj->GetComponent<CPTransform>()->GetPosition();

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

	void PauseQuitButton::Destroy()
	{

	}

	CPCollider* PauseQuitButton::GetCollider() 
	{
		return mCollider;
	}
}