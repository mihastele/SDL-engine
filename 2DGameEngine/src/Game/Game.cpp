
// include with "" is looking in the local folder, <> (angle brackets) look in the OS include folders
#include <fstream>
#include <glm/glm.hpp>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "Game.h" // sibling file in this folder

// scope resolution::contructor method
Game::Game() {
	isRunning = false;

	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();

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


void Game::LoadLevel(int level) {
	// playerPosition = glm::vec2(10.0, 20.0);
	// playerVelocity = glm::vec2(10.0, 5.0);

	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();


	assetStore->AddTexture(renderer, "tank-image", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "truck-image", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "chopper-image", "./assets/images/chopper.png");
	assetStore->AddTexture(renderer, "radar-image", "./assets/images/radar.png");
	assetStore->AddTexture(renderer, "tilemap-image", "./assets/tilemaps/jungle.png");

	
	// Entity truck = registry->CreateEntity();



	// Load the tilemap
	// We need to load the tilemap texture from ./assets/tilemaps/jungle.png
	// We need to load the file ./assets/tilemaps/jungle.map (mapping of which tiles should be displayed from png)
	// Tip: You can use the idea of the source rectangle
	// Tip: Consider creating one entity per tile

	double defaultEntityScale = 2.5;

	int tileSize = 32;
	double tileScale = 5.0;
	int mapNumCols = 25;
	int mapNumRows = 20;
	std::fstream mapFile;
	mapFile.open("./assets/tilemaps/jungle.map");

	for (int y = 0; y < mapNumRows; y++) {
		for (int x = 0; x < mapNumCols; x++) {
			char ch;
			mapFile.get(ch);
			int srcRectY = std::atoi(&ch) * tileSize;
			mapFile.get(ch);
			int srcRectX = std::atoi(&ch) * tileSize;
			mapFile.ignore(); // skip the comma

			Entity tile = registry->CreateEntity();
			tile.AddComponent<TransformComponent>(glm::vec2(x * (tileScale * tileSize), y * (tileScale * tileSize)), glm::vec2(tileScale, tileScale), 0.0);
			tile.AddComponent<SpriteComponent>("tilemap-image", tileSize, tileSize, 0, srcRectX, srcRectY);

		}
	}


	Entity choppah = registry->CreateEntity();

	choppah.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(defaultEntityScale, defaultEntityScale), 0.0);
	choppah.AddComponent<RigidBodyComponent>(glm::vec2(50.0, 0.0));
	choppah.AddComponent<SpriteComponent>("chopper-image", 32, 32, 2);
	choppah.AddComponent<AnimationComponent>(2, 10);

	Entity radar = registry->CreateEntity();

	radar.AddComponent<TransformComponent>(glm::vec2(windowWidth - 74, 10.0), glm::vec2(1, 1), 0.0);
	radar.AddComponent<RigidBodyComponent>(glm::vec2(0.0, 0.0));
	radar.AddComponent<SpriteComponent>("radar-image", 64, 64, 2);
	radar.AddComponent<AnimationComponent>(8, 8);


	Entity tank = registry->CreateEntity();

	// registry->AddComponent<TransformComponent>(tank, glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	// registry->AddComponent<RigidBodyComponent>(tank, glm::vec2(50.0, 0.0));
	tank.AddComponent<TransformComponent>(glm::vec2(500.0, 10.0), glm::vec2(defaultEntityScale, defaultEntityScale), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(-20.0, 0.0));
	tank.AddComponent<SpriteComponent>("tank-image", 32, 32, 2);
	tank.AddComponent<BoxColliderComponent>(32, 32); // offset will be default

	Entity truck = registry->CreateEntity();

	truck.AddComponent<TransformComponent>(glm::vec2(10.0, 10.0), glm::vec2(defaultEntityScale, defaultEntityScale), 0.0);
	truck.AddComponent<RigidBodyComponent>(glm::vec2(20.0, 0.0));
	truck.AddComponent<SpriteComponent>("truck-image", 32, 32, 1);
	truck.AddComponent<BoxColliderComponent>(32, 32);
}

// glm::vec2 playerPosition;
// glm::vec2 playerVelocity;

void Game::Setup() {
	LoadLevel(1);
}

void Game::Update() {
	// TODO: if we are too fast, we free processing power for other processes until we reach MILISECONDS_PER_FRAME

	// Don't use the commented version, it wastes CPU cycles since it doesn't share resources while sleeping in the loop
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), milisecondsPreviousFrame + MILISECONDS_PER_FRAME));

	int timeToWait = MILISECONDS_PER_FRAME - (SDL_GetTicks() - milisecondsPreviousFrame);

	if (timeToWait > 0 && timeToWait <= MILISECONDS_PER_FRAME) { // checking for edge cases
		SDL_Delay(timeToWait);
	}

	// delta time is used for distance = delta(velocity) * delta(time) -> delta(time) == deltaTime
	double deltaTime = (SDL_GetTicks() - milisecondsPreviousFrame) / 1000.0;

	// Store the current frame time
	milisecondsPreviousFrame = SDL_GetTicks();


	// velocity
	// playerPosition.x += playerVelocity.x * deltaTime;
	// playerPosition.y += playerVelocity.y * deltaTime;

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<AnimationSystem>().Update();
	registry->GetSystem<CollisionSystem>().Update();
	// TODO: registry->GetSystem<CollisionSystem>().Update();
	
	// Update registry to process the entities that are waiting to be created/deleted
	registry->Update();

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


	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
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