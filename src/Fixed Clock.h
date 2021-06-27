#pragma once
#include "stdafx.h"

class FixedClock {
	sf::Clock clock;
	bool clock_flag = true;
public:
	float getElapsedTime() { if (clock_flag) { clock.restart(); clock_flag = false; } return clock.getElapsedTime().asSeconds(); }
	void restart() { clock.restart(); clock_flag = true; }
};
