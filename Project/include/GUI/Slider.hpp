#pragma once

#include "Widget.hpp"
#include "Button.hpp"

// Slider class dziedziczy z klasy button i widget
class Slider : public Button {
public:
	Slider(sf::Vector2f position);
	~Slider() = default;

	// Returns value 0 - 1
	float GetProgress() const;

private:
	constexpr static float s_maxOffset = 150.0f;

	sf::Vector2f m_position;

	bool m_snapToMouse = false;

	sf::RectangleShape m_sliderBody;

	virtual void Update() override;
	virtual void Render() override;
};
