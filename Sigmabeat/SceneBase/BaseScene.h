#pragma once
#include "ISceneChanger.h"
#include "../Task.h"
#include "../DxSystem.h"

enum class ChangeEffect {
    None,
    Fade,
    Push
};

struct SceneChangeEffect {
    ChangeEffect      effect;
    easing::easeParam easing;
    int               time_ms;
    unsigned int      color;
    int               direction;
};

class BaseScene : public Task {
public:
    BaseScene(ISceneChanger* changer, unsigned int bgColor = 0U) {
        m_sceneChanger = changer;
        m_bgColor = bgColor;
        m_changeEffect = { ChangeEffect::None, easing::linear, 1.0_sec, 0U, 0 };
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
    SceneChangeEffect GetSceneChangeEffect() { return m_changeEffect; }

protected:
    ISceneChanger* m_sceneChanger;
    SceneChangeEffect  m_changeEffect;
    unsigned int m_bgColor;
};