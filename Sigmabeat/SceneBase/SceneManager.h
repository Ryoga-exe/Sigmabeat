#pragma once
#include "ISceneChanger.h"
#include "BaseScene.h"

class SceneManager : public ISceneChanger, Task {
public:
    SceneManager();
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;

    void ChangeScene(Scene NextScene) override;

private:
    BaseScene* m_scene;
    Scene m_nextScene;
};