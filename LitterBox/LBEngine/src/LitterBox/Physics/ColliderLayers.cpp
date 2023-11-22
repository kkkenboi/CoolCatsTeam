#include "ColliderLayers.h"

namespace LB {

	void ColliderLayerSystem::Initialize() {
		m_layers.clear();
		m_collision_layer_matrix.clear();

		ColliderLayer DefaultLayer = AddLayer("Default");
		ColliderLayer GameEntityLayer = AddLayer("GameEntity");
		ColliderLayer UILayer = AddLayer("UI");

		
		std::cout << "2D Array : [" << m_collision_layer_matrix.size() << "]" <<
			"[" << m_collision_layer_matrix[0].size() << "]" << std::endl;

		std::cout << "Layer Number Default: " << FindLayerNumber("Default")
			<< std::endl;

		std::cout << "Vec Num 0 :" << m_layers[0].first << std::endl;
		std::cout << "Vec Num 1 :" << m_layers[1].first << std::endl;
		std::cout << "Vec Num 2 :" << m_layers[2].first << std::endl;
		
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
			std::cout << "In FindLayerNumber" << std::endl;
			//std::cout << layer_name << std::endl;
			std::cout << item.first << std::endl;
			if (item.first == layer_name)
			{
				std::cout << "YoYo" << std::endl;
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
	
	

	std::string const ColliderLayer::GetName() const 
	{
		return m_name;
	}

	int& ColliderLayer::GetPosInVec() 
	{
		return vec_pos;
	}
} // Namespace LB