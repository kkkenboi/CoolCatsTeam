#pragma once
#include <string>
#include <utility>

namespace LB {
	class ColliderLayer;

	class ColliderLayerSystem {
	public:
		// Initializes m_layers to have 3 different layers
		// Default, GameEntity , UI
		// Default allowing for Collision with everything
		void Initialize();

		// Adds a layer and default initializes all
		// layers to not collide with this layer except default
		ColliderLayer AddLayer(std::string layer_name);

		void RemoveLayer(std::string layer_name);

		int FindLayerNumber(std::string layer_name);

		ColliderLayer& GetDefaultLayer();

		std::vector<std::pair<std::string, ColliderLayer>>& GetLayerVector();

		// Sets the matrix for the two given layers
		void SetCollisionLayer(ColliderLayer layerA, ColliderLayer layerB, bool canCollide);

		void SetCollisionLayerAllTrue(ColliderLayer layer);

		// Returns true if these two layers can collide
		bool ShouldLayerCollide(ColliderLayer layerA, ColliderLayer layerB);

	private:
		std::vector<std::pair<std::string, ColliderLayer>> m_layers;
		std::vector<std::vector<bool>> m_collision_layer_matrix;
	};


	class ColliderLayer {
	public:
		std::string const GetName() const;
		int& GetPosInVec();
	public:
		std::string m_name;
		int vec_pos;
	};


}