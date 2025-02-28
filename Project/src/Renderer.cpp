#pragma once
#include "core/Renderer.hpp"

#include "core/Macros.hpp"

using namespace priv;

void Renderer::Init(sf::RenderWindow& window) {
	// Renderer was initialized before
	DB_ASSERT(!m_window);

	m_window = &window;
}

void Renderer::Clear() {
	m_window->clear(sf::Color(31, 42, 48));
}

void Renderer::Flush() {
	m_window->display();
}

void Renderer::Draw(const sf::Drawable& drawable, const sf::RenderStates& states) {
	m_window->draw(drawable, states);
}

void Renderer::Draw(const sf::Vertex* vertices, std::size_t vertexCount, sf::PrimitiveType type, const sf::RenderStates& states) {
	m_window->draw(vertices, vertexCount, type, states);
}

void Renderer::Draw(const sf::VertexBuffer& vertexBuffer, const sf::RenderStates& states) {
	m_window->draw(vertexBuffer, states);
}

void Renderer::Draw(const sf::VertexBuffer& vertexBuffer, std::size_t firstVertex, std::size_t vertexCount, const sf::RenderStates& states) {
	m_window->draw(vertexBuffer, firstVertex, vertexCount, states);
}
