#pragma once

#include "SFML/Graphics/RenderWindow.hpp"

namespace priv {
	class Renderer {
	public:
		Renderer()  = default;
		~Renderer() = default;

		void Init(sf::RenderWindow& window);

		void Clear();
		void Flush();

		void Draw(const sf::Drawable& drawable, const sf::RenderStates& states = sf::RenderStates::Default);
		void Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states = sf::RenderStates::Default);
		void Draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states = sf::RenderStates::Default);
		void Draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states = sf::RenderStates::Default);

	private:
		sf::RenderWindow* m_window = nullptr;
	};
}

inline priv::Renderer Renderer;
