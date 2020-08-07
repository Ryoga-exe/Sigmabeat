#pragma once
#include "../SceneBase/BaseScene.h"

class Setup : public BaseScene {
public:
    Setup(ISceneChanger* changer);
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
};