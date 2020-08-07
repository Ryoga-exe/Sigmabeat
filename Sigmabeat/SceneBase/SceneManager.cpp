#include "../Scene/Setup.h"
#include "../Scene/Title.h"
#include "../Scene/Menu.h"
#include "SceneManager.h"

SceneManager::SceneManager() : m_nextScene(Scene::None) {
    m_scene = (BaseScene*) new Setup(this);
}

void SceneManager::Initialize() {
    m_scene->Initialize();
}

void SceneManager::Finalize() {
    m_scene->Finalize();
}

void SceneManager::Update() {
    if (m_nextScene != Scene::None) {
        m_scene->Finalize();
        delete m_scene;
        switch (m_nextScene) {
        case Scene::Setup:
            m_scene = (BaseScene*) new Setup(this);
            break;
        case Scene::Title:
            m_scene = (BaseScene*) new Title(this);
            break;
        case Scene::Menu:
            m_scene = (BaseScene*) new Menu(this);
            break;
        }
        m_nextScene = Scene::None;
        m_scene->Initialize();
    }

    m_scene->Update();
}

void SceneManager::Draw() {
    m_scene->Draw();
}

void SceneManager::ChangeScene(Scene NextScene) {
    m_nextScene = NextScene;
}