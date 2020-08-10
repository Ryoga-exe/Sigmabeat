#include "Menu.h"

Menu::Menu(ISceneChanger* changer) : BaseScene(changer), m_count(0) {
}

void Menu::Initialize() {
}

void Menu::Finalize() {
}

void Menu::Changed() {
}

void Menu::Update() {
    if (CheckHitKey(KEY_INPUT_1) != 0) {
        m_sceneChanger->ChangeScene(Scene::Title);
    }
    if (CheckHitKey(KEY_INPUT_A) != 0) {
        m_count++;
    }
}

void Menu::Draw() {
    BaseScene::Draw();
    DrawString(0, 0, L"Scene : Menu", GetColor(255, 255, 255));

    DrawFormatString(0, 20, 0xffffff, L"%d", m_count);
}