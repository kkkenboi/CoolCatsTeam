#pragma once
class Layer
{
public:
	Layer(std::string layerName);
	virtual ~Layer() {};

	virtual void AttachLayer() {};
	virtual void DetachLayer() {};

	virtual void UpdateLayer() {};

	std::string const& GetName() { return m_Layer; }

private:
	std::string m_Layer;
};

