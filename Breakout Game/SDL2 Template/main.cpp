#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <string>

#include "Math.hpp"
#include "Entity.hpp"
#include "Platform.hpp"
#include "Ball.hpp"
#include "Block.hpp"
#include "RenderWindow.hpp"
#include "Utils.hpp"

void loadStartScreen(RenderWindow& window, SDL_Texture* screenTexture) {
	window.render(screenTexture, WINDOW_WIDTH, WINDOW_HEIGHT, 1);
}

void loadEndScreen(RenderWindow& window, SDL_Texture* screenTexture, TTF_Font* headerFont, TTF_Font* messageFont, SDL_Color white, int& highestScore) {
	window.render(screenTexture, 952, 460, 2);

	SDL_Surface* heading = TTF_RenderText_Solid(headerFont, "No more balls left!", white);
	SDL_Rect headingRect{};
	headingRect.x = 205;
	headingRect.y = 190;
	headingRect.w = 550;
	headingRect.h = 80;
	window.render(heading, headingRect);

	std::string text{ "Highest Score: " + std::to_string(highestScore) };
	SDL_Surface* message1 = TTF_RenderText_Solid(messageFont, text.c_str(), white);
	SDL_Rect messageRect1{};
	messageRect1.x = 280;
	messageRect1.y = 318;
	messageRect1.w = 400;
	messageRect1.h = 64;
	window.render(message1, messageRect1);

	SDL_Surface* message2 = TTF_RenderText_Solid(messageFont, "Press ENTER to attempt again...", white);
	SDL_Rect messageRect2{};
	messageRect2.x = 155;
	messageRect2.y = 418;
	messageRect2.w = 650;
	messageRect2.h = 64;
	window.render(message2, messageRect2);
}

void loadScore(RenderWindow& window, TTF_Font* messageFont, SDL_Color white, const int& score) {

	std::string text{ "Score: " + std::to_string(score) };
	SDL_Surface* message = TTF_RenderText_Solid(messageFont, text.c_str(), white);
	SDL_Rect messageRect{};
	messageRect.x = 10;
	messageRect.y = 10;
	messageRect.w = 160;
	messageRect.h = 50;
	window.render(message, messageRect);
}
void loadCountdown(RenderWindow& window, TTF_Font* messageFont, SDL_Color white, const int& seconds) {

	std::string text{ std::to_string(seconds) };
	SDL_Surface* message = TTF_RenderText_Solid(messageFont, text.c_str(), white);
	SDL_Rect messageRect{};
	messageRect.x = 448;
	messageRect.y = 296;
	messageRect.w = 64;
	messageRect.h = 128;
	window.render(message, messageRect);
}


void setInitialState(RenderWindow& window, Platform& playerPlatform, std::vector<Ball>& balls, std::vector<Block>& blocks, int& score, int& keyPressed) {
	score = 0;
	keyPressed = 0;

	Vector2f originalPos{ 416, 640 };
	playerPlatform.getEntity().setPos(originalPos);
	playerPlatform.setVel(Vector2f(0.0f, 0.0f));

	Ball ball{ Vector2f(475, 500), window };
	ball.setVel(Vector2f(0.0f, 0.0f));
	balls.push_back(ball);

	//for (int i{ 0 }; i < 960; i += 30) {
	//	balls.push_back(Ball(Vector2f(i, 400), window));
	//}

	blocks.clear();
	for (int i{ 0 }; i < 960; i += 64) {
		blocks.push_back(Block(Vector2f(i, 64), window));
	}
}

void addRandomBlock(RenderWindow& window,  std::vector<Block>& blocks) {
	Vector2f randomPos{};
	bool samePos{ true };
	while (samePos) {
		samePos = false;
		int randomX = (std::rand() % 15);
		int randomY = (2 + std::rand() % 9);
		randomPos = Vector2f(randomX * 64.0f, randomY * 32.0f);
		for (Block& block : blocks) {
			if (block.getEntity().getPos() == randomPos) {
				samePos = true;
			}
		}
	}
	
	blocks.push_back(Block(randomPos, window));
}

