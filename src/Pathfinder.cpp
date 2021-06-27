#include "stdafx.h"
#include "Pathfinder.hpp"

void Pathfinder::reconstructPath(Node* finish_node, Node* start_node) {
	Node* node = finish_node;
	while (*node != *start_node) {
		node->path = true;
		node = node->parent;
	}
}

std::vector<Node*> Pathfinder::getNeighbourNodes(std::vector<Node>& nodes, Node* current_node, bool diagonal) const {
	std::vector<Node*> neighbour_nodes;

	if (!diagonal) {
		auto n = [&nodes, &neighbour_nodes, &current_node](int x, int y) {
			const int X = current_node->x + x;
			const int Y = current_node->y + y;

			if (X > SIZE_X - 1 || Y > SIZE_Y - 1 || X < 0 || Y < 0)
				return;

			const int index = Y * SIZE_X + X;

			if (index >= nodes.size() || index < 0)
				return;

			auto* node = &nodes[index];

			neighbour_nodes.push_back(node);
		};

		for (int y = -1; y <= 1; y++) {
			if (y == 0)
				continue;
			n(0, y);
		}

		for (int x = -1; x <= 1; x++) {
			if (x == 0)
				continue;
			n(x, 0);
		}

		return neighbour_nodes;
	}

	for (int y = -1; y <= 1; y++) {
		for (int x = -1; x <= 1; x++) {
			if (x == 0 && y == 0)
				continue;

			const int X = current_node->x + x;
			const int Y = current_node->y + y;

			if (X > SIZE_X - 1 || Y > SIZE_Y - 1 || X < 0 || Y < 0)
				continue;

			const int index = this->getIndex(X, Y);

			if (index >= nodes.size() || index < 0)
				continue;

			auto* node = &nodes[index];

			neighbour_nodes.push_back(node);
		}
	}

	return neighbour_nodes;
}

int Pathfinder::calculateDistance(const Node& n1, const Node& n2) const {
	const int X = abs(static_cast<int>(n1.x - n2.x));
	const int Y = abs(static_cast<int>(n1.y - n2.y));

	if (X > Y)
		return 14 * Y + 10 * (X - Y);
	return 14 * X + 10 * (Y - X);
}

int Pathfinder::getIndex(const int X, const int Y) const {
	return Y * SIZE_X + X;
}

Pathfinder::Pathfinder() {

}

Pathfinder::~Pathfinder() {

}

bool Pathfinder::startPathfinding(std::vector<Node>& nodes, float& pathfind_time, bool diagonal, Node* start_node, Node* end_node) {
	if (start_node == nullptr || end_node == nullptr)
		return false;

	FixedClock clock;
	clock.getElapsedTime();

	List open_set(start_node);
	List closed_set;

	Node* node;

	while (open_set.size() > 0) {
		node = open_set[0];
		int node_id = 0;

		for (int i = 1; i < open_set.size(); i++) {
			auto* current_node = open_set[i];
			if (current_node->f_cost < node->f_cost) {
				node = current_node;
				node_id = i;
			}
		}

		open_set.remove(node_id);
		closed_set.push_back(node);

		for (auto* n : this->getNeighbourNodes(nodes, node, diagonal)) {
			if (closed_set.contains(*n))
				continue;

			const int tentative_g_cost = node->g_cost + this->calculateDistance(*node, *n);
			if (tentative_g_cost < n->g_cost || !open_set.contains(*n)) {
				if (*n == *end_node) {
					pathfind_time = clock.getElapsedTime();
					this->reconstructPath(node, start_node);
					return true;
				}

				if (!n->walkable)
					continue;

				n->g_cost = tentative_g_cost;
				n->h_cost = this->calculateDistance(*n, *end_node);
				n->f_cost = n->g_cost + n->h_cost;
				n->parent = node;

				if (!open_set.contains(*n))
					open_set.push_back(n);
			}
		}
	}

	pathfind_time = clock.getElapsedTime();

	return false;
}
