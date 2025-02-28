#include "Quad Map.hpp"

#include "core/Renderer.hpp"

#include "utilities/Functions.hpp"

#include "utilities/Resource Manager.hpp"

#include "SFML/Graphics/Image.hpp"

sf::Texture QuadMap::s_defaultTexture;

void QuadMap::Create(bool createGrid) {
	m_size	   = global::NODE_COUNT;
	m_quadSize = global::NODE_SIZE;

	m_Vertices.resize(m_size.x * m_size.y * 6);

	uint32_t index = 0;

	for (uint32_t y = 0; y < m_size.y; y++) {
		for (uint32_t x = 0; x < m_size.x; x++) {
			CreateQuad(x, y, index);

			index++;
		}
	}

	if (createGrid)
		CreateGrid();

	if (!s_defaultTexture.getSize().x) {
		sf::Image im;
		im.create(sf::Vector2u(1, 1), sf::Color::White);

		s_defaultTexture.loadFromImage(im);
	}
}

void QuadMap::Render() {
	sf::RenderStates states;
	//states.shader  = &ShaderManager["gradient"];
	states.texture = &s_defaultTexture;
	states.transform.translate(m_position);

	Renderer.Draw(m_Vertices.data(), m_Vertices.size(), sf::PrimitiveType::Triangles, states);
	Renderer.Draw(m_GridVertices.data(), m_GridVertices.size(), sf::PrimitiveType::Lines, states);
}

void QuadMap::Reset() {
	for (auto& v : m_Vertices)
		v.color = s_defaultColor;
}

void QuadMap::Set(sf::Vector2u coords, sf::Color color) {
	uint32_t index = In2D(coords.x, coords.y, m_size.x) * 6;

	for (uint32_t i = 0; i < 6; i++)
		m_Vertices[index + i].color = color;
}

void QuadMap::SetPosition(sf::Vector2f position) {
	m_position = position;
}

sf::Vector2f QuadMap::GetSize() const {
	return sf::Vector2f(m_size).cwiseMul(m_quadSize);
}

void QuadMap::CreateQuad(uint32_t x, uint32_t y, uint32_t index) {
	sf::Vector2f size = m_quadSize;

	sf::Vector2f translation = size.cwiseMul(sf::Vector2f(x, y));

	// The quads are made of two triangles
	/*
	   5,0------1
		|\		|
		| \		|
		|  \	|
		|   \	|
		|	 \	|
		|	  \ |
		|	   \|
		4------3,2

		Vertices 0, 1, 2 represent one triangle and
		Vertices 3, 4, 5 represent second triangle
	*/

	uint32_t offset = index * 6;
	m_Vertices[offset + 0].position = sf::Vector2f(0.0f,   0.0f	 ) + translation;
	m_Vertices[offset + 1].position = sf::Vector2f(size.x, 0.0f	 ) + translation;
	m_Vertices[offset + 2].position = sf::Vector2f(size.x, size.y) + translation;
	m_Vertices[offset + 3].position = sf::Vector2f(size.x, size.y) + translation;
	m_Vertices[offset + 4].position = sf::Vector2f(0.0f,   size.y) + translation;
	m_Vertices[offset + 5].position = sf::Vector2f(0.0f,   0.0f	 ) + translation;

	for (uint32_t i = 0; i < 6; i++)
		m_Vertices[offset + i].color = s_defaultColor;
}

void QuadMap::CreateGrid() {
	sf::Vector2f size = GetSize();

	m_GridVertices.reserve(m_size.x * m_size.y * 4);

	// Horizontal Lines
	for (uint32_t i = 0; i < size.y / m_quadSize.y; i++) {
		auto& v0 = m_GridVertices.emplace_back();
		auto& v1 = m_GridVertices.emplace_back();

		v0.color = sf::Color(255, 255, 255, 150);
		v1.color = sf::Color(255, 255, 255, 150);

		v0.position = sf::Vector2f(0.0f, i * m_quadSize.y);
		v1.position = sf::Vector2f(size.x, i * m_quadSize.y);
	}

	// Vertical Lines
	for (uint32_t i = 0; i < size.x / m_quadSize.x; i++) {
		auto& v0 = m_GridVertices.emplace_back();
		auto& v1 = m_GridVertices.emplace_back();

		v0.color = sf::Color(255, 255, 255, 150);
		v1.color = sf::Color(255, 255, 255, 150);

		v0.position = sf::Vector2f(i * m_quadSize.x, 0.0f);
		v1.position = sf::Vector2f(i * m_quadSize.x, size.y);
	}
}
