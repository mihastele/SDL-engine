
// include with "" is looking in the local folder, <> (angle brackets) look in the OS include folders
#include "Game.h" // sibling file in this folder
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"

// scope resolution::contructor method
Game::Game() {
	isRunning = false;

	registry = std::make_unique<Registry>();

	Logger::Log("Game Constructor called");
}

Game::~Game() {
	// for professional logging, please use libraries rather than this simple handmade implementation
	Logger::Log("Game Destructor called");
}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error Initilaizing SDL");
		return;
	}

	// C style of doing stuff :P
	SDL_DisplayMode displayMode;
	// populating structs data by passing it by reference, so the function modifies it
	SDL_GetCurrentDisplayMode(0, &displayMode);
	// properties populated, so now we can access them
	// prevent the fake full screen to be fair play on the different resolution sizes
	// windowWidth = 800;// displayMode.w;
	// windowHeight = 600; // displayMode.h;
	windowWidth = displayMode.w;
	windowHeight =  displayMode.h;
	// this is fake fullscreen due to not using the fullscreen mode, but just using the screen size without borders.

	// instead of copying entire thing to memory, it only stores the address of the struct
	window = SDL_CreateWindow("Game made by the 2D Engine of Miha Stele",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		Logger::Err("Error creating SDL window.");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		Logger::Err("Error creating SDL renderer.");
		return;
	}

	// TODO enable comment below for the fullscren when stop debugging
	// SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}


glm::vec2 playerPosition;
glm::vec2 playerVelocity;

void Game::Setup() {
	// playerPosition = glm::vec2(10.0, 20.0);
	// playerVelocity = glm::vec2(10.0, 5.0);

	Entity tank = registry->CreateEntity();
	// Entity truck = registry->CreateEntity();
	 
	
	// registry->AddComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	// registry->AddComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));

	// tank.AddComponent<TransformerComponent>();
	// tank.AddComponent<BoxColliderComponent>();
	// tank.AddComponent<SpriteComponent>("./assets/images/tank.png");
}

void Game::Update() {
	// TODO: if we are too fast, we free processing power for other processes until we reach MILISECONDS_PER_FRAME

	// Don't use the commented version, it wastes CPU cycles since it doesn't share resources while sleeping in the loop
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), milisecondsPreviousFrame + MILISECONDS_PER_FRAME));

	int timeToWait = MILISECONDS_PER_FRAME - (SDL_GetTicks() - milisecondsPreviousFrame);

	if (timeToWait > 0 && timeToWait <= MILISECONDS_PER_FRAME) { // checking for edge cases
		SDL_Delay(timeToWait);
	}

	// delta time is used for distande = delta(velocity) * delta(time) -> delta(time) == deltaTime
	double deltaTime = (SDL_GetTicks() - milisecondsPreviousFrame) / 1000.0;

	// Store the current frame time
	milisecondsPreviousFrame = SDL_GetTicks();


	// velocity
	// playerPosition.x += playerVelocity.x * deltaTime;
	// playerPosition.y += playerVelocity.y * deltaTime;

	//TODO:
}

void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);

	/*
	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	// SDL_Rect player = { 10, 10, 20, 20 };
	// SDL_RenderFillRect(renderer, &player);

	// Core SDL lib only knows how to decode bmp, so we need to use SDL image library
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	SDL_Rect dstRect = {
		static_cast<int>(playerPosition.x),
		static_cast<int>(playerPosition.y), 
		32, 
		32
	}; // Where we want to paste the picture
	SDL_RenderCopy(renderer, texture, NULL, &dstRect); // srcRect is null because we don't want a portion of the image

	SDL_DestroyTexture(texture);
	*/

	//TODO: Render gamre objects


	// ctrl + shift + space to show parameter hints on VS
	SDL_RenderPresent(renderer);
	// SDL is handling all the dirty OS APIs to create windows.

}

void Game::Run() {

	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}

}

void Game::ProcessInput() {
	SDL_Event sdlEvent; // pure struct
	while (SDL_PollEvent(&sdlEvent)) { // passing reference of the full struct
	// if you sent the entire struct, the function copies it and that is inefficient
		switch (sdlEvent.type) {
			case SDL_QUIT: // event that system triggers when you click the close window button
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				break;
		}
	}

}


void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit(); // opposite of SDL init
}