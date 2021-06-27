#pragma once

struct Node
{

	Node(unsigned _x, unsigned _y) 
		: x(_x), y(_y) {}

	struct 
	{

		unsigned x, y;

		bool walkable	= true;
		bool path	= false;

		int f_cost = -1; // G + H
		int g_cost = -1; // Distance from starting node
		int h_cost = -1; // Distance from end node

		Node* parent = nullptr;

	};
	
	bool operator == (const Node& n) const
	{
		return x == n.x && y == n.y;
	}

	bool operator != (const Node& n) const
	{
		return x != n.x || y != n.y;
	}

};
