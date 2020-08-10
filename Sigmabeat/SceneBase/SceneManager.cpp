#include "../Scene/Setup.h"
#include "../Scene/Title.h"
#include "../Scene/Menu.h"
#include "SceneManager.h"

SceneManager::SceneManager() : m_currentScene(Scene::Setup), m_nextScene(Scene::None), m_hasFinishedChanging(false), m_changedTime(0) {
    m_scenes.push_back(new Setup(this));
    m_scenes.push_back(new Title(this));
    m_scenes.push_back(new Menu(this));
} 

void SceneManager::Initialize() {
    for (auto scene : m_scenes) {
        scene->Initialize();
    }
    m_timer.Start();
    m_changedTime = m_timer.Elapse();
}

void SceneManager::Finalize() {
    for (auto scene : m_scenes) {
        scene->Finalize();
        delete scene;
    }
    m_scenes.clear();
}

void SceneManager::Update() {
    if (m_scenes.empty()) return;
    if (m_nextScene != Scene::None && m_hasFinishedChanging) {
        m_scenes[(int)m_currentScene]->Quit();
        m_currentScene = m_nextScene;
        m_nextScene = Scene::None;
        m_changedTime = m_timer.Elapse();
        m_scenes[(int)m_currentScene]->Changed();
    }
    m_scenes[(int)m_currentScene]->Update();
}

void SceneManager::Draw() {
    if (m_scenes.empty()) return;
    m_scenes[(int)m_currentScene]->Draw();
    m_hasFinishedChanging = DrawSceneChangeEffect(m_nextScene != Scene::None);
}

void SceneManager::ChangeScene(Scene NextScene) {
    if (m_nextScene != Scene::None) return;
    m_nextScene = NextScene;
    m_changedTime = m_timer.Elapse();
}

bool SceneManager::DrawSceneChangeEffect(bool isQuit) {
    SceneChangeEffect changeEffect = m_scenes[(int)m_currentScene]->GetSceneChangeEffect();
    double values[2] = {};
    int blendMode[2] = {};
    switch (changeEffect.effect) {
    case ChangeEffect::None:
        return isQuit;
    case ChangeEffect::Fade:
        GetDrawBlendMode(&blendMode[0], &blendMode[1]);
        values[0] = isQuit ? 0 : 255;
        values[1] = isQuit ? 255 : 0;
        DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, (int)easing::ease(m_timer, m_changedTime, m_changedTime + changeEffect.time_ms, values[0], values[1], changeEffect.easing));
        DrawBg(changeEffect.color);
        DxLib::SetDrawBlendMode(blendMode[0], blendMode[1]);
    default:
        break;
    }
    return m_changedTime + changeEffect.time_ms < m_timer.Elapse();
}