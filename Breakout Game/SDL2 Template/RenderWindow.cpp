#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) : window{ NULL }, renderer{ NULL } {

	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Window failed to init. Error: " << SDL_GetError() << '\n';
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED || SDL_RENDERER_PRESENTVSYNC);

}

RenderWindow::~RenderWindow() {
	SDL_DestroyWindow(window);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {

	SDL_Texture* texture{ IMG_LoadTexture(renderer, p_filePath) };
	if (texture == NULL) {
		std::cout << "Failed to load texture. Error: " << SDL_GetError() << '\n';
	}

	return texture;
}

void RenderWindow::clear() {
	SDL_RenderClear(renderer);
}

void RenderWindow::render(Entity& p_entity) {
	// source
	SDL_Rect src{};
	src.x = p_entity.getCurrentFrame().x;
	src.y = p_entity.getCurrentFrame().y;
	src.w = p_entity.getCurrentFrame().w;
	src.h = p_entity.getCurrentFrame().h;
	// destination
	SDL_Rect dst{};
	dst.x = p_entity.getPos().x;
	dst.y = p_entity.getPos().y;
	dst.w = p_entity.getCurrentFrame().w * p_entity.getScale().x;
	dst.h = p_entity.getCurrentFrame().h * p_entity.getScale().y;

	SDL_RenderCopy(renderer, p_entity.getTex(), &src, &dst);
}

void RenderWindow::render(SDL_Texture* p_tex, int p_w, int p_h, int multipler) {
	SDL_Rect src{};
	src.x = 0;
	src.y = 0;
	src.w = p_w;
	src.h = p_h;

	SDL_Rect dst{};
	dst.x = src.x * multipler;
	dst.y = src.y * multipler;
	dst.w = src.w * multipler;
	dst.h = src.h * multipler;

	SDL_RenderCopy(renderer, p_tex, &src, &dst);
}

void RenderWindow::render(SDL_Surface* surfaceMessage, SDL_Rect messageRect) {
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);
	SDL_RenderCopy(renderer, message, NULL, &messageRect);

	SDL_FreeSurface(surfaceMessage);
	SDL_DestroyTexture(message);
}

void RenderWindow::display() {
	SDL_RenderPresent(renderer);
}

int RenderWindow::getRefreshRate() {
	int displayIndex{ SDL_GetWindowDisplayIndex(window) };

	SDL_DisplayMode mode{};

	SDL_GetDisplayMode(displayIndex, 0, &mode);

	return mode.refresh_rate;
}