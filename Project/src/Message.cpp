#include "GUI/Message.hpp"

#include "core/Renderer.hpp"

#include "utilities/Resource Manager.hpp"

#include "Global Variables.hpp"

Message::Message(const std::string& msg)
	: m_text(FontManager["default"])
{
	CreateText(msg);
	CreateBody();
	CreateButton();
}

Message::~Message() {
	delete m_closeButton;
}

void Message::Update() {
	m_closeButton->Update();
}

void Message::Render() {
	Renderer.Draw(m_body);
	Renderer.Draw(m_text);

	m_closeButton->Render();
}

bool Message::Alive() const {
	return m_alive;
}

void Message::CreateText(const std::string& msg) {
	m_text.setString(msg);

	sf::Vector2f size = m_text.getGlobalBounds().getSize();

	m_text.setOrigin(size * 0.5f);

	m_text.setPosition(sf::Vector2f(1200.0f, 900.0f) * 0.5f - sf::Vector2f(0.0f, 100.0f));
}

void Message::CreateBody() {
	constexpr sf::Vector2f SIZE   = sf::Vector2f(600.0f, 300.0f);
	constexpr sf::Vector2f BOUNDS = sf::Vector2f(20.0f, 20.0f);

	sf::Vector2f textSize = m_text.getGlobalBounds().getSize();

	sf::Vector2f size = sf::Vector2f(std::max(textSize.x, SIZE.x), std::max(textSize.y, SIZE.y)) + BOUNDS;

	m_body.setSize(size);
	m_body.setOrigin(size * 0.5f);
	m_body.setPosition(m_text.getPosition());

	m_body.setFillColor(sf::Color(86, 48, 156));
	m_body.setOutlineThickness(5.0f);
}

void Message::CreateButton() {
	sf::Vector2f bodySize = m_body.getSize() * 0.5f;
	bodySize.y *= -1.0f; // Invers body size so we get right Y position

	sf::Vector2f bodyPos  = m_body.getPosition();

	m_closeButton = new Button("X", bodyPos + bodySize - sf::Vector2f(40.0f, -40.0f), sf::Vector2f(50.0f, 50.0f));

	m_closeButton->OnPress = [this] { m_alive = false; };
}
