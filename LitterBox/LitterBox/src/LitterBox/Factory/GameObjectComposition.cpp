#include "pch.h"
#include "Components.h"
#include "GameObjectFactory.h"
#include "GameObjectComposition.h"

namespace LB
{
	//Used to sort components using their type Id.
	struct ComponentSorter
	{
		bool operator()(GameComponent* left, GameComponent* right) const
		{
			return left->TypeID < right->TypeID;
		}
	};

	//Binary search a sorted array of components.
	GameComponent* BinaryComponentSearch(std::vector<GameComponent*>& components, ComponentTypeID name)
	{
		size_t begin = 0;
		size_t end = components.size();

		while (begin < end)
		{
			size_t mid = (begin + end) / 2;
			if (components[mid]->TypeID < name)
				begin = mid + 1;
			else
				end = mid;
		}

		if ((begin < components.size()) && (components[begin]->TypeID == name))
			return components[begin];
		else
			return NULL;
	}

	void GameObjectComposition::Initialize()
	{
		//Initialize every component on this composition
		//and set their composition owner. This allows each
		//component to initialize itself separate from its constructor.
		//Which is need for serialization and to allow components to get
		//pointers to each other during initialization.
		for (ComponentIt it = Components.begin(); it != Components.end(); ++it)
		{
			(*it)->Base = this;
			(*it)->Initialize();
		}
	}

	GameObjectComposition::GameObjectComposition()
	{
		GameObjID = 0;
	}

	GameObjectComposition::~GameObjectComposition()
	{
		//Delete each component using the component's virtual destructor
		//takes care of all resources and memory.
		for (ComponentIt it = Components.begin(); it != Components.end(); ++it)
			delete* it;
	}

	void GameObjectComposition::AddComponent(ComponentTypeID typeId, GameComponent* component)
	{
		//Store the components type Id
		component->TypeID = typeId;
		Components.push_back(component);

		//Sort the component array so binary search can be used to find components quickly.
		std::sort(Components.begin(), Components.end(), ComponentSorter());
	}

	void GameObjectComposition::SendMessage(Message* message)
	{
		//When a message is sent to a composition it sends to
		//all of its components
		//for( MapIt it = Map.begin();it!=Map.end();++it)
		for (ComponentIt it = Components.begin(); it != Components.end(); ++it)
			(*it)->SendMessage(message);
	}

	GameComponent* GameObjectComposition::GetComponent(ComponentTypeID typeId)
	{
		return BinaryComponentSearch(Components, typeId);
	}

	void GameObjectComposition::Destroy()
	{
		//Signal the factory that is object needs to be destroyed
		//this will happen at the end of the frame.
		FACTORY->Destroy(this);
	}

}