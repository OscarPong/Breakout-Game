#pragma once

#include "RenderWindow.hpp"
#include "Entity.hpp"

const float PLATFORM_SPEED{ 5.0f };

class Platform {
public:
	Platform(Vector2f p_pos, RenderWindow& window);

	void handleEvent(const SDL_Event& event, int& keyPressed);
	void update();

	Entity& getEntity();
	Vector2f& getVel();
	float getWidth();
	float getHeight();
	Vector2f& getLeftCenter();
	Vector2f& getRightCenter();

	void setVel(Vector2f p_vel);

private:
	Entity entity{};
	Vector2f vel{};
	float width{};
	float height{};
	Vector2f leftCenter{};
	Vector2f rightCenter{};
};