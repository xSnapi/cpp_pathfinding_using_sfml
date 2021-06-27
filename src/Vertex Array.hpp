#pragma once
#include "stdafx.h"
#include "Public Variables.h"

class VertexArray : public sf::Drawable{
	std::vector<sf::Vertex> nodes;
	std::vector<sf::Vertex> grid;

	void addQuad(const int index_X, const int index_Y);
	void addStrip(const int index_X, const int index_Y, const float max_strip_size, const bool vertical);

	void initNodes();
	void initGrid();
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
public:
	const sf::Color default_color = sf::Color(230, 230, 230);

	VertexArray();
	~VertexArray();
	
	void setNodeColor(const int index_X, const int index_Y, sf::Color color);
	void setNodeColor(size_t index, sf::Color color);
	void resetNodes();
};