int main(int argc, char* argv[]) {

	// For generating random integers
	std::srand((unsigned)time(NULL));

	// Initialise SDL2. Add these for all SDL2 lib to work
	if (SDL_Init(SDL_INIT_VIDEO) > 0) {
		std::cout << "SDL_Init has failed. Error: " << SDL_GetError() << '\n';
	}
	if (!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "IMG_Init has failed. Error: " << SDL_GetError() << '\n';
	}
	if (TTF_Init() == -1) {
		std::cout << "TTF_Init has failed. Error: " << SDL_GetError() << '\n';
	}

	//Set callback timers
	std::vector<SDL_TimerID> timers{};

	int highestScore{ 0 };
	int score{ 0 };
	int keyPressed{ 0 };

	RenderWindow window("BREAKOUT GAME v1.0", WINDOW_WIDTH, WINDOW_HEIGHT);

	// Create graphics
	SDL_Texture* titleScreenTexture{ window.loadTexture("../gfx/titleScreen.png") };
	SDL_Texture* transitionScreenTexture{ window.loadTexture("../gfx/transition_screen_tex.png") };
	TTF_Font* aOvelSansRounded{ TTF_OpenFont("../font/AovelSansRounded-rdDL.ttf", 48) };
	TTF_Font* pixelLettersFull{ TTF_OpenFont("../font/Pixellettersfull-BnJ5.ttf", 48) };
	SDL_Color white{ 255, 255, 255 };

	// Create entities
	Platform playerPlatform{ Vector2f(416, 640), window };
	std::vector<Ball> balls{};
	std::vector<Block> blocks{};
	setInitialState(window, playerPlatform, balls, blocks, score, keyPressed);

	bool gameRunning{ true };
	SDL_Event event{};

	const float timeStep{ 0.01f };
	float accumulator{ 0.0f };
	float currentTime{ utils::hireTimeInSeconds() };

	bool startGame{ false };
	bool attemptAgain{ false };

	bool isTimerOn{ false };
	float startTimer{};
	float endTimer{};

	while (gameRunning) {

		int startTicks{ static_cast<int>(SDL_GetTicks()) };

		float newTime{ utils::hireTimeInSeconds() };
		float frameTime{ newTime - currentTime };

		currentTime = newTime;
		accumulator += frameTime;

		while (accumulator >= timeStep) {
			// Handle all events
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_QUIT) {
					gameRunning = false;
				}
				if (event.type == SDL_KEYDOWN && event.key.repeat == 0 && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)) {
					startTimer = utils::hireTimeInSeconds();
					isTimerOn = true;
					startGame = true;
				}
				if (attemptAgain && event.type == SDL_KEYDOWN && event.key.repeat == 0 && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_KP_ENTER)) {
					setInitialState(window, playerPlatform, balls, blocks, score, keyPressed);
					startTimer = utils::hireTimeInSeconds();
					isTimerOn = true;
					attemptAgain = false;
				}
				if (startGame && !balls.empty()) {
					playerPlatform.handleEvent(event, keyPressed);
				}
			}
			if (startGame && !balls.empty()) {
				int currentScore{ score };

				endTimer = utils::hireTimeInSeconds();
				if (isTimerOn && endTimer - startTimer >= 3) {
					isTimerOn = false;
					balls.begin()->setRandomVel();
				}

				playerPlatform.update();
				for (auto iter{ balls.begin() }; iter < balls.end(); iter++) {
					iter->update(playerPlatform, balls, blocks, score);
					if (iter->getCenter().y > WINDOW_HEIGHT) {
						balls.erase(iter);
						break;
					}
				}
				if (score % 10 == 0 && score != 0 && currentScore != score) {
					balls.push_back(Ball(Vector2f(475, 500), window));
				}
				/*if (currentScore != score) {
					balls.push_back(Ball(Vector2f(475, 500), window));
				}*/
				if (currentScore != score) {
					addRandomBlock(window, blocks);
				}
			}
			accumulator -= timeStep;
		}

		window.clear();
		
		// Render Entities
		window.render(playerPlatform.getEntity());
		
		for (Block& block : blocks) {
			window.render(block.getEntity());
		}
		for (Ball& ball : balls) {
			window.render(ball.getEntity());
		}

		loadScore(window, pixelLettersFull, white, score);
		if (!startGame) {
			loadStartScreen(window, titleScreenTexture);
		}
		if (balls.empty()) {
			if (score > highestScore) {
				highestScore = score;
			}
			loadEndScreen(window, transitionScreenTexture, aOvelSansRounded, pixelLettersFull, white, highestScore);
			attemptAgain = true;
		}
		if (isTimerOn && endTimer - startTimer >= 0 && endTimer - startTimer < 1) {
			loadCountdown(window, pixelLettersFull, white, 3);
		}
		if (isTimerOn && endTimer - startTimer >= 1 && endTimer - startTimer < 2) {
			loadCountdown(window, pixelLettersFull, white, 2);
		}
		if (isTimerOn && endTimer - startTimer >= 2 && endTimer - startTimer < 3) {
			loadCountdown(window, pixelLettersFull, white, 1);
		}

		window.display();

		// Optimize CPU for not running in every frame
		int frameTicks{ static_cast<int>(SDL_GetTicks() - startTicks) };
		if (frameTicks < 1000 / window.getRefreshRate()) {
			SDL_Delay(1000 / window.getRefreshRate() - frameTicks);
		}

	}

	//Remove timer in case the call back was not called
	for (SDL_TimerID& timerID : timers) {
		SDL_RemoveTimer(timerID);
	}

	SDL_Quit();

	return 0;
}