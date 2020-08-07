#include "Game.h"

bool Game::Initialize(const TCHAR* windowTitle) {
    DxSystem::Inst()->SetWindowSize({ 1280, 720 });
    if (DxSystem::Inst()->Initialize(windowTitle)) return true;
    m_sceneMgr.Initialize();
    return false;
}
bool Game::Finalize() {
    m_sceneMgr.Finalize();
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
    DxSystem::Inst()->Update();
    input::UpdateAll();
    return (!DxLib::ScreenFlip() && !DxLib::ProcessMessage() && !DxLib::ClearDrawScreen());
}
bool Game::Update() {
    if (Keyboard::Inst()->GetStatus(KEY(F11)) == Keyboard::State::Pressed) {
        DxSystem::Inst()->ToggleFullscreenMode();
    }
    m_sceneMgr.Update();

    return false;
}
void Game::Draw() {
    m_sceneMgr.Draw();
}