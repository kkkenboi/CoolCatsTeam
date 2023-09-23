#include "GameObjectManager.h"

namespace LB
{
	//GameObjectManager* GOMANAGER = nullptr;

	GameObjectManager::GameObjectManager()
	{

		//if (!GOMANAGER)
		//{
		//	GOMANAGER = this;
		//}
		//else
		//{
		//	std::cerr << "GameObjectManager already exist\n";
		//}



		std::cout << "GameObjectManager constructed\n";
	}
	GameObjectManager::~GameObjectManager()
	{
		std::cout << "GameObjectManager destructed\n";
	}

	GameObject::GameObject()
	{
		std::cout << "GameObject constructed\n";
	}

	GameObject::GameObject(std::vector<IComponent*> const& componentList) : m_Components{ componentList } 
	{
		std::cout << "GameObject copy constructor\n";
	};

	GameObject::~GameObject()
	{
		// Should delete all of the components
		std::cout << "GameObject destructed\n";
	}

}