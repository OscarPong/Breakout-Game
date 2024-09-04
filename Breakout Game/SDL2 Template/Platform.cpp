#include "Platform.hpp"

Platform::Platform(Vector2f p_pos, RenderWindow& window) {
	SDL_Texture* platformTexture{ window.loadTexture("../gfx/platform.png") };
	SDL_Rect currentFrame{};
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 64;
	currentFrame.h = 12;
	float widthScale{ 2.0f };
	float heightScale{ 2.0f };

	entity.setPos(p_pos);
	entity.setCurrentFrame(currentFrame);
	entity.setTex(platformTexture);
	entity.setScale(Vector2f(widthScale, heightScale));

	height = currentFrame.h * heightScale;
	width = currentFrame.w * widthScale - height;
}

void Platform::handleEvent(const SDL_Event& event, int& keyPressed) {
	if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			vel.x -= PLATFORM_SPEED;
			keyPressed++;
		}
		if (event.key.keysym.sym == SDLK_RIGHT) {
			vel.x += PLATFORM_SPEED;
			keyPressed++;
		}
	}
	else if (event.type == SDL_KEYUP && event.key.repeat == 0 && keyPressed > 0) {
		if (event.key.keysym.sym == SDLK_LEFT) {
			vel.x += PLATFORM_SPEED;
			keyPressed--;
		}
		if (event.key.keysym.sym == SDLK_RIGHT) {
			vel.x -= PLATFORM_SPEED;
			keyPressed--;
		}
	}
}

void Platform::update() {
	Vector2f newPos{ entity.getPos() + vel };
	if ((newPos.x >= 0) && (newPos.x + entity.getCurrentFrame().w * entity.getScale().x <= WINDOW_WIDTH)) {
		entity.setPos(newPos);
		leftCenter = Vector2f(entity.getPos().x + height / 2, entity.getPos().y + height / 2);
		rightCenter = Vector2f(entity.getPos().x + height / 2 + width, entity.getPos().y + height / 2);
	}
}

Entity& Platform::getEntity() {
	return entity;
}

Vector2f& Platform::getVel() {
	return vel;
}

float Platform::getWidth() {
	return width;
}

float Platform::getHeight() {
	return height;
}

Vector2f& Platform::getLeftCenter() {
	return leftCenter;
}

Vector2f& Platform::getRightCenter() {
	return rightCenter;
}

void Platform::setVel(Vector2f p_vel) {
	vel = p_vel;
}