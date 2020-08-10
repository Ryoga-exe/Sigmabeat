#pragma once
#include "ISceneChanger.h"
#include "BaseScene.h"
#include <vector>

class SceneManager : public ISceneChanger, Task {
public:
    SceneManager();
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

    void ChangeScene(Scene NextScene) override;

private:
    bool DrawSceneChangeEffect(bool isQuit);

    std::vector<BaseScene*> m_scenes;
    Scene m_currentScene, m_nextScene;
    bool m_hasFinishedChanging;

    Timer m_timer;
    int   m_changedTime;
};