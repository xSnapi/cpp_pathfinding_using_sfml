#include "stdafx.h"
#include "Application.hpp"

void Application::startPathfinding() {
	Pathfinder pathfinder;
	this->resetPathNodes();
	pathfinder.startPathfinding(this->nodes, this->last_pathfind_time, this->diagonal, this->start_node, this->end_node);
	this->markPathNodes();
}

void Application::updateSingleNode(Node& node, const int index) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) { // placing starting node
		if (this->checkNode(node))
			return;

		if (!node.walkable) // checks if node isn't a wall
			return;

		node.walkable = false;

		if (this->start_node != nullptr) {
			this->resetNode(*this->start_node);
			this->vertex_array.setNodeColor(this->start_node->x, this->start_node->y, this->vertex_array.default_color);
		}
		this->start_node = &node;

		//TODO: delete this
		//std::cout << " Start node position: X - " << node.x << ", Y - " << node.y << '\n';

		this->resetPathNodes();
		this->vertex_array.setNodeColor(index, sf::Color::Red);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) { // placing ending node
		if (this->checkNode(node))
			return;

		if (!node.walkable) // checks if node isn't a wall
			return;

		node.walkable = false;

		if (this->end_node != nullptr) {
			this->resetNode(*this->end_node);
			this->vertex_array.setNodeColor(this->end_node->x, this->end_node->y, this->vertex_array.default_color);
		}
		this->end_node = &node;

		//TODO: delete this
		//std::cout << " End node position: X - " << node.x << ", Y - " << node.y << '\n';

		this->resetPathNodes();
		this->vertex_array.setNodeColor(index, sf::Color::Green);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) { // placing wall
		if (this->checkNode(node))
			return;

		node.walkable = false;
		this->vertex_array.setNodeColor(index, sf::Color::Black);
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) { // removing node
		if (this->start_node != nullptr && node == *this->start_node)
			this->start_node = nullptr;
		else if (this->end_node != nullptr && node == *this->end_node)
			this->end_node = nullptr;

		this->resetNode(node);
	}

	//TODO: delete this
	//debug stuff
	/*if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
		system("cls");

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::B) && !this->key_pressed) {
		system("cls");
		this->key_pressed = true;
		for (const auto& n : this->nodes)
			if (!n.walkable)
				std::cout << ' ' << n.x << ", " << n.y << " wall" << std::endl;
	}
	else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::B))
		this->key_pressed = false;*/
}

bool Application::checkNode(Node& node) const {
	if (this->start_node == nullptr && this->end_node == nullptr)
		return false;
	else if (this->end_node != nullptr && this->start_node != nullptr) 
		return node == *this->end_node || node == *this->start_node;
	else if (this->start_node != nullptr)
		return node == *this->start_node;
	else if (this->end_node != nullptr)
		return node == *this->end_node;
	return false;
}

int Application::getIndex(const int X, const int Y) const {
	return Y * SIZE_X + X;
}

sf::Vector2i Application::raycast() const {
	const sf::Vector2i mouse_position = static_cast<sf::Vector2i>(this->mouse_position);

	if (mouse_position.x < 0 || mouse_position.x >= WINDOW_WIDTH || mouse_position.y < 0 || mouse_position.y >= SIZE_Y * NODE_SIZE)
		return sf::Vector2i(-1, -1);

	const int X = mouse_position.x / NODE_SIZE;
	const int Y = mouse_position.y / NODE_SIZE;

	return sf::Vector2i(X, Y);
}

void Application::resetNode(Node& node) {
	node.f_cost		= -1;
	node.g_cost		= -1;
	node.h_cost		= -1;
	node.walkable		= true;
	node.path		= false;
	node.parent		= nullptr;

	this->vertex_array.setNodeColor(node.x, node.y, this->vertex_array.default_color);
}

void Application::resetPathNodes() {
	for (auto& n : this->nodes)
		if (n.path) {
			const int index = this->getIndex(n.x, n.y);

			n.path = false;

			if (n.walkable)
				this->vertex_array.setNodeColor(index, this->vertex_array.default_color);
		}
}

void Application::markPathNodes() {
	for (auto& n : this->nodes)
		if (n.path)
			this->vertex_array.setNodeColor(n.x, n.y, sf::Color::Blue);
}

void Application::updateNodes() {
	const sf::Vector2i pos = this->raycast();

	if (pos.x < 0 || pos.y < 0) // Checks if mouse isn't out of bounds
		return;

	const int index = this->getIndex(pos.x, pos.y);
	auto& node = this->nodes[index];

	this->updateSingleNode(node, index);
}

