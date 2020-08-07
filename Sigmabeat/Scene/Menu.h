#pragma once
#include "../SceneBase/BaseScene.h"

class Menu : public BaseScene {
public:
    Menu(ISceneChanger* changer);
    void Initialize() override;
    void Finalize() override;
    void Update() override;
    void Draw() override;
    
private:
    int m_count;
};