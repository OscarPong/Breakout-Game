#include "Ball.hpp"

Ball::Ball(Vector2f p_pos, RenderWindow& window) {
	SDL_Texture* ballTexture{ window.loadTexture("../gfx/ball.png") };
	SDL_Rect currentFrame{};
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 50;
	currentFrame.h = 50;

	entity.setPos(p_pos);
	entity.setCurrentFrame(currentFrame);
	entity.setTex(ballTexture);
	entity.setScale(Vector2f(0.2f, 0.2f));

	float theta{ static_cast<float>(getRandomInt(45, 90 + 1) * 2 * M_PI / 360) };
	vel.x = BALL_SPEED * cos(theta);
	vel.y = -BALL_SPEED * sin(theta);

	radius = currentFrame.w * this->entity.getScale().x / 2;
	center.x = p_pos.x + radius;
	center.y = p_pos.y + radius;
}

void Ball::update(Platform& playerPlatform, std::vector<Ball>& balls, std::vector<Block>& blocks, int& score) {
	Vector2f newPos{ entity.getPos() + vel };

	if ((newPos.x < 0) || (newPos.x + entity.getCurrentFrame().w * entity.getScale().x > WINDOW_WIDTH)) {
		vel.x *= -1;  
	}
	else if (newPos.y < 0) {
		vel.y *= -1;
	}
	else if (this->checkRectCollision(playerPlatform)) {
		
	}
	else if (this->checkCircleCollision(playerPlatform)) {

	}
	else {
		// Sweep and prune algorithm
		std::vector<Ball*> activeBalls{};
		for (Ball& ball : balls) {
			float leftRange{ ball.getCenter().x - ball.getRadius() };
			float rightRange{ ball.getCenter().x + ball.getRadius() };
			if (center.x - radius >= leftRange && center.x - radius <= rightRange) {
				activeBalls.push_back(&ball);
			}
			else if (center.x + radius >= leftRange && center.x + radius <= rightRange) {
				activeBalls.push_back(&ball);
			}
		}
		for (Ball* ballPtr : activeBalls) {
			if (this->checkBallCollision(*ballPtr)) {
				vel = this->findCollisionVelocity(ballPtr->getCenter(), ballPtr->getRadius());
			}
		}

		std::vector<Block*> activeBlocks{};
		for (Block& block : blocks) {
			float leftRange{ block.getEntity().getPos().x };
			float rightRange{ block.getEntity().getPos().x + block.getWidth() };
			if (center.x - radius >= leftRange && center.x - radius <= rightRange) {
				activeBlocks.push_back(&block);
			}
			else if (center.x + radius >= leftRange && center.x + radius <= rightRange) {
				activeBlocks.push_back(&block);
			}
		}
		for (Block* blockPtr : activeBlocks) {
			if (this->checkRectCollision(*blockPtr)) {
				blocks.erase(std::find(blocks.begin(), blocks.end(), *blockPtr));
				score++;
			}
		}
	}

	newPos = entity.getPos() + vel;
	entity.setPos(newPos);
	center.x = newPos.x + radius;
	center.y = newPos.y + radius;

	if (vel.magnitude() < BALL_SPEED - 0.5) {
		vel = 1.1 * vel;
	}
	if (vel.magnitude() > BALL_SPEED) {
		vel = 0.8 * vel;
	}

}

Vector2f Ball::findCollisionVelocity(Vector2f& center2, float radius2) {
	Vector2f normal{ center2 - center };
	Vector2f tangent{ normal.y, -normal.x };

	Vector2f v1n{ vel.dotProduct(normal) / pow(normal.magnitude(), 2) * normal };
	Vector2f v1t{ vel.dotProduct(tangent) / pow(tangent.magnitude(), 2) * tangent };
	Vector2f vf{ v1t - v1n };

	// Solve 'balls stick together' bug
	vf = vf + (-1/normal.magnitude()) * normal;

	return vf;
}

bool Ball::checkRectCollision(Block& block) {
	Vector2f rectCoord{ block.getEntity().getPos().x, block.getEntity().getPos().y };
	float left{ rectCoord.x };
	float right{ rectCoord.x + block.getWidth() };
	float top{ rectCoord.y };
	float bottom{ rectCoord.y + block.getHeight() };

	if (center.x + radius < left || center.x - radius > right || center.y + radius < top || center.y - radius > bottom) {
		return false;
	}

	float dx1{ radius - (left - center.x) };
	float dx2{ radius - (center.x - right) };
	float dy1{ radius - (top - center.y) };
	float dy2{ radius - (center.y - bottom) };
	float dx{ (dx1 <= dx2) ? dx1 : dx2 };
	float dy{ (dy1 <= dy2) ? dy1 : dy2 };

	if (dx <= dy) {
		vel.x *= -1;
		vel.x += (vel.x >= 0) ? dx : -dx;
	}
	if (dy <= dx) {
		vel.y *= -1;
		vel.y += (vel.y >= 0) ? dy : -dy;
	}

	return true;
}

