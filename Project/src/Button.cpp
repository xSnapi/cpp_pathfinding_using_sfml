#include "GUI/Button.hpp"

#include "core/Renderer.hpp"

#include "utilities/Resource Manager.hpp"

#include "core/Runtime.hpp"

#include "SFML/Window/Mouse.hpp"
#include "SFML/Graphics/Image.hpp"

Button::Button(const std::string& label, sf::Vector2f position, sf::Vector2f size) 
	: m_label(FontManager["default"])
{
	CreateBody(position, size);
	CreateLabel(label);

	static sf::Texture defaultTexture;
	static bool textureCreated = false;

	if (!textureCreated) {
		sf::Image im;
		im.create(sf::Vector2u(1, 1), sf::Color::White);

		defaultTexture.loadFromImage(im);

		textureCreated = true;
	}

	m_body.setTexture(&defaultTexture);
}

void Button::SetActive(bool active) {
	m_active = active;

	m_body.setOutlineColor(sf::Color(255, 255, 255, 255));
	m_label.setFillColor(sf::Color(255, 255, 255, 255));

	if (m_active)
		return;

	m_body.setOutlineColor(sf::Color(255, 255, 255, 100));
	m_label.setFillColor(sf::Color(255, 255, 255, 100));

	m_hovered = false;
	m_pressed = false;
}

void Button::CreateBody(sf::Vector2f pos, sf::Vector2f size) {
	m_body.setSize(size);
	m_body.setOrigin(size * 0.5f);

	m_body.setPosition(pos);
	m_body.setFillColor(sf::Color::Transparent);
	m_body.setOutlineThickness(5.0f);
}

void Button::CreateLabel(const std::string& label) {
	m_label.setString(label);
	
	sf::Vector2f size = m_label.getGlobalBounds().getSize();

	m_label.setOrigin(size * 0.5f);

	m_label.setPosition(m_body.getPosition());
}

void Button::Update() {
	if (!m_active)
		return;

	sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(Runtime::Instance->GetWindow());

	bool good = m_body.getGlobalBounds().contains(mousePos);

	if (good && !m_hovered) {
		if (OnHover)
			OnHover();

		m_hovered = true;
	}

	else if (!good && m_hovered) {
		if (OnExit)
			OnExit();

		m_hovered = false;
	}

	if (m_pressed && (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) || !good)) {
		if (OnRelease)
			OnRelease();

		m_pressed = false;
	}

	if (!good)
		return;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && !m_pressed) {
		if (OnPress)
			OnPress();

		m_pressed = true;
	}
}

void Button::Render() {
	if (m_hovered) {
		if (Runtime::Instance->ShaderAvailable()) {
			// We simply draw button with gradient shader and return
			auto& shader = ShaderManager["gradient"];

			Renderer.Draw(m_body, &shader);
			Renderer.Draw(m_label, &shader);
			return;
		}

		// Shader is not available
		if (m_active) {
			m_body.setOutlineColor(sf::Color(118, 78, 128));
			m_label.setFillColor(sf::Color(118, 78, 128));
		}
	}
	else if (!m_hovered && !Runtime::Instance->ShaderAvailable()) {
		// We need to check if button is active before doing that
		if (m_active) {
			m_body.setOutlineColor(sf::Color::White);
			m_label.setFillColor(sf::Color::White);
		}
	}

	Renderer.Draw(m_body);
	Renderer.Draw(m_label);
}
