#pragma once

#include <vector>

#include "Node.hpp"

#include "Global Variables.hpp"

#include "SFML/System/Vector2.hpp"

typedef std::vector<Node> Nodes;

class NodeMap {
public:
	NodeMap()  = default;
	~NodeMap() = default;

	void Create();
	void Reset();

	void Destroy();

	Nodes& GetNodes();

	Node& operator()(uint32_t x, uint32_t y);
	Node& operator()(sf::Vector2u coords);

private:
	Nodes m_Nodes;
};
