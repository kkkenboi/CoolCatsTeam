/*!************************************************************************
 \file				ColliderComponent.cpp
 \author(s)			Justine Carlo Villa Ilao
 \par DP email(s):	justine.c@digipen.edu
 \par Course:		CSD2401A
 \date				25-11-2023
 \brief
 This file contains the ColliderLayerSystem and the ColliderLayer class and
 all its functionalities, these classes handle making Collider Layers,
 and which layers can collide with each other in a collider matrix

  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction or
  disclosure of this file or its contents without the prior written consent
  of DigiPen Institute of Technology is prohibited.
**************************************************************************/

#include "ColliderLayers.h"
#include "LitterBox/Serialization/Serializer.h"
#include "ColliderManager.h"

namespace LB {

	/*!***********************************************************************
	\brief
	  This function initializes the ColliderLayerSystem with some default
	  layers and also sets their collision matrix
	*************************************************************************/
	void ColliderLayerSystem::Initialize() {
		m_layers.clear();
		m_collision_layer_matrix.clear();

		ColliderLayer DefaultLayer = AddLayer("Default");
		ColliderLayer GameEntityLayer = AddLayer("GameEntity");
		ColliderLayer UILayer = AddLayer("UI");
		ColliderLayer PlayerLayer = AddLayer("Player");
		ColliderLayer PlayerBallLayer = AddLayer("PlayerBall");
		ColliderLayer EnemyBallLayer = AddLayer("EnemyBall");
		
		SetCollisionLayer(DefaultLayer, DefaultLayer, true);
		SetCollisionLayer(DefaultLayer, GameEntityLayer, true);
		SetCollisionLayer(DefaultLayer, UILayer, true);

		SetCollisionLayer(GameEntityLayer, GameEntityLayer, true);
		SetCollisionLayer(UILayer, UILayer, true);

		SetCollisionLayer(PlayerLayer, GameEntityLayer, true);

		SetCollisionLayer(PlayerBallLayer, GameEntityLayer, true);
		SetCollisionLayer(PlayerBallLayer, PlayerBallLayer, true);

		SetCollisionLayer(EnemyBallLayer, PlayerLayer, true);

		SetCollisionLayerAllTrue(DefaultLayer);
		//JSONSerializer::DeserializeFromFile()
		JSONSerializer::DeserializeFromFile("CollisionMatrix", COLLIDERS->GetLayerSystem());

	}

	/*!***********************************************************************
	\brief
	  This function adds a layer to the CollisionLayerSystem, sets all its
	  collision with other layers to false within the matrix
	*************************************************************************/
	ColliderLayer ColliderLayerSystem::AddLayer(std::string layer_name) {
		ColliderLayer TempLayer(layer_name);

		// All layers vector changes =============

		// Add the Layer to the vector list
		m_layers.emplace_back(std::make_pair(TempLayer.GetName(), TempLayer));
		m_layers.back().second.GetPosInVec() = static_cast<int>(m_layers.size()) - 1;


		// Collision Matrix Changes ===========
		
		// Resize the Collision Layer Matrix
		int position = static_cast<int>(m_layers.size() - 1);
		// This adds the first layer to have a new layer added,
		// corresponding to the position of the layer in m_layers + 1

		// We then default initialise the vector inside the vector to be of
		// the new size and setting all collisions with other layers to false
		m_collision_layer_matrix.resize(position + 1, std::vector<int>(position + 1, 0));
		// We then adjust all the previously declared vectors to be of the new size
		for (int i = 0; i < position; ++i)
		{
			m_collision_layer_matrix[i].resize(position + 1, 0);
			m_collision_layer_matrix[position][i] = 0;
		}


		return m_layers.back().second;
	}

