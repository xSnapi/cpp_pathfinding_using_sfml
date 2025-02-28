#include "Hight Resolution Clock.hpp"

void HightResolutionClock::Start() {
	m_start = std::chrono::high_resolution_clock::now();
}

void HightResolutionClock::Stop() {
	m_end = std::chrono::high_resolution_clock::now();
}

float HightResolutionClock::GetTimeAsMilliseconds() {
	m_duration = m_end - m_start;

	return m_duration.count() * 1000.0f;
}
