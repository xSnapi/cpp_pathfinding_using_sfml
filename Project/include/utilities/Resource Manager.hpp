#pragma once

#include <cstddef>
#include <unordered_map>

#include "core/Macros.hpp"
#include "utilities/Functions.hpp"

#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Font.hpp"

template<typename T>
class ResourceManager {
private:
	typedef std::unordered_map<std::string, T> Resources;

public:
	ResourceManager() = default;
	virtual ~ResourceManager() = default;

	Resources& Raw() {
		return m_Resources;
	}

	T& Get(const std::string& name) {
		#ifdef DEBUG
		if (!Contains(m_Resources, name))
			LOG("Accessing not existing resource on name \"", name, "\"\n");
		#endif

		return m_Resources[name];
	}

	bool Load(std::string name, const std::string& path) {
		#ifdef DEBUG
		if (Contains(m_Resources, name)) {
			LOG("Tried to override resource on name \"", name, "\"\n");
			return false;
		}
		#endif

		return m_Resources[name].loadFromFile(path);
	}

	void Unlaod(const std::string& name) {
		DB_ASSERT(Contains(m_Resources, name)); // Tried deleting resource that dosen't exist

		m_Resources.erase(name);
	}

	void Clear() {
		m_Resources.clear();
	}

	size_t Size() const {
		return m_Resources.size();
	}

	T& operator[](const std::string& name) {
		return Get(name);
	}

protected:
	Resources m_Resources;
};

namespace priv {
	class ShaderManager : public ResourceManager<sf::Shader> {
	public:
		bool Load(std::string name, const std::string& path) = delete;

		bool LoadFromFile(std::string name, const std::string& vertPath, const std::string& fragPath) {
			return m_Resources[name].loadFromFile(vertPath, fragPath);
		}

		bool LoadFromMemory(std::string name, const std::string& vert, const std::string& frag) {
			return m_Resources[name].loadFromMemory(vert, frag);
		}
	};

}

inline ResourceManager<sf::Font> FontManager;

inline priv::ShaderManager ShaderManager;