	/*!***********************************************************************
	\brief
	  This function removes a layer from the ColliderLayerSystem and resizes
	  the collision matrix with one less layer
	*************************************************************************/
	void ColliderLayerSystem::RemoveLayer(std::string layer_name) {
		// Loop through the std::vector of std::pairs, looking to see
		// if the std::string is found	
		bool found = false;
		int position = 0;

		for (std::pair<std::string, ColliderLayer> item: m_layers) 
		{
			if (item.first == layer_name) 
			{
				found = true;
				position = item.second.GetPosInVec();
			}
		}

		if (!found)
		{
			//std::cout << "Not found" << std::endl;
			return;
		}

		// Erase layer from m_layers the vector storing all layers
		m_layers.erase(m_layers.begin() + position);

		// Erase the column associated with the layer in the matrix
		m_collision_layer_matrix.erase(m_collision_layer_matrix.begin() + position);

		// Erase the rows associated with the layer in all the other columns
		for (std::vector<int> column : m_collision_layer_matrix) 
		{
			column.erase(column.begin() + position);
		}

		// Update all the ColliderLayers position data
		// that was after the removed column
		for (int i = position; i < m_layers.size(); ++i)
		{
			m_layers[i].second.GetPosInVec() = i;
		}
	}

	/*!***********************************************************************
	\brief
	  This function finds the layer number from a layer name
	*************************************************************************/
	int ColliderLayerSystem::FindLayerNumber(std::string layer_name) 
	{
		for (std::pair<std::string, ColliderLayer> item : m_layers)
		{
			//std::cout << "In FindLayerNumber" << std::endl;
			//std::cout << layer_name << std::endl;
			//std::cout << item.first << std::endl;
			if (item.first == layer_name)
			{
				//std::cout << "YoYo" << std::endl;
				return item.second.GetPosInVec();
			}
		}
		// If not found return -1
		return -1;
	}

	/*!***********************************************************************
	\brief
	  This function returns the first layer in the vector which is normally
	  the default layer
	*************************************************************************/
	ColliderLayer& ColliderLayerSystem::GetDefaultLayer()
	{
		return m_layers[0].second;
	}

	/*!***********************************************************************
	\brief
	  This returns the vector of all the layers in the CollisionLayerSystem
	*************************************************************************/
	std::vector<std::pair<std::string, ColliderLayer>>& ColliderLayerSystem::GetLayerVector()
	{
		return m_layers;
	}
	
	/*!***********************************************************************
	\brief
	  This function returns the collision matrix of the CollisionLayerSystem
	*************************************************************************/
	std::vector<std::vector<int>>& ColliderLayerSystem::GetLayerMatrix()
	{
		return m_collision_layer_matrix;
	}
	
	/*!***********************************************************************
	\brief
	  This function initializes the ColliderLayerSystem with some default
	  layers and also sets their collision matrix
	*************************************************************************/
	void ColliderLayerSystem::SetCollisionLayer(ColliderLayer layerA, ColliderLayer layerB, bool canCollide)
	{
		int indexA = layerA.GetPosInVec();
		int indexB = layerB.GetPosInVec();

		m_collision_layer_matrix[indexA][indexB] = canCollide;
		m_collision_layer_matrix[indexB][indexA] = canCollide;
	}

	/*!***********************************************************************
	\brief
	  This function sets all the Collision Matrix for this layer to true,
	  which means all layers will collide with this layer
	*************************************************************************/
	void ColliderLayerSystem::SetCollisionLayerAllTrue(ColliderLayer layer)
	{
		int index = layer.GetPosInVec();

		for (size_t i = 0; i < m_collision_layer_matrix.size(); ++i) 
		{
			for (size_t j = 0; j < m_collision_layer_matrix[i].size(); ++j)
			{
				if (i == index || j == index) 
				{
					m_collision_layer_matrix[i][j] = 1;
				}
			}
		}
	}

	/*!***********************************************************************
	\brief
	  This function returns whether or not two layers should collide
	*************************************************************************/
	bool ColliderLayerSystem::ShouldLayerCollide(ColliderLayer layerA, ColliderLayer layerB)
	{
		return m_collision_layer_matrix[layerA.GetPosInVec()][layerB.GetPosInVec()];
	}

