#pragma once

#include "core/Runtime.hpp"

#include "Quad Map.hpp"

#include "Hight Resolution Clock.hpp"

#include "GUI/Layer.hpp"

#include "pathfinding/Pathfinder.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Window/Mouse.hpp"

struct UniqueNode {
	sf::Vector2u Coords;
	sf::RectangleShape Body;

	void Render();
};

class Application : public Runtime {
public:
	Application();
	~Application() = default;

private:
	constexpr static sf::Vector2f s_wallInvalid = sf::Vector2f(-1.0f, -1.0f);

	HightResolutionClock m_pathfindClock;

	sf::RectangleShape m_background;

	// Graphical user interface stuff
	std::unordered_map<std::string, sf::Text> m_Texts;
	sf::RectangleShape m_ghostNode;
	Layer m_guiLayer;

	sf::Vector2f m_wallPlace[2]  = { s_wallInvalid, s_wallInvalid };
	sf::Vector2f m_wallRemove[2] = { s_wallInvalid, s_wallInvalid };

	UniqueNode m_startNode;
	UniqueNode m_endNode;

	// Stores adress of node picked by user (start or end)
	UniqueNode* m_currentPickedNode = nullptr;
	
	QuadMap m_quadMap; // Graphical representation of node graph
	NodeMap	m_nodeMap; // Structural representation of node graph

	AnimatedPathfinder m_animatedPathfinder;
	Pathfinder		   m_pathfinder;

	uint32_t m_tickCount = 100;

	bool m_animatedPathfind = false;

	float m_lastPathfindTime = 0.0f;

	virtual void Update() override;
	virtual void Render() override;

	void ClearOldPath();

	void InstantPathfind();
	void AnimatedPathfind();

	void UpdateSpeedText();
	void UpdatePathfindTimeText();

	bool UpdateWalls(sf::Mouse::Button button, sf::Color color, sf::Vector2f& start, sf::Vector2f& end, bool nodeWalkable = false);

	void UpdateShaders();

	bool UpdateUniqueNodes(UniqueNode** nodes, uint32_t count = 2);

	void LoadAssets();

	void Init();
	void InitGUI();

	void ThrowError(const std::wstring& title, const std::wstring& msg, bool quit = true);

	sf::Vector2f GetMousePos() const;
	sf::Vector2u MapPosToCoords(sf::Vector2f pos) const;

	bool CheckBounds(sf::Vector2f point) const;

	void DrawGhostNode();

	virtual void CreateWindow() override;
};
