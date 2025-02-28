#include "Application.hpp"

#include "core/Renderer.hpp"
#include "SFML/Graphics.hpp"

#include "core/Time Steps.hpp"
#include "core/Macros.hpp"

#include "GUI/Button.hpp"
#include "GUI/Slider.hpp"
#include "GUI/Message.hpp"

#include "utilities/Resource Manager.hpp"
#include "utilities/Windows MSG Box.hpp"

#include "Global Variables.hpp"

/////////////////////////////
// Application Definitions //
/////////////////////////////

Application::Application() {
	LoadAssets();
	Init();
	InitGUI();
}

void Application::Update() {
	static bool wallSetUpdate = false;

	UpdateShaders();

	m_guiLayer.Update();

	if (m_guiLayer.MessageActive() || m_guiLayer.BlocksInput())
		return;

	bool uniqueNodeUpdate = false;

	UniqueNode* nodes[2] = 
	{
		&m_startNode,
		&m_endNode,
	};

	if (!wallSetUpdate)
		uniqueNodeUpdate = UpdateUniqueNodes(nodes);

	if (!m_animatedPathfind) {
		if (!uniqueNodeUpdate)
			wallSetUpdate = UpdateWalls(sf::Mouse::Button::Left, sf::Color(13, 23, 64), m_wallPlace[0], m_wallPlace[1]);

		if (!wallSetUpdate)
			UpdateWalls(sf::Mouse::Button::Right, sf::Color::Transparent, m_wallRemove[0], m_wallRemove[1], true);
	}

	UpdateSpeedText();
	AnimatedPathfind();
}

void Application::Render() {
	// Drawing background
	Renderer.Draw(m_background);

	m_quadMap.Render();

	DrawGhostNode();

	// Drawing unique nodes (start, end node)
	m_startNode.Render();
	m_endNode.Render();

	for (auto& [name, t] : m_Texts)
		Renderer.Draw(t);

	m_guiLayer.Render();
}

void Application::Init() {
	m_background.setSize(global::MAP_SIZE);
	m_background.setFillColor(sf::Color(67, 100, 148));

	m_nodeMap.Create();
	m_quadMap.Create();

	// Ghost node
	{
		m_ghostNode.setSize(global::NODE_SIZE);
	}

	// Pathfinding nodes
	{
		m_startNode.Body.setSize(global::NODE_SIZE);
		m_startNode.Body.setFillColor(sf::Color::Green);

		m_startNode.Coords = sf::Vector2u(5, global::NODE_COUNT.y / 2);

		m_endNode.Body.setSize(global::NODE_SIZE);
		m_endNode.Body.setFillColor(sf::Color::Red);

		m_endNode.Coords = sf::Vector2u(global::NODE_COUNT.x - 5 - 1, global::NODE_COUNT.y / 2);
	}

	// Speed Text
	{
		// sf::Text doesn't have default constructor so we do a little trick here
		m_Texts.emplace("animation_speed", FontManager["default"]);

		auto& t = m_Texts.at("animation_speed");

		t.setPosition(sf::Vector2f(520.0f, 640.0f));
		t.setString("Animation Speed: 50");
	}

	// Pathfind Time
	{
		m_Texts.emplace("pathfind_time", FontManager["default"]);

		auto& t = m_Texts.at("pathfind_time");

		t.setPosition(sf::Vector2f(520.0f, 720.0f));
		t.setString("Pathfind took: 0.0000ms");
	}
}

