#pragma once
#include "DxSystem.h"
#include "SceneBase/SceneManager.h"
#include "ScoreManager.h"

class Game {
public:
    bool Initialize(const TCHAR* windowTitle);
    bool Finalize();
    void ProcessLoop();
private:
    bool SystemUpdate();
    bool Update();
    void Draw();

    SceneManager m_sceneMgr;
};