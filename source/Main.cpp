#include "Game.h"
const TCHAR* title = u8"Sigmabeat";

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    Game game;
    if (game.Initialize(title)) return -1;
    game.ProcessLoop();
    game.Finalize();
    return 0;
}