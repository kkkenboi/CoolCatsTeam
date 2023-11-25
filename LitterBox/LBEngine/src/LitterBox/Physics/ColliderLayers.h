#pragma once
#include <string>
#include <utility>
#include "LitterBox/Serialization/Serializer.h"

namespace LB {
	class ColliderLayer;

	class ColliderLayerSystem {
	public:
		
		/*!***********************************************************************
		\brief
		  This function initializes the ColliderLayerSystem with some default
		  layers and also sets their collision matrix
		*************************************************************************/
		void Initialize();

		/*!***********************************************************************
		\brief
		  This function adds a layer to the CollisionLayerSystem, sets all its
		  collision with other layers to false within the matrix
		*************************************************************************/
		ColliderLayer AddLayer(std::string layer_name);

		/*!***********************************************************************
		\brief
		  This function removes a layer from the ColliderLayerSystem and resizes
		  the collision matrix with one less layer
		*************************************************************************/
		void RemoveLayer(std::string layer_name);

		/*!***********************************************************************
		\brief
		  This function finds the layer number from a layer name
		*************************************************************************/
		int FindLayerNumber(std::string layer_name);

		/*!***********************************************************************
		\brief
		  This function returns the first layer in the vector which is normally
		  the default layer
		*************************************************************************/
		ColliderLayer& GetDefaultLayer();

		/*!***********************************************************************
		\brief
		  This returns the vector of all the layers in the CollisionLayerSystem
		*************************************************************************/
		std::vector<std::pair<std::string, ColliderLayer>>& GetLayerVector();

		/*!***********************************************************************
		\brief
		  This function returns the collision matrix of the CollisionLayerSystem
		*************************************************************************/
		std::vector<std::vector<int>>& GetLayerMatrix();

		/*!***********************************************************************
		\brief
		  This function initializes the ColliderLayerSystem with some default
		  layers and also sets their collision matrix
		*************************************************************************/
		void SetCollisionLayer(ColliderLayer layerA, ColliderLayer layerB, bool canCollide);


		/*!***********************************************************************
		\brief
		  This function sets all the Collision Matrix for this layer to true,
		  which means all layers will collide with this layer
		*************************************************************************/
		void SetCollisionLayerAllTrue(ColliderLayer layer);

		/*!***********************************************************************
		\brief
		  This function returns whether or not two layers should collide
		*************************************************************************/
		bool ShouldLayerCollide(ColliderLayer layerA, ColliderLayer layerB);

		/*!***********************************************************************
		\brief
		  This function serializes the ColliderLAyerSystem
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		\brief
		  This function deserializes the ColliderLayerSystem
		*************************************************************************/
		bool Deserialize(const Value& data);


	private:
		std::vector<std::pair<std::string, ColliderLayer>> m_layers;
		std::vector<std::vector<int>> m_collision_layer_matrix;
	};


	class ColliderLayer {
	public:
		/*!***********************************************************************
		\brief
		  This function gets the name of the layer
		*************************************************************************/
		std::string& GetName();

		/*!***********************************************************************
		\brief
		  This function gets the position of the layer in the vector of the
		  CollisionLayerMatrix
		*************************************************************************/
		int& GetPosInVec();

		/*!***********************************************************************
		\brief
		  This function allows us to serialize the CollisionLayer
		*************************************************************************/
		bool Serialize(Value& data, Document::AllocatorType& alloc);

		/*!***********************************************************************
		\brief
		  This function allows us to deserialize the CollisionLayer
		*************************************************************************/
		bool Deserialize(const Value& data);
	public:
		std::string m_name;
		int vec_pos;
	};


}