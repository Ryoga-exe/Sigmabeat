#include "Title.h"

Title::Title(ISceneChanger* changer) : BaseScene(changer) {
}

void Title::Initialize() {
}

void Title::Finalize() {
}

void Title::Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {
        m_sceneChanger->ChangeScene(Scene::Setup);
    }
    if (CheckHitKey(KEY_INPUT_RETURN) != 0) {
        m_sceneChanger->ChangeScene(Scene::Menu);
    }
}

void Title::Draw() {
    BaseScene::Draw();
    DrawString(0, 0, L"Scene : Title", GetColor(255, 255, 255));
}