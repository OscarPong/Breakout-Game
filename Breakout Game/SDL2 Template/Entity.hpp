#pragma once

#include <SDL.h>
#include <SDL_image.h>

#include "Math.hpp"

class Entity {
public:
	Entity() = default;
	Entity(Vector2f p_pos, SDL_Rect p_currentFrame, SDL_Texture* p_tex);

	Vector2f& getPos();
	SDL_Rect& getCurrentFrame();
	SDL_Texture* getTex();
	Vector2f& getScale();

	void setPos(Vector2f& p_pos);
	void setCurrentFrame(SDL_Rect& p_currentFrame);
	void setTex(SDL_Texture* p_tex);
	void setScale(Vector2f p_scale);

	friend bool operator==(const Entity& lhs, const Entity& rhs);

private:
	Vector2f pos{};
	SDL_Rect currentFrame{};
	SDL_Texture* tex{};
	Vector2f scale{};
};