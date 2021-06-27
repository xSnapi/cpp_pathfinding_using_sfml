#pragma once
/* ~ A class that behaves as application engine ~ */

#include "stdafx.h"
#include "Vertex Array.hpp"
#include "Button.hpp"
#include "Public Variables.h"
#include "Node.h"
#include "Pathfinder.hpp"

class Application {
private:
	sf::RenderWindow window;
	sf::VideoMode video_mode;
	sf::Event event;

	sf::Clock delta_clock;
	sf::Clock FPS_clock;

	sf::Font font;

	sf::Vector2f mouse_position;

	VertexArray vertex_array;

	std::map<const std::string, sf::Text> Texts;
	std::map<const std::string, Button> Buttons;

	std::vector<Node>nodes;

	Node* start_node	= nullptr;
	Node* end_node		= nullptr;

	float delta_time = 1.f / 60.f;
	unsigned int FPS = 0;

	float last_pathfind_time = 0.f;

	bool diagonal = true;

	//TODO: delete this
	bool key_pressed = true;

	// Pathfinding stuff
	void startPathfinding();

	// Nodes stuff
	bool checkNode(Node& node) const;
	int getIndex(const int X, const int Y) const;
	sf::Vector2i raycast() const;
	void resetNode(Node& node);
	void resetPathNodes();
	void markPathNodes();
	
	void updateSingleNode(Node& node, const int index);
	void updateNodes();
	void initNodes();
	//

	void updateButtons();
	void initButtons();

	void updateTexts();
	void initTexts();
	void initFont();

	void updateMousePosition();
	void updateDeltaTime();
	void updateFPS();
	void handleEvents();
	void initWindow();
public:
	Application();
	~Application();

	void update();
	void render();

	//Accessors
	bool isRunning() const;
};