void Application::InitGUI() {
	// Start Button
	{
		Button* startButton = new Button("Start Pathfinding", sf::Vector2f(170.0f, 670.0f), sf::Vector2f(260.0f, 55.0f));
		startButton->OnPress = [&] {
			LOG("Pathfinding started\n");

			// This will work because of widget system although stop and reset buttons don't exist at this point yet
			((Button*)m_guiLayer.GetWidget("start_button"))->SetActive(false);
			((Button*)m_guiLayer.GetWidget("stop_button"))->SetActive(true);
			((Button*)m_guiLayer.GetWidget("reset_button"))->SetActive(false);

			float progress = ((Slider*)m_guiLayer.GetWidget("speed_slider"))->GetProgress();

			ClearOldPath();

			if (progress == 1.0f)
				InstantPathfind();
			else {
				m_animatedPathfinder.Restart();
				m_animatedPathfind = true;
			}
		};

		m_guiLayer.AddWidget("start_button", startButton);
	}

	// Stop Button
	{
		Button* stopButton = new Button("Stop", sf::Vector2f(90.0f, 750.0f), sf::Vector2f(100.0f, 55.0f));
		stopButton->OnPress = [&] {
			LOG("Pathfinding stopped\n");

			((Button*)m_guiLayer.GetWidget("start_button"))->SetActive(true);
			((Button*)m_guiLayer.GetWidget("stop_button"))->SetActive(false);
			((Button*)m_guiLayer.GetWidget("reset_button"))->SetActive(true);

			m_animatedPathfind = false;
		};

		stopButton->SetActive(false);

		m_guiLayer.AddWidget("stop_button", stopButton);
	}

	// Reset Button
	{
		Button* resetButton = new Button("Reset", sf::Vector2f(90.0f, 830.0f), sf::Vector2f(100.0f, 55.0f));
		resetButton->OnPress = [&] {
			LOG("Reset\n");

			m_quadMap.Reset();
			m_nodeMap.Reset();

			m_startNode.Coords = sf::Vector2u(5, global::NODE_COUNT.y / 2);
			m_endNode.Coords   = sf::Vector2u(global::NODE_COUNT.x - 5 - 1, global::NODE_COUNT.y / 2);
		};

		m_guiLayer.AddWidget("reset_button", resetButton);
	}

	// Info Button
	{
		Button* infoButton = new Button("Info", sf::Vector2f(1100.0f, 830.0f), sf::Vector2f(100.0f, 55.0f));
		infoButton->OnPress = [&] {
			Message* message = new Message
			(
				R"(Press LEFT Mouse button to place walls
Press RIGHT Mouse button to remove walls

Green Node - Pathfinding start point
Red Node - Pathfinding end point

Simply drag and drop Green or Red nodes to change it positions

Buttons:
"Start Pathfinding" - Starts pathfinding
"Stop" - Stops pathfinding animation
"Reset" - Resets map to default state

"Animation Speed Slider" - changes animation speed,
 where left most position is slowest,
 and right most position is instant
)"
			);

			m_guiLayer.PushMessage(message);
		};

		m_guiLayer.AddWidget("info_button", infoButton);
	}

	// Speed Slider
	{
		Slider* speedSlider = new Slider(sf::Vector2f(1000.0f, 660.0f));

		m_guiLayer.AddWidget("speed_slider", speedSlider);
	}

	// Welcome Message
	{
		Message* message = new Message("            Welcome to A* Pathfinding!\n\nFor more information press \"Info\" Button");

		m_guiLayer.PushMessage(message);
	}
}

void Application::ThrowError(const std::wstring& title, const std::wstring& msg, bool quit) {
	int retVal = CustomMessageBox(msg.c_str(), title.c_str(), 0x00000000L | 0x00001000L);

	if(quit)
		Runtime::Instance->Shutdown();
}

sf::Vector2f Application::GetMousePos() const {
	return (sf::Vector2f)m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
}

sf::Vector2u Application::MapPosToCoords(sf::Vector2f pos) const {
	// Fatal error point was out of map bounds.
	DB_ASSERT(!CheckBounds(pos));

	return 
	{
		uint32_t(pos.x / 20),
		uint32_t(pos.y / 20)
	};
}

void Application::DrawGhostNode() {
	sf::Vector2f mousePos = GetMousePos();

	if (CheckBounds(mousePos))
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
		m_ghostNode.setFillColor(sf::Color(100, 0, 0, 100));
	else
		m_ghostNode.setFillColor(sf::Color(0, 0, 0, 100));

	m_ghostNode.setPosition(sf::Vector2f(MapPosToCoords(mousePos)).cwiseMul(global::NODE_SIZE));

	Renderer.Draw(m_ghostNode);
}

void Application::ClearOldPath() {
	for (auto& n : m_nodeMap.GetNodes()) {
		if (!n.Walkable)
			continue;

		m_quadMap.Set(sf::Vector2u(n.x, n.y), sf::Color::Transparent);
	}
}

