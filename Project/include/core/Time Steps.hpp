#pragma once

extern const long double& et;
extern const float& dt;

// Jedyne zadanie tej klasy to sprawiene ¿e deltaTime nie jest akcesowany jako Runtime.GetDeltaTime() tylko jest dostêpny publicznie jako dt
struct TimeSteps {
	static const long double& ElapsedTime;

	static const float& DeltaTime;
};
