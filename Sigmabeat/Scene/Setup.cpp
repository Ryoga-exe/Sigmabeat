#include "Setup.h"

Setup::Setup(ISceneChanger* changer) : BaseScene(changer) {
}

void Setup::Initialize() {
}

void Setup::Finalize() {
}

void Setup::Update() {
    if (CheckHitKey(KEY_INPUT_SPACE) != 0) {
        m_sceneChanger->ChangeScene(Scene::Title);
    }
}

void Setup::Draw() {
    BaseScene::Draw();
    DrawString(0, 0, L"Scene : Setup", GetColor(255, 255, 255));
}