void Application::initNodes() {
	for (size_t y = 0u; y < SIZE_Y; y++) {
		for (size_t x = 0u; x < SIZE_X; x++)
			this->nodes.push_back(Node(x, y));
	}
}

void Application::updateButtons() {
	for (auto& b : this->Buttons)
		b.second.update(this->mouse_position);

	if (this->Buttons["start"].isPressed()) 
		this->startPathfinding();
	else if (this->Buttons["reset"].isPressed()) {
		for (auto& n : this->nodes)
			this->resetNode(n);

		this->start_node	= nullptr;
		this->end_node		= nullptr;

		//TODO: delete this
		//system("cls");
	}
	else if (this->Buttons["diagonal"].isPressed())
		this->diagonal = !this->diagonal;
}

void Application::initButtons() {
	const std::initializer_list <std::pair<const std::string, Button>> list
	{
		{
			"start", Button(&this->font, "START", sf::Vector2f(30.f, 635.f), sf::Color(50, 168, 82))
		},

		{
			"reset", Button(&this->font, "RESET", sf::Vector2f(30.f, 685.f), sf::Color(247, 40, 71))
		},

		{
			"diagonal", Button(&this->font, "DIAGONAL", sf::Vector2f(30.f, 735.f))
		},
	};

	this->Buttons = list;
}

void Application::updateTexts() {
	this->Texts["lpt"].setString("Last pathfind\ntime: " + std::to_string(this->last_pathfind_time));
	this->Texts["fps"].setString(std::to_string(this->FPS) + " FPS");

	(this->diagonal) ? this->Texts["diagonal"].setString("-ON") : this->Texts["diagonal"].setString("-OFF");
}

void Application::initTexts() {
	this->Texts["info"].setString("A* pathfinding\n algorithm\n\nHold A to\n place starting\n  node\n\nHold D to\n place finish\n  node");
	this->Texts["info"].setPosition(sf::Vector2f(440.f, 660.f));

	this->Texts["lpt"].setPosition(sf::Vector2f(30.f, 800.f)); // l p t - last pathfind time

	this->Texts["fps"].setPosition(sf::Vector2f(30.f, 850.f));

	this->Texts["diagonal"].setPosition(sf::Vector2f(350.f, 753.f));

	for (auto& t : this->Texts) {
		t.second.setFont(this->font);
		t.second.setCharacterSize(20);
	}
}

void Application::initFont() {
	this->font.loadFromFile("res/font.ttf");
}

void Application::updateDeltaTime() {
	this->delta_time = this->delta_clock.restart().asSeconds();
}

void Application::updateMousePosition() {
	this->mouse_position = this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));
}

void Application::updateFPS() {
	this->FPS = static_cast<unsigned int>(floor(1.f / this->FPS_clock.restart().asSeconds()));
}

void Application::handleEvents() {
	while (this->window.pollEvent(this->event)) {
		switch (this->event.type) {
		case sf::Event::Closed: { this->window.close(); }
		}
	}
}

void Application::initWindow() {
	sf::ContextSettings context_settings;

	this->video_mode.width	= WINDOW_WIDTH;
	this->video_mode.height = WINDOW_HEIGHT;

	this->window.create(this->video_mode, "Pathfinding 5", sf::Style::Close, context_settings);
	//this->window.setFramerateLimit(240);
}

Application::Application() {
	this->initWindow();

	this->initFont();
	this->initTexts();
	this->initButtons();
	this->initNodes();
}

Application::~Application() {

}

void Application::update() {
	this->handleEvents();
	this->updateMousePosition();

	this->updateButtons();
	this->updateTexts();
	this->updateNodes();

	//TODO: debug delete
	/*if (this->start_node != nullptr)
		std::cout << [&]()->std::string {if (end_node != nullptr) return "amogusy 2"; return "sus"; }() << std::endl;*/
}

void Application::render() {
	this->window.clear();

	this->window.draw(this->vertex_array);

	// Drawing text array
	for (auto& t : this->Texts)
		this->window.draw(t.second);
	
	// Drawing buttons array
	for (auto& b : this->Buttons)
		this->window.draw(b.second);

	this->window.display();

	this->updateDeltaTime();
	this->updateFPS();
}

bool Application::isRunning() const {
	return this->window.isOpen();
}
