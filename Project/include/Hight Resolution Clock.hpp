#pragma once

#include <chrono>

class HightResolutionClock {
public:
	HightResolutionClock()  = default;
	~HightResolutionClock() = default;

	void Start();
	void Stop();

	float GetTimeAsMilliseconds();

private:
	std::chrono::time_point<std::chrono::steady_clock> m_start;
	std::chrono::time_point<std::chrono::steady_clock> m_end;

	std::chrono::duration<float> m_duration;
};
