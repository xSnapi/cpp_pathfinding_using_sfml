#include "stdafx.h"
#include "Button.hpp"

void Button::hover() {
	const sf::Color hover_color = sf::Color(this->default_color.r - 40, this->default_color.g - 40, this->default_color.b - 40, 255);
	this->text.setFillColor(hover_color);
}

void Button::unhover() {
	this->text.setFillColor(this->default_color);
}

void Button::checkForPress(const sf::Vector2f mouse_position) {
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !this->is_button_pressed) {
		this->is_pressed = true;
		this->is_button_pressed = true;
	}
	else if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
		this->is_button_pressed = false;
}

void Button::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(this->text);
}

void Button::initBody(const std::string text, const sf::Vector2f position) {
	this->text.setFont(*this->font);
	this->text.setString(text);
	this->text.setCharacterSize(40);
	this->text.setPosition(position);
	this->text.setFillColor(this->default_color);
}

void Button::initSound() {

}

Button::Button(sf::Font* font, const std::string text, const sf::Vector2f position, const sf::Color color, sf::SoundBuffer* buffer) {
	this->font			= font;
	this->buffer		= buffer;
	this->default_color = color;

	this->initBody(text, position);
	if (this->buffer != nullptr)
		this->initSound();
}

Button::~Button() {

}

void Button::update(const sf::Vector2f mouse_position) {
	(this->text.getGlobalBounds().contains(mouse_position)) ? (this->hover(), this->checkForPress(mouse_position)) : this->unhover();
}

bool Button::isPressed() {
	if (this->is_pressed) {
		this->is_pressed = false;
		return true;
	}
	return false;
}
