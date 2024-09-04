#pragma once

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "Platform.hpp"
#include "Block.hpp"

#include <iostream>
#include <vector>
#include <cmath>
#include <iterator>
#include <algorithm>

const float BALL_SPEED{ 3.0f };

class Ball {
public:
	Ball(Vector2f p_pos, RenderWindow& window);

	void update(Platform& playerPlatform, std::vector<Ball>& balls, std::vector<Block>& blocks, int& score);
	Vector2f findCollisionVelocity(Vector2f& center2, float radius2);
	bool checkRectCollision(Block& block);
	bool checkRectCollision(Platform& playerPlatform);
	bool checkCircleCollision(Platform& playerPlatform);
	bool checkBallCollision(Ball& ball);;
	
	Entity& getEntity();
	Vector2f& getVel();
	Vector2f& getCenter();
	float getRadius();

	void setVel(Vector2f p_vel);
	void setRandomVel();

	friend bool operator==(const Ball& lhs, const Ball& rhs);

private:
	Entity entity{};
	Vector2f vel{};
	Vector2f center{};
	float radius{};

	int getRandomInt(int offset, int range) {
		int randomIndex = (offset + std::rand() % range);
		return randomIndex;
	}
};