#pragma once
#include "stdafx.h"

class Button : public sf::Drawable {
	sf::Font* font = nullptr;
	sf::SoundBuffer* buffer = nullptr;
	sf::Text text;
	sf::Color default_color;

	bool is_pressed = false;

	bool is_button_pressed = false;

	void hover();
	void unhover();
	void checkForPress(const sf::Vector2f mouse_position);

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void initBody(const std::string text, const sf::Vector2f position);
	void initSound();
public:
	Button(sf::Font* font = nullptr, const std::string text = "", const sf::Vector2f position = sf::Vector2f(), const sf::Color color = sf::Color(255, 255, 255, 255), sf::SoundBuffer* buffer = nullptr);
	~Button();

	void update(const sf::Vector2f mouse_position);

	//Accessors
	bool isPressed();
};