#pragma once

#include <vector>

#include "SFML/Graphics/Vertex.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Graphics/RenderStates.hpp"

class QuadMap {
public:
	QuadMap()  = default;
	~QuadMap() = default;

	void Create(bool createGrid = true);

	void Render();

	void Reset();

	void Set(sf::Vector2u coords, sf::Color color);

	void SetPosition(sf::Vector2f position);

	sf::Vector2f GetSize() const;

private:
	constexpr static sf::Color s_defaultColor = sf::Color(0, 0, 0, 0);

	std::vector<sf::Vertex> m_Vertices;
	std::vector<sf::Vertex> m_GridVertices;
	sf::Vector2f m_quadSize;
	sf::Vector2u m_size;
	sf::Vector2f m_position;

	static sf::Texture s_defaultTexture;

	void CreateQuad(uint32_t x, uint32_t y, uint32_t index);
	void CreateGrid();
};
