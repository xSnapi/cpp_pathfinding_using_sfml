#pragma once

#include <string>

#include "Button.hpp"

#include "SFML/Graphics/Text.hpp"
#include "SFML/Graphics/RectangleShape.hpp"

class Message {
public:
	Message(const std::string& msg);
	~Message();

	void Update();
	void Render();

	bool Alive() const;

private:
	bool m_alive = true;

	Button* m_closeButton = nullptr; // Button doesn't have default constructor or move functions so we use pointer for that

	sf::Text m_text;

	sf::RectangleShape m_body;

	void CreateText(const std::string& msg);
	void CreateBody();
	void CreateButton();
};
