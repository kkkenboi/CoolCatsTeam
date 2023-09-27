#pragma once
#include "LitterBox/Engine/Layer.h"
class LayerStack
{
public:
	LayerStack();
	~LayerStack();

	// emplace back
	void AddLayer(Layer* layer);
	void RemoveLayer(Layer* layer);

	//std::vector<Layer*> GetLayers() const;
	std::vector<Layer*>::iterator begin();
	std::vector<Layer*>::iterator end();

	// Overlay layer should be the last layer
	// emplace
	void MakeLayerOverlay(Layer* layer);

private:
	std::vector<Layer*> layers{};
	int layerIndex{};
};

