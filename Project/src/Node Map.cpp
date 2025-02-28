#include "pathfinding/Node Map.hpp"

#include "Global Variables.hpp"

#include "utilities/Functions.hpp"

void NodeMap::Create() {
	sf::Vector2u count = global::NODE_COUNT;

	for (uint32_t y = 0; y < count.y; y++) {
		for (uint32_t x = 0; x < count.x; x++) {
			m_Nodes.emplace_back
			(
				x,
				y
			);
		}
	}
}

void NodeMap::Reset() {
	for (auto& n : m_Nodes)
		n.Reset();
}

void NodeMap::Destroy() {
	m_Nodes.clear();
}

Nodes& NodeMap::GetNodes() {
	return m_Nodes;
}

Node& NodeMap::operator()(uint32_t x, uint32_t y) {
	return m_Nodes[In2D(x, y)];
}

Node& NodeMap::operator()(sf::Vector2u coords) {
	return m_Nodes[In2D(coords.x, coords.y)];
}
