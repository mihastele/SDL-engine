#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <glm/glm.hpp>
#include <imgui/imgui.h>
#include <sol/sol.hpp>

#include "Game/Game.h"

// int argc, char* argv[] is required on Windows!
int main(int argc, char* argv[]) {
    // TODO: Do some magic!

    // since we are not using "new" keyword, this object will be stored in stack and will be destroyed when the scope ends
    Game game;

    game.Initialize();
    game.Run();
    game.Destroy();


    return 0;
}