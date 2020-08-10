#include "Setup.h"

Setup::Setup(ISceneChanger* changer) : BaseScene(changer, DxLib::GetColor(255, 255, 255)) {
    m_myLogoImg = -1;
    m_changeEffect.effect = ChangeEffect::Fade;
    m_changeEffect.easing = easing::expoOut;
    m_changeEffect.color = DxLib::GetColor(255, 255, 255);
}

void Setup::Initialize() {
    m_myLogoImg = DxLib::LoadGraph(L"Data/Image/Ryoga-exe.png");
}

void Setup::Finalize() {
    if (m_myLogoImg != -1) DxLib::DeleteGraph(m_myLogoImg);
}

void Setup::Update() {
    if (Keyboard::Inst()->GetStatus(KEY(RETURN)) == Keyboard::State::Pressed) {
        m_sceneChanger->ChangeScene(Scene::Title);
    }
}

void Setup::Draw() {
    BaseScene::Draw();
    RectSize winSize = DxSystem::Inst()->GetWindowSize();

    int begin = 200;
    for (int i = begin; i < 256; i++) {
        DxLib::DrawLine(0, i - begin, winSize.width, i - begin, GetColor(i, i, i));
        DxLib::DrawLine(0, winSize.height - i + begin, winSize.width, winSize.height - i + begin, GetColor(i, i, i));
    }

    int shorterSide = min(winSize.width, winSize.height);
    DxLib::DrawRotaGraph((int)(winSize.width / 2), (int)(winSize.height / 2), shorterSide / 130.0, 0.0, m_myLogoImg, TRUE);
}