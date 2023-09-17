#include "pch.h"
#include "GameObjectFactory.h"

namespace LB
{
	GameObjectFactory* FACTORY = NULL;

	GameObjectFactory::GameObjectFactory()
	{
		if (FACTORY != NULL)
		{
			std::cout << "Factory already created!\n";
		}

		FACTORY = this;
		LastGameObjectID = 0;
	}

	GameObjectFactory::~GameObjectFactory()
	{
		//Delete all component creators
		ComponentMapType::iterator it = ComponentMap.begin();
		for (; it != ComponentMap.end(); ++it)
			delete it->second;
	}

	GameObjectComposition* GameObjectFactory::CreateEmptyComposition()
	{
		//Create the composition and give it an id
		//this function is used to create compositions
		//programmatically
		GameObjectComposition* gameObject = new GameObjectComposition();
		IDGameObject(gameObject);
		return gameObject;
	}

	GameObjectComposition* GameObjectFactory::BuildAndSerialize(const std::string& filename)
	{
	//	////Open the text file stream serializer -- TODO using our serializer
	//	//TextSerializer stream;
	//	//bool fileOpened = stream.Open(filename);
	//	//ErrorIf(!fileOpened, "Could not open file %s. File does not exist or is protected.", filename.c_str());
	//	//std::string componentName;


	//	//Make sure the stream is valid
	//	if (stream.IsGood())
	//	{
	//		//Create a new game object to hold the components
	//		GOC* gameObject = new GOC();

	//		while (stream.IsGood())
	//		{
	//			//Read the component's name
	//			StreamRead(stream, componentName);

	//			//Find the component's creator
	//			ComponentMapType::iterator it = ComponentMap.find(componentName);
	//			ErrorIf(it == ComponentMap.end(), "Could not find component creator with name %s", componentName.c_str());
	//			if (it != ComponentMap.end())
	//			{
	//				//ComponentCreator is an object that creates the component
	//				ComponentCreator* creator = it->second;

	//				//Create the component by using the interface
	//				GameComponent* component = creator->Create();

	//				//Serialize the component with current stream (this will move
	//				//the stream to the next component

	//				//Note: this invokes the base case of serialization in which
	//				//the object serializes itself. component->Serialize( stream );
	//				StreamRead(stream, *component);

	//				//Add the new component to the composition
	//				gameObject->AddComponent(creator->TypeId, component);
	//			}
	//		}

	//		//Id and initialize the game object composition
	//		IdGameObject(gameObject);

	//		return gameObject;
	//	}

	//	return NULL;
	}

	void GameObjectFactory::IDGameObject(GameObjectComposition* gameObject)
	{
		//Just increment the last id used. Does not handle 
		//overflow but it would take over 4 billion objects
		//to break
		++LastGameObjectID;
		gameObject->GameObjID = LastGameObjectID;

		//Store the game object in the global object id map
		GameObjectIDMap[LastGameObjectID] = gameObject;
	}

	GameObjectComposition* GameObjectFactory::GetObjectWithID(GOCID id)
	{
		GameObjectIDMapType::iterator it = GameObjectIDMap.find(id);
		if (it != GameObjectIDMap.end())
			return it->second;
		else
			return NULL;
	}


	GameObjectComposition* GameObjectFactory::Create(const std::string& filename)
	{
		GameObjectComposition* goc = BuildAndSerialize(filename);
		if (goc) goc->Initialize();
		return goc;
	}

	void GameObjectFactory::SendMessage(Message* message)
	{
		UNREFERENCED_PARAMETER(message);
	}

	void GameObjectFactory::Destroy(GameObjectComposition* gameObject)
	{
		//Add the object to the to be deleted list they will be delete
		//when the factory is updated
		ObjectsToBeDeleted.insert(gameObject);
	}

	void GameObjectFactory::Update(float dt)
	{
		dt;
		//Delete all objects in the ObjectsToBeDeleted list 

		std::set<GameObjectComposition*>::iterator it = ObjectsToBeDeleted.begin();
		for (; it != ObjectsToBeDeleted.end(); ++it)
		{
			GameObjectComposition* gameObject = *it;
			GameObjectIDMapType::iterator idItr = GameObjectIDMap.find(gameObject->GameObjID);
			if (idItr == GameObjectIDMap.end())
			{
				std::cout << "Object " << gameObject->GameObjID << " was double deleted or is bad memory.\n";
			}
			if (idItr != GameObjectIDMap.end())
			{
				//Delete it and remove its entry in the Id map
				delete gameObject;
				GameObjectIDMap.erase(idItr);
			}
		}
		//All objects to be delete have been deleted
		ObjectsToBeDeleted.clear();
	}

	//Bind the creator for this component with the component name
	void GameObjectFactory::AddComponentCreator(const std::string& name, ComponentCreator* creator)
	{
		ComponentMap[name] = creator;
	}

	//Clean up the game world
	void GameObjectFactory::DestroyAllObjects()
	{
		GameObjectIDMapType::iterator it = GameObjectIDMap.begin();
		for (; it != GameObjectIDMap.end(); ++it)
			delete it->second;

		GameObjectIDMap.clear();
	}



}