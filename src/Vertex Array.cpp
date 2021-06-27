#include "stdafx.h"
#include "Vertex Array.hpp"

void VertexArray::addQuad(const int index_X, const int index_Y) {
	const float x = index_X * NODE_SIZE;
	const float y = index_Y * NODE_SIZE;

	std::array<sf::Vertex, 4> vertices;

	vertices[0].position = sf::Vector2f(x,				y);
	vertices[1].position = sf::Vector2f(x + NODE_SIZE,	y);
	vertices[2].position = sf::Vector2f(x + NODE_SIZE,	y + NODE_SIZE);
	vertices[3].position = sf::Vector2f(x,				y + NODE_SIZE);

	for (auto& v : vertices) {
		v.color = this->default_color;
		this->nodes.push_back(v);
	}
}

void VertexArray::addStrip(const int index_X, const int index_Y, const float max_strip_size, const bool vertical) {
	const sf::Color color = sf::Color(150, 150, 150);

	const int line_size = 1.f;

	const float x = ( index_X * NODE_SIZE ) - line_size / 2.f;
	const float y = ( index_Y * NODE_SIZE ) - line_size / 2.f;

	std::array<sf::Vertex, 4> vertices;
	
	if (vertical) {
		vertices[0].position = sf::Vector2f(x,				0.f);
		vertices[1].position = sf::Vector2f(x + line_size,	0.f);
		vertices[2].position = sf::Vector2f(x + line_size,	max_strip_size);
		vertices[3].position = sf::Vector2f(x,				max_strip_size);
	}
	else {
		vertices[0].position = sf::Vector2f(0.f,			y);
		vertices[1].position = sf::Vector2f(WINDOW_WIDTH,	y);
		vertices[2].position = sf::Vector2f(WINDOW_WIDTH,	y + line_size);
		vertices[3].position = sf::Vector2f(0.f,			y + line_size);
	}

	for (auto& v : vertices) {
		v.color = color;
		this->grid.push_back(v);
	}
}

void VertexArray::initNodes() {
	const long nodes_size = SIZE_X * SIZE_Y * 4;

	this->nodes.reserve(nodes_size);

	for (size_t y = 0; y < SIZE_Y; y++) {
		for (size_t x = 0; x < SIZE_X; x++) {
			this->addQuad(x, y);
		}
	}
}

void VertexArray::initGrid() {
	const long grid_size = SIZE_X * SIZE_Y * 4;

	this->grid.reserve(grid_size);

	for (size_t i = 0; i != SIZE_X; i++) {
		this->addStrip(i, 0, SIZE_Y * NODE_SIZE, true);
	}

	for (size_t i = 0; i != SIZE_Y + 1; i++) {
		this->addStrip(0, i, 0, false);
	}
}

void VertexArray::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->nodes.data(), this->nodes.size(), sf::Quads);
	target.draw(this->grid.data(), this->grid.size(), sf::Quads);
}

VertexArray::VertexArray() {
	this->initNodes();
	this->initGrid();
}

VertexArray::~VertexArray() {

}

void VertexArray::setNodeColor(const int index_X, const int index_Y, sf::Color color) {
	unsigned index = (index_Y * SIZE_X + index_X) * 4;
	if (index > this->nodes.size())
		return;
	for (size_t i = 0; i < 4; i++)
		this->nodes[index + i].color = color;
}

void VertexArray::setNodeColor(size_t index, sf::Color color) {
	index *= 4;
	if (index > this->nodes.size())
		return;
	for (size_t i = 0; i < 4; i++)
		this->nodes[index + i].color = color;
}

void VertexArray::resetNodes() {
	for (size_t i = 0; i < this->nodes.size(); i++)
		this->nodes[i].color = this->default_color;
}
