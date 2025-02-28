#pragma once

#include <cstdint>

struct Node {
	Node(uint32_t x, uint32_t y)
		: x(x), y(y)
	{ }

	~Node() = default;

	struct {
		uint32_t x = 0;
		uint32_t y = 0;

		bool Walkable = true;
		bool Path	  = false;

		int FCost = -1;
		int GCost = -1;
		int HCost = -1;

		Node* Parent = nullptr;
	};

	bool operator == (const Node& other) const {
		return x == other.x && y == other.y;
	}

	bool operator != (const Node& other) const {
		return x != other.x || y != other.y;
	}

	void Reset() {
		Walkable = true;
		Path = false;

		FCost = -1;
		GCost = -1;
		HCost = -1;

		Parent = nullptr;
	}
};
