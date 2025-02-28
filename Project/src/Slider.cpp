#include "GUI/Slider.hpp"	

#include <algorithm> // For std::clamp

#include "core/Runtime.hpp"
#include "core/Renderer.hpp"

#include "SFML/Window/Mouse.hpp"

Slider::Slider(sf::Vector2f position)
	: Button("", position, sf::Vector2f(25.0f, 50.0f))
	, m_position(position)
{
	OnPress = [&] {
		m_snapToMouse = true;
		};

	m_body.setOutlineThickness(0.0f);
	m_body.setFillColor(sf::Color::White);

	// Slider Body init
	{
		sf::Vector2f size = sf::Vector2f(s_maxOffset * 2.0f, 15.0f);

		m_sliderBody.setSize(size);
		m_sliderBody.setOrigin(size * 0.5f);
		m_sliderBody.setPosition(m_position);
		
		m_sliderBody.setFillColor(sf::Color::Transparent);
		m_sliderBody.setOutlineThickness(5.0f);
	}
}

float Slider::GetProgress() const {
	sf::Vector2f diff = m_body.getPosition() - m_position;

	//because button is centered we add offset here

	return (diff.x + s_maxOffset) / (s_maxOffset * 2.0f);
}

void Slider::Update() {
	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(Runtime::Instance->GetWindow());

	float mouseMin = m_position.x - s_maxOffset;
	float mouseMax = m_position.x + s_maxOffset;

	float posX = std::clamp(mousePos.x, mouseMin, mouseMax);

	if(m_snapToMouse)
		m_body.setPosition(sf::Vector2f(posX, m_body.getPosition().y));

	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		m_snapToMouse = false;

	Button::Update();
}

void Slider::Render() {
	Renderer.Draw(m_sliderBody);
	Renderer.Draw(m_body);
}
