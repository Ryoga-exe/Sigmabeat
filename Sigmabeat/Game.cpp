#include "Game.h"

bool Game::Initialize(const TCHAR* windowTitle) {
    DxSystem::Inst()->SetWindowSize({ 1280, 720 });
    if (DxSystem::Inst()->Initialize(windowTitle)) return true;
    return false;
}
bool Game::Finalize() {
    DxSystem::Inst()->Finalize();
    return false;
}
void Game::ProcessLoop() {
    while (SystemUpdate()) {
        Update();
        Draw();
    }
}

bool Game::SystemUpdate() {
    return (!DxLib::ScreenFlip() && !DxLib::ProcessMessage() && !DxLib::ClearDrawScreen());
}
bool Game::Update() {
    if (GetInputChar(TRUE) == ' ') {
        DxSystem::Inst()->ToggleFullscreenMode();
    }
    return false;
}
void Game::Draw() {

}