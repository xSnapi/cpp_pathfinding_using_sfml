#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/String.hpp"
#include "SFML/System/Clock.hpp"

struct TimeSteps;

class Runtime {
public:
	Runtime();
	virtual ~Runtime();

	static Runtime* Instance;

	void Run();
	void Shutdown();

	bool ShaderAvailable() const;

	sf::RenderWindow& GetWindow();

protected:
	virtual void Update() = 0;
	virtual void Render() = 0;

	virtual void CreateWindow();

	virtual void HandleEvents();

	bool m_shaderAvailable = true;

	sf::RenderWindow m_window;

private:
	void UpdatePass();
	void RenderPass();

	void Init();

	bool m_running = true;

	static float s_deltaTime;
	static long double s_elapsedTime;

	sf::Clock m_deltaClock;

	friend struct TimeSteps;
};
