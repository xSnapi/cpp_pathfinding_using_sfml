#pragma once
/*
	~ Precompiled Header - reduces project compilation time ~

- Libraries that are used more than once should be included here
- Last update - 12/04/2021
*/
#include <iostream>
#include <map>
#include <vector>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 900;

#ifdef NDEBUG
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#define LOG(x)
#else
#define LOG(x) std::cout << x << std::endl
#endif // NDEBUG
/* ↑↑↑↑ Dumps the console window ↑↑↑↑ */