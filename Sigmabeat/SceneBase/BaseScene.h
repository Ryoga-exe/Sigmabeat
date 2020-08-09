#pragma once
#include "ISceneChanger.h"
#include "../Task.h"
#include "../DxSystem.h"

enum class SceneChangeEffect {
    None,
    Fade,
    Push
};

class BaseScene : public Task {
public:
    BaseScene(ISceneChanger* changer, unsigned int bgColor = 0U) {
        m_sceneChanger = changer;
        m_bgColor = bgColor;
        m_changeEffect = SceneChangeEffect::None;
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
    SceneChangeEffect m_changeEffect;
    int m_changeTime_ms;
    unsigned int m_bgColor;
};