void Application::InstantPathfind() {
	auto& startNode = m_nodeMap(m_startNode.Coords);
	auto& endNode   = m_nodeMap(m_endNode.Coords);

	m_pathfindClock.Start();

	bool pathfound = m_pathfinder.FindPath(m_nodeMap.GetNodes(), startNode, endNode, &m_quadMap);

	m_pathfindClock.Stop();

	m_lastPathfindTime = m_pathfindClock.GetTimeAsMilliseconds();

	UpdatePathfindTimeText();

	for (auto& n : m_nodeMap.GetNodes()) {
		if (!n.Path)
			continue;

		m_quadMap.Set(sf::Vector2u(n.x, n.y), sf::Color::White);
	}

	((Button*)m_guiLayer.GetWidget("start_button"))->SetActive(true);
	((Button*)m_guiLayer.GetWidget("stop_button"))->SetActive(false);
	((Button*)m_guiLayer.GetWidget("reset_button"))->SetActive(true);

	if (pathfound)
		return;

	Message* message = new Message("Path not found!");
	m_guiLayer.PushMessage(message);
}

void Application::AnimatedPathfind() {
	constexpr float TICK_TIME = 0.0005f;
	static float	pathfindET = 0.0f;
	static uint32_t tickCount = 0;

	if (!m_animatedPathfind)
		return;

	pathfindET += dt;

	while (pathfindET >= TICK_TIME) {
		pathfindET -= TICK_TIME;
		tickCount++;

		if (tickCount < m_tickCount)
			continue;

		tickCount = 0;

		auto& startNode = m_nodeMap(m_startNode.Coords);
		auto& endNode   = m_nodeMap(m_endNode.Coords);

		auto [good, state] = m_animatedPathfinder.FindPath(m_nodeMap.GetNodes(), startNode, endNode, &m_quadMap);

		m_animatedPathfind = good;

		if (state == AnimatedPathfindState::NotFound) {
			Message* message = new Message("Path not found!");
			m_guiLayer.PushMessage(message);
		}

		if (!m_animatedPathfind) {
			LOG("Animated pathfind finished\n");
			((Button*)m_guiLayer.GetWidget("start_button"))->SetActive(true);
			((Button*)m_guiLayer.GetWidget("stop_button"))->SetActive(false);
			((Button*)m_guiLayer.GetWidget("reset_button"))->SetActive(true);

			break;
		}
	}

	// Pathfinding is still running
	if (m_animatedPathfind)
		return;

	// Getting time for our pathfind (highly inefficient but it is the easiest way to do this)
	{
		auto& startNode = m_nodeMap(m_startNode.Coords);
		auto& endNode = m_nodeMap(m_endNode.Coords);

		m_pathfindClock.Start();

		m_pathfinder.FindPath(m_nodeMap.GetNodes(), startNode, endNode);

		m_pathfindClock.Stop();

		m_lastPathfindTime = m_pathfindClock.GetTimeAsMilliseconds();
	}

	UpdatePathfindTimeText();
}

void Application::UpdateSpeedText() {
	float progress = ((Slider*)m_guiLayer.GetWidget("speed_slider"))->GetProgress();
	auto& text = m_Texts.at("animation_speed");

	if (progress == 1.0f) {
		text.setString("Animation Speed: Instant");
		text.setPosition(sf::Vector2f(480.0f, 640.0f));
		return;
	}

	int val = Lerp(100, 10, progress);

	m_tickCount = val;

	text.setPosition(sf::Vector2f(520.0f, 640.0f));
	text.setString("Animation Speed: " + std::to_string(val));
}

