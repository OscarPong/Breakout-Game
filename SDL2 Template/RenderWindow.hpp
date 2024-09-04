#pragma once

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Math.hpp"
#include "Entity.hpp"

const int WINDOW_WIDTH{ 960 };
const int WINDOW_HEIGHT{ 720 };

class RenderWindow {
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	~RenderWindow();
	
	SDL_Texture* loadTexture(const char* p_filePath);
	void clear();
	void render(Entity& p_entity);
	void render(SDL_Texture* p_tex, int p_w, int p_h, int multipler);
	void render(SDL_Surface* surfaceMessage, SDL_Rect messageRect);
	void display();

	int getRefreshRate();

private:
	SDL_Window* window{};
	SDL_Renderer* renderer{};
};