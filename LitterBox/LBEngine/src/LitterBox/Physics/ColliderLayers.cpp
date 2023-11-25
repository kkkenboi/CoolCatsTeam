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

namespace LB {

	void ColliderLayerSystem::Initialize() {
		m_layers.clear();
		m_collision_layer_matrix.clear();

		ColliderLayer DefaultLayer = AddLayer("Default");
		ColliderLayer GameEntityLayer = AddLayer("GameEntity");
		ColliderLayer UILayer = AddLayer("UI");
		
		SetCollisionLayer(DefaultLayer, DefaultLayer, true);
		SetCollisionLayer(DefaultLayer, GameEntityLayer, true);
		SetCollisionLayer(DefaultLayer, UILayer, true);

		SetCollisionLayer(GameEntityLayer, GameEntityLayer, true);
		SetCollisionLayer(UILayer, UILayer, true);
		SetCollisionLayerAllTrue(DefaultLayer);
	}

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
			std::cout << "Not found" << std::endl;
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

	ColliderLayer& ColliderLayerSystem::GetDefaultLayer()
	{
		return m_layers[0].second;
	}

	std::vector<std::pair<std::string, ColliderLayer>>& ColliderLayerSystem::GetLayerVector()
	{
		return m_layers;
	}
	
	std::vector<std::vector<int>>& ColliderLayerSystem::GetLayerMatrix()
	{
		return m_collision_layer_matrix;
	}
	
	void ColliderLayerSystem::SetCollisionLayer(ColliderLayer layerA, ColliderLayer layerB, bool canCollide)
	{
		int indexA = layerA.GetPosInVec();
		int indexB = layerB.GetPosInVec();

		m_collision_layer_matrix[indexA][indexB] = canCollide;
		m_collision_layer_matrix[indexB][indexA] = canCollide;
	}

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

	bool ColliderLayerSystem::ShouldLayerCollide(ColliderLayer layerA, ColliderLayer layerB)
	{
		return m_collision_layer_matrix[layerA.GetPosInVec()][layerB.GetPosInVec()];
	}

	bool ColliderLayerSystem::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		if (!m_layers.empty())
		{
			data.SetObject();
			Value layerArray(rapidjson::kArrayType);
			for (auto& layer : m_layers)
			{
				Value layerValue;
				if (layer.second.Serialize(layerValue, alloc))
				{
					layerArray.PushBack(layerValue, alloc);
				}
			}
			data.AddMember("CollisionLayers", layerArray, alloc);
			return true;
		}
		return false;
	}

	bool ColliderLayerSystem::Deserialize(const Value& data)
	{
		bool HasCollisionLayers = data.HasMember("CollisionLayers");
		if (HasCollisionLayers)
		{	//We just save it into a json array 
			const Value& childrenValue = data["CollisionLayers"].GetArray();
			//for (rapidjson::SizeType i{ childrenValue.Size() }; i > 0; --i)
			//{
				//ColliderLayer temp;
				//temp.Deserialize(childrenValue[i]);
				//AddLayer(childrenValue[i].)
			for (Value::ConstMemberIterator itr = childrenValue.MemberEnd();
				itr != childrenValue.MemberBegin(); --itr)
			{
				if (itr->name.IsString() && itr->value.IsInt())
				{
					AddLayer(itr->name.GetString());
				}
			}
			return true;
		}
		return false;
	}

	// =================================
	// Collider Layer stuff
	// =================================
	/*
	ColliderLayer::ColliderLayer(std::string const name = "Default") : m_name{name}, vec_pos{-1}
	{
		// Empty by design
	}
	*/
	/*
	ColliderLayer& ColliderLayer::operator=(ColliderLayer layer) 
	{
		m_name = layer.GetName();
		vec_pos = layer.GetPosInVec();

		return *this;
	}
	*/
	
	

	std::string& ColliderLayer::GetName()
	{
		return m_name;
	}

	int& ColliderLayer::GetPosInVec() 
	{
		return vec_pos;
	}
	bool ColliderLayer::Serialize(Value& data, Document::AllocatorType& alloc)
	{
		data.SetObject();
		Value layerNameValue(m_name.c_str(), alloc);
		data.AddMember("Layer Name", layerNameValue, alloc);
		data.AddMember("Layer ID", vec_pos, alloc);
		return true;
	}
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