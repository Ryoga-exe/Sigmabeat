#include "Menu.h"

Menu::Menu(ISceneChanger* changer) : BaseScene(changer, DxLib::GetColor(255, 255, 255)) {
    m_changeEffect.effect = ChangeEffect::Fade;
    m_changeEffect.easing = easing::expoOut;
    m_changeEffect.color = DxLib::GetColor(255, 255, 255);
}

void Menu::Initialize() {
}

void Menu::Finalize() {
}

void Menu::Changed() {
}

void Menu::Update() {
    if (Keyboard::Inst()->GetStatus(KEY(ESCAPE)) == Keyboard::State::Pressed) {
        m_sceneChanger->ChangeScene(Scene::Title);
    }
}

void Menu::Draw() {
    BaseScene::Draw();
    
}