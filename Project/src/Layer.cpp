#include "GUI/Layer.hpp"

#include "core/Renderer.hpp"

#include "GUI/Widget.hpp"

#include "GUI/Message.hpp"

#include "core/Macros.hpp"
#include "utilities/Functions.hpp"

#include "SFML/Window/Mouse.hpp"

Layer::Layer() {
	m_messageBackground.setSize(sf::Vector2f(1200.0f, 900.0f));
	m_messageBackground.setFillColor(sf::Color(0, 0, 0, 150));
}

void Layer::Update() {
	if (!sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && m_blockInput)
		m_blockInput = false;

	if (!MessageActive()) {
		for (auto& [n, w] : m_Widgets)
			w->Update();

		return;
	}

	Message* message = m_Messages.front();

	message->Update();

	if (!message->Alive()) {
		delete message;
		m_Messages.pop();

		m_blockInput = true;
	}
}

void Layer::Render() {
	for (auto& [n, w] : m_Widgets)
		w->Render();

	if (!MessageActive())
		return;

	Renderer.Draw(m_messageBackground);

	m_Messages.front()->Render();
}

void Layer::AddWidget(const std::string& name, Widget* widget) {
	// Overriding existing widget.
	DB_ASSERT(!Contains(m_Widgets, name));

	m_Widgets.insert
	(
		std::make_pair
		(
			name,
			widget
		)
	);

	//widget->SetParent(this);
}

Widget* Layer::GetWidget(const std::string& name) {
	DB_ASSERT(Contains(m_Widgets, name));

	return m_Widgets[name];
}

void Layer::PushMessage(Message* message) {
	m_Messages.push(message);
}

bool Layer::MessageActive() const {
	return !m_Messages.empty();
}

bool Layer::BlocksInput() const {
	return m_blockInput;
}