bool Application::UpdateWalls(sf::Mouse::Button button, sf::Color color, sf::Vector2f& start, sf::Vector2f& end, bool nodeWalkable) {
	sf::Vector2f mousePos = GetMousePos();

	auto Reset = [&] {
		start = s_wallInvalid;
		end   = s_wallInvalid;
	};

	if (CheckBounds(mousePos)) {
		Reset();
		return false;
	}

	if (!sf::Mouse::isButtonPressed(button)) {
		if (start != s_wallInvalid) {
			sf::Vector2u coords = MapPosToCoords(start);

			m_quadMap.Set(coords, color);
			m_nodeMap(coords).Walkable = nodeWalkable;
		}

		Reset();
		return false;
	}

	if (start == s_wallInvalid) {
		start = mousePos;
		return true;
	}

	if (end == s_wallInvalid) {
		end = mousePos;
		return true;
	}

	sf::Vector2u startCoords = MapPosToCoords(start);
	sf::Vector2u endCoords   = MapPosToCoords(end);

	if (startCoords == endCoords) {
		m_quadMap.Set(startCoords, color);
		m_nodeMap(startCoords).Walkable = nodeWalkable;
	}

	int dx = (int)endCoords.x - (int)startCoords.x;
	int dy = (int)endCoords.y - (int)startCoords.y;

	int step = abs(dx);

	if (abs(dy) > abs(dx))
		step = abs(dy);

	float stepXIncrement = (float)dx / step;
	float stepYIncrement = (float)dy / step;

	sf::Vector2f coords = (sf::Vector2f)startCoords;

	auto Round = [](float v) {
		return (v - (int)v < 0.5f) ? (int)v : (int)(v + 1.0f);
	};

	for (uint32_t i = 0; i < step; i++) {
		sf::Vector2u nodeCoords = sf::Vector2u(Round(coords.x), Round(coords.y));

		m_quadMap.Set(nodeCoords, color);
		m_nodeMap(nodeCoords).Walkable = nodeWalkable;

		coords.x += stepXIncrement;
		coords.y += stepYIncrement;
	}

	start = end;
	end   = s_wallInvalid;

	return true;
}

void Application::UpdateShaders() {
	ShaderManager["gradient"].setUniform("u_time", (float)et);
}

void Application::UpdatePathfindTimeText() {
	// This is bad (inefficient)
	std::string timeText = std::to_string(m_lastPathfindTime);

	// This can be terrible (or not?)
	timeText.resize(timeText.size() - 2);

	m_Texts.at("pathfind_time").setString("Pathfind took: " + timeText + "ms");
}

bool Application::UpdateUniqueNodes(UniqueNode** nodes, uint32_t count) {
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
		m_currentPickedNode = nullptr;
		return false;
	}

	sf::Vector2f mousePos = GetMousePos();

	if (!m_currentPickedNode) {
		for (uint32_t i = 0; i < count; i++) {
			if (!nodes[i]->Body.getGlobalBounds().contains(mousePos))
				continue;

			m_currentPickedNode = nodes[i];
			break;
		}

		if (!m_currentPickedNode)
			return false;
	}

	if (CheckBounds(mousePos)) {
		m_currentPickedNode = nullptr;
		return false;
	}

	if(m_currentPickedNode)
		m_currentPickedNode->Coords = MapPosToCoords(mousePos);

	return true;
}

void Application::LoadAssets() {
	// Load Fonts
	{
		bool good = FontManager.Load("default", "res/default.ttf");

		if(!good)
			ThrowError(L"Fatal Error", L"Filed To Load Font: \"res/default.ttf\"");
	}

	if (!ShaderAvailable())
		return;

	// Load Shaders
	{
		bool good = ShaderManager.LoadFromFile("gradient", "res/default.vert", "res/gradient.frag");

		if (!good) {
			ThrowError(L"Error", L"Filed To Load Shader:\n\"res/default.vert\"or\n\"res/gradient.frag\"\n\nRunning program in shaderless mode.", false);
			m_shaderAvailable = false;
		}
	}
}

void Application::CreateWindow() {
	sf::VideoMode mode;
	mode.size = sf::Vector2u(1200, 900);

	sf::ContextSettings settings;

	m_window.create(mode, "A* Pathfinding", sf::Style::Close, sf::State::Windowed, settings);
	m_window.setFramerateLimit(144);
}

bool Application::CheckBounds(sf::Vector2f point) const {
	return point.x < 0.0f || point.x >= global::MAP_SIZE.x || point.y < 0.0f || point.y >= global::MAP_SIZE.y;
}

/////////////////////////////
// Unique Node Definitions //
/////////////////////////////

void UniqueNode::Render() {
	Body.setPosition(sf::Vector2f(Coords).cwiseMul(global::NODE_SIZE));
	Renderer.Draw(Body);
}