bool Ball::checkRectCollision(Platform& playerPlatform) {
	Vector2f rectCoord{ playerPlatform.getEntity().getPos().x + playerPlatform.getHeight() / 2, playerPlatform.getEntity().getPos().y };
	float left{ rectCoord.x };
	float right{ rectCoord.x + playerPlatform.getWidth() };
	float top{ rectCoord.y };
	float bottom{ rectCoord.y + playerPlatform.getHeight() };

	if (center.x + radius < left || center.x - radius > right || center.y + radius < top || center.y - radius > bottom) {
		return false;
	}

	float dx1{ radius - (left - center.x) };
	float dx2{ radius - (center.x - right) };
	float dy1{ radius - (top - center.y) };
	float dy2{ radius - (center.y - bottom) };
	float dx{ (dx1 <= dx2) ? dx1 : dx2 };
	float dy{ (dy1 <= dy2) ? dy1 : dy2 };

	if (dx <= dy && dx > 0) {
		if (playerPlatform.getVel().x == 0) {
			vel.x *= -1;
			vel.x += (vel.x >= 0) ? dx : -dx;
		}
		else if (playerPlatform.getVel().x > vel.x) {
			vel.x = PLATFORM_SPEED;
		}
		else if (playerPlatform.getVel().x < vel.x) {
			vel.x = -PLATFORM_SPEED;
		}
	}
	if (dy <= dx && dy > 0) {
		vel.y *= -1;
		vel.y += (vel.y >= 0) ? dy : -dy;
		if (playerPlatform.getVel().x != 0) {
			vel.x += 0.5 * playerPlatform.getVel().x;
		}
	}
	
	return true;
}

bool Ball::checkCircleCollision(Platform& playerPlatform) {

	float distance1{ static_cast<float>(sqrt(pow(playerPlatform.getLeftCenter().x - center.x, 2) + pow(playerPlatform.getLeftCenter().y - center.y, 2))) };
	float distance2{ static_cast<float>(sqrt(pow(playerPlatform.getRightCenter().x - center.x, 2) + pow(playerPlatform.getRightCenter().y - center.y, 2))) };

	if (distance1 <= radius + playerPlatform.getHeight() / 2) {
		if (playerPlatform.getVel().x == 0) {
			vel = this->findCollisionVelocity(playerPlatform.getLeftCenter(), playerPlatform.getHeight() / 2);
		}
		else if (playerPlatform.getVel().x > vel.x) {
			vel.x = PLATFORM_SPEED;
		}
		else if (playerPlatform.getVel().x < vel.x) {
			vel.x = -PLATFORM_SPEED;
		}
		return true;
	}
	if (distance2 <= radius + playerPlatform.getHeight() / 2) {
		if (playerPlatform.getVel().x == 0) {
			vel = this->findCollisionVelocity(playerPlatform.getRightCenter(), playerPlatform.getHeight() / 2);
		}
		else if (playerPlatform.getVel().x > vel.x) {
			vel.x = PLATFORM_SPEED;
		}
		else if (playerPlatform.getVel().x < vel.x) {
			vel.x = -PLATFORM_SPEED;
		}
		return true;
	}
	return false;
}

bool Ball::checkBallCollision(Ball& other) {
	if (*this == other) {
		return false;
	}
	float distance{ static_cast<float>(sqrt(pow(other.center.x - center.x, 2) + pow(other.center.y - center.y, 2))) };
	if (distance <= radius + other.radius) {
		return true;
	}
	return false;
}

Entity& Ball::getEntity() {
	return entity;
}

Vector2f& Ball::getVel() {
	return vel;
}

Vector2f& Ball::getCenter() {
	return center;
}

float Ball::getRadius() {
	return radius;
}

void Ball::setVel(Vector2f p_vel) {
	vel = p_vel;
}

void Ball::setRandomVel() {
	float theta{ static_cast<float>(this->getRandomInt(45, 90 + 1) * 2 * M_PI / 360) };
	vel.x = BALL_SPEED * cos(theta);
	vel.y = -BALL_SPEED * sin(theta);
}

bool operator==(const Ball& lhs, const Ball& rhs) {
	if (lhs.vel != rhs.vel) {
		return false;
	}
	if (lhs.center != rhs.center) {
		return false;
	}
	return true;
}