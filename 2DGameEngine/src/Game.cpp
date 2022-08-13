
// include with "" is looking in the local folder, <> (angle brackets) look in the OS include folders
#include "Game.h" // sibling file in this folder
#include <iostream>

// scope resolution::contructor method
Game::Game() {
	std::cout << "Game Constructor called" << std::endl;
}

Game::~Game() {

}

void Game::Initialize() {

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