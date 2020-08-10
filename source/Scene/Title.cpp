#include "Title.h"

Title::Title(ISceneChanger* changer) : BaseScene(changer, DxLib::GetColor(255, 255, 255)) {
    m_logo = -1;
    m_changeEffect.effect = ChangeEffect::Fade;
    m_changeEffect.easing = easing::expoOut;
    m_changeEffect.color = DxLib::GetColor(255, 255, 255);
}

void Title::Initialize() {
    m_logo = DxLib::LoadGraph(L"Data/Image/logo.png");
}

void Title::Finalize() {
    if (m_logo != -1) DxLib::DeleteGraph(m_logo);
}

void Title::Update() {
    if (Keyboard::Inst()->GetStatus(KEY(ESCAPE)) == Keyboard::State::Pressed) {
        m_sceneChanger->ChangeScene(Scene::Setup);
    }
    if (Keyboard::Inst()->GetStatus(KEY(RETURN)) == Keyboard::State::Pressed) {
        m_sceneChanger->ChangeScene(Scene::Menu);
    }
}

void Title::Draw() {
    BaseScene::Draw();
    RectSize winSize = DxSystem::Inst()->GetWindowSize();
    DxLib::SetDrawMode(DX_DRAWMODE_BILINEAR);
    int shorterSide = min(winSize.width, winSize.height);
    DxLib::DrawRotaGraphF(winSize.width / 2.f, winSize.height / 2.f, shorterSide / 1000.0, 0.0, m_logo, TRUE);
    DxLib::SetDrawMode(DX_DRAWMODE_NEAREST);
}