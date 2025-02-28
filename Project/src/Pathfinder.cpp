#include "pathfinding/Pathfinder.hpp"

#include <set>

#include "utilities/Functions.hpp"
#include "core/Macros.hpp"

////////////////////////////
// Pathfinder Definitions //
////////////////////////////

bool Pathfinder::FindPath(Nodes& nodes, Node& start, Node& finish, QuadMap* quadMap) {
	for (auto& n : nodes)
		n.Path = false;

	std::set<Node*> OpenSet;
	std::set<Node*> ClosedSet;

	Node* node = nullptr;

	OpenSet.insert(&start);

	while (!OpenSet.empty()) {
		node = *(OpenSet.begin());

		for (auto& o : OpenSet)
			if (o->FCost < node->FCost)
				node = o;

		OpenSet.erase(node);

		ClosedSet.insert(node);

		NeighbourNodes neigbours = GetNeighbourNodes(nodes, *node);

		ColorNeighbourNodes(neigbours, quadMap);

		for (auto& n : neigbours) {
			if (!n)
				continue;

			if (Contains(ClosedSet, n))
				continue;

			int unsureGCost = node->GCost + CalculateDistance(*node, *n);
			if (unsureGCost < n->GCost || !Contains(OpenSet, n)) {
				if (*n == finish) {

					finish.Parent = node;

					ReconstructPath(node, &start);

					return true;
				}

				if (!n->Walkable)
					continue;

				n->GCost = unsureGCost;
				n->HCost = CalculateDistance(*n, finish);
				n->FCost = n->GCost + n->HCost;
				n->Parent = node;

				if (!Contains(OpenSet, n))
					OpenSet.insert(n);
			}
		}
	}

	return false;
}

void Pathfinder::ColorNeighbourNodes(NeighbourNodes& nodes, QuadMap* quadMap) {
	if (!quadMap)
		return;

	for (auto& n : nodes) {
		if (!n || !n->Walkable)
			continue;

		//quadMap->Set(sf::Vector2u(n->x, n->y), sf::Color(58, 186, 128, 220));
		quadMap->Set(sf::Vector2u(n->x, n->y), sf::Color(58, 186, 128, 220));
	}
}

void Pathfinder::ReconstructPath(Node* start, Node* finish) {
	Node* node = start;

	while (*node != *finish) {
		node->Path = true;
		node = node->Parent;
	}
}

Pathfinder::NeighbourNodes Pathfinder::GetNeighbourNodes(Nodes& nodes, Node& currentNode) const {
	auto& count = global::NODE_COUNT;

	NeighbourNodes Neighbours;

	// simplified it can yield errors although it shouldn't
	auto CheckNode = [&nodes, &currentNode](Node* node)->bool {
		uint32_t index0 = In2D(node->x, currentNode.y);
		uint32_t index1 = In2D(currentNode.x, node->y);

		if (!nodes[index0].Walkable && !nodes[index1].Walkable)
			return false;

		return true;
	};

	uint32_t i = 0;

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (!x && !y)
				continue;

			int offsetX = currentNode.x + x;
			int offsetY = currentNode.y + y;

			if (CheckOffset(offsetX, offsetY)) {
				Neighbours[i] = nullptr;
				i++;
				continue;
			}

			uint32_t index = In2D(offsetX, offsetY);

			if (!CheckNode(&nodes[index])) {
				Neighbours[i] = nullptr;
				i++;
				continue;
			}

			Neighbours[i] = &nodes[index];
			i++;
		}
	}

	return Neighbours;
}

uint32_t Pathfinder::CalculateDistance(Node& node0, Node& node1) const {
	/*int x = abs((int64_t)node0.x - (int64_t)node1.x);
	int y = abs((int64_t)node0.y - (int64_t)node1.y);

	return x > y ? 14 * y + 10 * (x - y) : 14 * x + 10 * (y - x);*/

	int dstX = abs((int64_t)node0.x - (int64_t)node1.x);
	int dstY = abs((int64_t)node0.y - (int64_t)node1.y);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}

// Returns true if array out of bounds
bool Pathfinder::CheckOffset(int offsetX, int offsetY) {
	auto& count = global::NODE_COUNT;

	return (uint32_t)offsetX > count.x - 1 || (uint32_t)offsetY > count.y - 1 || offsetX < 0 || offsetY < 0;
}

/////////////////////////////////////
// Animated Pathfinder Definitions //
/////////////////////////////////////

std::pair<bool, AnimatedPathfindState> AnimatedPathfinder::FindPath(Nodes& nodes, Node& start, Node& finish, QuadMap* quadMap) {
	m_quadMap = quadMap;

	DB_ASSERT(m_quadMap);

	Reset(nodes, start);

	if (m_pathFound) {
		bool good = ReconstructPath(&start);

		if (!good)
			m_running = false;

		return std::make_pair
		(
			good,
			AnimatedPathfindState::Found
		);
	}

	if (m_OpenSet.empty()) {
		return std::make_pair
		(
			false,
			AnimatedPathfindState::NotFound
		);
	}

	m_node = *(m_OpenSet.begin());

	for (auto& o : m_OpenSet)
		if (o->FCost < m_node->FCost)
			m_node = o;

	m_OpenSet.erase(m_node);

	m_ClosedSet.insert(m_node);

	NeighbourNodes neigbours = GetNeighbourNodes(nodes, *m_node);

	ColorNeighbourNodes(neigbours, quadMap);

	for (auto& n : neigbours) {
		if (!n)
			continue;

		if (Contains(m_ClosedSet, n))
			continue;

		int unsureGCost = m_node->GCost + CalculateDistance(*m_node, *n);
		if (unsureGCost < n->GCost || !Contains(m_OpenSet, n)) {
			if (*n == finish) {

				finish.Parent = m_node;

				m_pathFound = true;

				return std::make_pair
				(
					true,
					AnimatedPathfindState::Running
				);
			}

			if (!n->Walkable)
				continue;

			n->GCost = unsureGCost;
			n->HCost = CalculateDistance(*n, finish);
			n->FCost = n->GCost + n->HCost;
			n->Parent = m_node;

			if (!Contains(m_OpenSet, n))
				m_OpenSet.insert(n);
		}
	}

	return std::make_pair
	(
		true,
		AnimatedPathfindState::Running
	);
}

void AnimatedPathfinder::Restart() {
	m_running = false;
}

bool AnimatedPathfinder::ReconstructPath(Node* finish) {
	if(*m_node != *finish) {
		m_quadMap->Set(sf::Vector2u(m_node->x, m_node->y), sf::Color::White);

		m_node->Path = true;
		m_node = m_node->Parent;

		return true;
	}

	return false;
}

void AnimatedPathfinder::Reset(Nodes& nodes, Node& start) {
	if (m_running)
		return;

	for (auto& n : nodes)
		n.Path = false;

	m_OpenSet.clear();
	m_ClosedSet.clear();

	m_OpenSet.insert(&start);

	m_node = &start;

	m_running   = true;
	m_pathFound = false;
}
