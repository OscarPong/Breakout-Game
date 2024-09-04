#include "Entity.hpp"

Entity::Entity(Vector2f p_pos, SDL_Rect p_currentFrame, SDL_Texture* p_tex) : pos{ p_pos }, currentFrame{ p_currentFrame }, tex{ p_tex } {

}

Vector2f& Entity::getPos() {
	return pos;
}

SDL_Rect& Entity::getCurrentFrame() {
	return currentFrame;
}

SDL_Texture* Entity::getTex() {
	return tex;
}

Vector2f& Entity::getScale() {
	return scale;
}

void Entity::setPos(Vector2f& p_pos) {
	pos = p_pos;
}

void Entity::setCurrentFrame(SDL_Rect& p_currentFrame) {
	currentFrame = p_currentFrame;
}

void Entity::setTex(SDL_Texture* p_tex) {
	tex = p_tex;
}

void Entity::setScale(Vector2f p_scale) {
	scale = p_scale;
}

bool operator==(const Entity& lhs, const Entity& rhs) {
	if (lhs.pos != rhs.pos) {
		return false;
	}
	if (lhs.scale != rhs.scale) {
		return false;
	}
	return true;
}