#pragma once

#include <unordered_map>
#include <string>
#include <queue>

#include "SFML/Graphics/RectangleShape.hpp"

class Message;
class Widget;

class Layer {
public:
	Layer();
	~Layer() = default;

	void Update();
	void Render();

	void AddWidget(const std::string& name, Widget* widget);
	Widget* GetWidget(const std::string& name);

	void PushMessage(Message* message);

	bool MessageActive() const;
	bool BlocksInput() const;

private:
	std::unordered_map<std::string, Widget*> m_Widgets;
	std::queue<Message*> m_Messages;

	sf::RectangleShape m_messageBackground;

	bool m_blockInput = false;
};
