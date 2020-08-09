#pragma once
#include "ISceneChanger.h"
#include "../Task.h"
#include "../DxSystem.h"

class BaseScene : public Task {
public:
    BaseScene(ISceneChanger* changer, unsigned int bgColor = 0U) {
        m_sceneChanger = changer;
        m_bgColor = bgColor;
    }
    virtual ~BaseScene() {}
    virtual void Initialize() override {}
    virtual void Finalize() override {}
    virtual void Update() override {}
    virtual void Draw() override {
        DrawBg(m_bgColor);
    }

    virtual void Changed() {}
    virtual void Quit() {}
    
protected:
    ISceneChanger* m_sceneChanger;
    unsigned int m_bgColor;
};