#pragma once
#include "ISceneChanger.h"
#include "../Task.h"
#include "../DxSystem.h"

class BaseScene : public Task {
public:
    BaseScene(ISceneChanger* changer) {
        m_sceneChanger = changer;
    }
    virtual ~BaseScene() {}
    virtual void Initialize() override {}
    virtual void Finalize() override {}
    virtual void Update() override {}
    virtual void Draw() override {}

    virtual void Changed() {}
    virtual void Quit() {}
    
protected:
    ISceneChanger* m_sceneChanger;
};