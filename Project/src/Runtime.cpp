#include "core/Runtime.hpp"

#include "core/Time Steps.hpp"
#include "core/Renderer.hpp"
#include "core/Macros.hpp"

#include "utilities/Resource Manager.hpp"

#include "SFML/Window/Event.hpp"

////////////////////////////
// Time Steps Definitions //
////////////////////////////

const long double& et = TimeSteps::ElapsedTime;
const float&	   dt = TimeSteps::DeltaTime;

const long double& TimeSteps::ElapsedTime = Runtime::s_elapsedTime;
const float&	   TimeSteps::DeltaTime	  = Runtime::s_deltaTime;

/////////////////////////
// Runtime Definitions //
/////////////////////////

long double Runtime::s_elapsedTime = 0.0f;
float		Runtime::s_deltaTime   = 0.25f;

Runtime* Runtime::Instance = nullptr;

Runtime::Runtime() {
	// Trying to create multiple Runtime instances (Runtime can only have 1 instance running)
	DB_ASSERT(!Instance);

	Instance = this;

	// Checking if shaders are available for this machine
	m_shaderAvailable = sf::Shader::isAvailable();
}

Runtime::~Runtime() {
	ShaderManager.Clear();
	FontManager.Clear();
}

void Runtime::Run() {
	Init();

	while(m_running) {
		if (s_deltaTime > 0.25f) // This prevents moments where we have 0-4 FPS in our app
			s_deltaTime = 0.25f;

		UpdatePass();
		HandleEvents();
		RenderPass();

		s_deltaTime = m_deltaClock.restart().asSeconds();

		s_elapsedTime += s_deltaTime;
	}

	m_window.close();
}

void Runtime::Shutdown() {
	m_running = false;
}

bool Runtime::ShaderAvailable() const {
	return m_shaderAvailable;
}

sf::RenderWindow& Runtime::GetWindow() {
	return m_window;
}

void Runtime::CreateWindow() {
	sf::VideoMode mode;
	mode.size = sf::Vector2u(640, 640);

	m_window.create(mode, "");
}

void Runtime::UpdatePass() {
	Update();
}

void Runtime::RenderPass() {
	Renderer.Clear();

	Render();

	Renderer.Flush();
}

void Runtime::HandleEvents() {
	sf::Event e;

	while(m_window.pollEvent(e)) {
		switch (e.type) {
		case sf::Event::Closed:
			Shutdown();
			break;
		}
	}
}

void Runtime::Init() {
	CreateWindow();

	Renderer.Init(m_window);		
}
