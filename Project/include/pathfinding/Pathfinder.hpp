#pragma once

#include <array>
#include <set>

#include "Node Map.hpp"
#include "Quad Map.hpp"

class Pathfinder {
public:
	Pathfinder()  = default;
	virtual ~Pathfinder() = default;

	bool FindPath(Nodes& nodes, Node& start, Node& finish, QuadMap* quadMap = nullptr);

protected:
	typedef std::array<Node*, 8> NeighbourNodes;

	void ColorNeighbourNodes(NeighbourNodes& nodes, QuadMap* quadMap = nullptr);

	void ReconstructPath(Node* finish, Node* start);
	NeighbourNodes GetNeighbourNodes(Nodes& nodes, Node& currentNode) const;

	inline uint32_t CalculateDistance(Node& node0, Node& node1) const;

	static bool CheckOffset(int offsetX, int offsetY);
};

enum class AnimatedPathfindState {
	Running = 0,
	Found,
	NotFound
};

class AnimatedPathfinder : public Pathfinder {
public:
	AnimatedPathfinder()  = default;
	~AnimatedPathfinder() = default;

	std::pair<bool, AnimatedPathfindState> FindPath(Nodes& nodes, Node& start, Node& finish, QuadMap* quadMap = nullptr);
	void Restart();

private:
	bool m_running = false;
	bool m_pathFound = false;

	QuadMap* m_quadMap = nullptr;

	std::set<Node*> m_OpenSet;
	std::set<Node*> m_ClosedSet;

	Node* m_node = nullptr;

	bool ReconstructPath(Node* finish);

	void Reset(Nodes& nodes, Node& start);
};
