#pragma once

#include <string>
#include <functional>

#include "Widget.hpp"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Text.hpp"

class Button : public Widget {
public:
	Button(const std::string& label, sf::Vector2f position, sf::Vector2f size);
	~Button() = default;

	virtual void Update() override;
	virtual void Render() override;

	void SetActive(bool active);

	std::function<void()> OnPress   = 0;
	std::function<void()> OnRelease = 0;
	std::function<void()> OnHover   = 0;
	std::function<void()> OnExit    = 0;

protected:
	sf::RectangleShape m_body;
	sf::Text m_label;

private:
	bool m_active = true;

	bool m_pressed = false;
	bool m_hovered = false;

	void CreateBody(sf::Vector2f pos, sf::Vector2f size);
	void CreateLabel(const std::string& label);
};
