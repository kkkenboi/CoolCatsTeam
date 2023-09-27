#include "pch.h"
#include "LayerStack.h"

LayerStack::LayerStack()
{

}

LayerStack::~LayerStack()
{
	for (Layer* layer : layers)
	{
		delete layer;
	}
}

//std::vector<Layer*> LayerStack::GetLayers() const
//{
//	return layers;
//}

// Overloading vector functions
std::vector<Layer*>::iterator LayerStack::begin()
{
	return layers.begin();
}
std::vector<Layer*>::iterator LayerStack::end()
{
	return layers.end();
}


void LayerStack::AddLayer(Layer* layer)
{
	layers.emplace(layers.begin() + layerIndex++, layer);
}

void LayerStack::RemoveLayer(Layer* layer)
{
	std::vector<Layer*>::iterator it = layers.begin();

	while (it != layers.end())
	{
		if (*it == layer)
		{
			layers.erase(it);
			--layerIndex;
		}
	}
}

void LayerStack::MakeLayerOverlay(Layer* layer)
{
	std::vector<Layer*>::iterator it = layers.begin();

	// Checks if there is duplicate layers
	while (it != layers.end())
	{
		if (*it == layer)
		{
			std::cerr << "Error: Layer is already within the LayerStack!\n";
		}
	}
	// Else set layer as overlay layer
	layers.emplace_back(layer);
}