	/*!***********************************************************************
	\brief
	  This function serializes the ColliderLAyerSystem
	*************************************************************************/
	bool ColliderLayerSystem::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		if (!m_layers.empty())
		{
			Value layerArray(rapidjson::kArrayType);
			for (auto& layer : m_layers)
			{
	/*			Value layerName(layer.first.c_str(), alloc);
				data.AddMember("Name", layerName, alloc);*/

				Value layerValue;
				if (layer.second.Serialize(layerValue, alloc))
				{
					layerArray.PushBack(layerValue, alloc);
				}
			}
			data.AddMember("CollisionLayers", layerArray, alloc);
		}
		else return false;
		if (!m_collision_layer_matrix.empty())
		{
			Value fromLayerValues(rapidjson::kArrayType);
			for (int i{}; i < m_collision_layer_matrix.size(); ++i)
			{
				Value toLayerValues(rapidjson::kArrayType);
				for (int j{}; j < m_collision_layer_matrix[i].size(); ++j)
				{
					toLayerValues.PushBack(m_collision_layer_matrix[i][j], alloc);
				}
				fromLayerValues.PushBack(toLayerValues, alloc);
			}
			data.AddMember("CollisionMatrix", fromLayerValues, alloc);
		}
		else return false;
		return true;
	}

	/*!***********************************************************************
	\brief
	  This function deserializes the ColliderLayerSystem
	*************************************************************************/
	bool ColliderLayerSystem::Deserialize(const Value& data)
	{
		//We probably want to clear before we deserialize????
		m_layers.clear();
		m_collision_layer_matrix.clear();
		bool HasCollisionLayers = data.HasMember("CollisionLayers");
		bool HasCollisionMatrix = data.HasMember("CollisionMatrix");
		//Then we deserialize the layers first
		if (HasCollisionLayers && HasCollisionMatrix)
		{	//We just save it into a json array 
			const Value& layerArray = data["CollisionLayers"].GetArray();
			for (rapidjson::SizeType i{}; i < layerArray.Size(); ++i)
			{
				ColliderLayer temp{};
				if (temp.Deserialize(layerArray[i]))
				{
					m_layers.push_back(std::make_pair(temp.m_name, temp));
				}
			}
			//Once we have all our layers, we resize our matrix
			//NOTE: Idk if this works, but it looks like it does
			ResizeMatrix();
			const Value& fromLayerValues = data["CollisionMatrix"].GetArray();
			for (rapidjson::SizeType i{}; i < fromLayerValues.Size(); ++i)
			{
				const Value& toLayerValues = fromLayerValues[i].GetArray();
				for (rapidjson::SizeType j{}; j < toLayerValues.Size(); ++j)
				{
					m_collision_layer_matrix[i][j] = toLayerValues[j].GetInt();
				}
			}
			return true;
		}
		return false;
	}

	//Helper function to resize the matrix
	void ColliderLayerSystem::ResizeMatrix()
	{
		int position = static_cast<int>(m_layers.size() - 1);
		// This adds the first layer to have a new layer added,
		// corresponding to the position of the layer in m_layers + 1

		// We then default initialise the vector inside the vector to be of
		// the new size and setting all collisions with other layers to false
		m_collision_layer_matrix.resize(position + 1, std::vector<int>(position + 1, 0));
		// We then adjust all the previously declared vectors to be of the new size
		for (int i = 0; i < position; ++i)
		{
			m_collision_layer_matrix[i].resize(position + 1, 0);
			m_collision_layer_matrix[position][i] = 0;
		}
	}

	// =================================
	// Collider Layer stuff
	// =================================
	
	
	/*!***********************************************************************
	\brief
	  This function gets the name of the layer
	*************************************************************************/
	std::string& ColliderLayer::GetName()
	{
		return m_name;
	}

	/*!***********************************************************************
	\brief
	  This function gets the position of the layer in the vector of the
	  CollisionLayerMatrix
	*************************************************************************/
	int& ColliderLayer::GetPosInVec() 
	{
		return vec_pos;
	}

	/*!***********************************************************************
	\brief
	  This function allows us to serialize the CollisionLayer
	*************************************************************************/
	bool ColliderLayer::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value layerNameValue(m_name.c_str(), alloc);
		data.AddMember("Layer Name", layerNameValue, alloc);
		data.AddMember("Layer ID", vec_pos, alloc);
		return true;
	}

	/*!***********************************************************************
	\brief
	  This function allows us to deserialize the CollisionLayer
	*************************************************************************/
	bool ColliderLayer::Deserialize(const Value& data)
	{
		bool HasLayerName = data.HasMember("Layer Name");
		bool HasLayerID = data.HasMember("Layer ID");
		if (HasLayerName && HasLayerID)
		{
			m_name = data["Layer Name"].GetString();
			vec_pos = data["Layer ID"].GetInt();
			return true;
		}
		return false;
	}
} // Namespace LB