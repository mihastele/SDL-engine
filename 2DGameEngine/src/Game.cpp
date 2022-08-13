
// include with "" is looking in the local folder, <> (angle brackets) look in the OS include folders
#include "Game.h" // sibling file in this folder
#include <iostream>
#include <SDL.h>

// scope resolution::contructor method
Game::Game() {
	std::cout << "Game Constructor called" << std::endl;
}

Game::~Game() {

}

void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error Initilaizing SDL" << std::endl;
		return;
	}

	// instead of copying entire thing to memory, it only stores the address of the struct
	SDL_Window* window = SDL_CreateWindow("Game made by the 2D Engine of Miha Stele",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		800,
		600,
		SDL_WINDOW_BORDERLESS);

	if (!window) {
		std::cerr << "Error creating SDL window." << std::endl;
		return;
	}

	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
	}
}

void Game::Run() {
	while (true) {
		ProcessInput();
		Update();
		Render();
	}

}

void Game::ProcessInput() {

}

void Game::Update() {

}

void Game::Render() {

}

void Game::Destroy() {

}