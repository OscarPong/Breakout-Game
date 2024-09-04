#pragma once

#include "Math.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"

#include <vector>

class Block {
public:
	Block(Vector2f p_pos, RenderWindow& window);

	Entity& getEntity();
	float getWidth();
	float getHeight();

	friend bool operator==(const Block& lhs, const Block& rhs);

private:
	Entity entity{};
	float width{};
	float height{};

	int getRandomInt(int offset, int range) {
		int randomIndex = (offset + std::rand() % range);
		return randomIndex;
	}
};