#include "../Scene/Setup.h"
#include "../Scene/Title.h"
#include "../Scene/Menu.h"
#include "SceneManager.h"

SceneManager::SceneManager() : m_currentScene(Scene::Setup), m_nextScene(Scene::None) {
    m_scenes.push_back(new Setup(this));
    m_scenes.push_back(new Title(this));
    m_scenes.push_back(new Menu(this));
}

void SceneManager::Initialize() {
    for (auto scene : m_scenes) {
        scene->Initialize();
    }
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
    if (m_nextScene != Scene::None) {
        m_scenes[(int)m_currentScene]->Quit();
        m_currentScene = m_nextScene;
        m_nextScene = Scene::None;
        m_scenes[(int)m_currentScene]->Changed();
    }
    m_scenes[(int)m_currentScene]->Update();
}

void SceneManager::Draw() {
    if (m_scenes.empty()) return;
    m_scenes[(int)m_currentScene]->Draw();
}

void SceneManager::ChangeScene(Scene NextScene) {
    m_nextScene = NextScene;
}