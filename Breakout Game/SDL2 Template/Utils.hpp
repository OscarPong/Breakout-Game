#pragma once

#include <SDL.h>

namespace utils {
	inline float hireTimeInSeconds() {
		float t{ static_cast<float>(SDL_GetTicks()) };
		t *= 0.001f;
		return t;
	}
}