#include "Block.hpp"

Block::Block(Vector2f p_pos, RenderWindow& window) {

	std::vector<SDL_Texture*> textures{
		window.loadTexture("../gfx/blockColour1.png"),
		window.loadTexture("../gfx/blockColour2.png"),
		window.loadTexture("../gfx/blockColour3.png"),
		window.loadTexture("../gfx/blockColour4.png"),
		window.loadTexture("../gfx/blockColour5.png"),
		window.loadTexture("../gfx/blockColour6.png")
	};
	int randomIndex{ getRandomInt(0, textures.size()) };

	SDL_Rect currentFrame{};
	currentFrame.x = 0;
	currentFrame.y = 0;
	currentFrame.w = 64;
	currentFrame.h = 64;
	float widthScale{ 1.0f };
	float heightScale{ 0.5f };

	entity.setPos(p_pos);
	entity.setCurrentFrame(currentFrame);
	entity.setTex(textures.at(randomIndex));
	entity.setScale(Vector2f(widthScale, heightScale));

	height = currentFrame.h * heightScale;
	width = currentFrame.w * widthScale;
}

Entity& Block::getEntity() {
	return entity;
}

float Block::getWidth() {
	return width;
}

float Block::getHeight() {
	return height;
}

bool operator==(const Block& lhs, const Block& rhs) {
	if (lhs.entity == rhs.entity && lhs.width == rhs.width && lhs.height == rhs.height) {
		return true;
	}
	return false;
}