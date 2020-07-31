#pragma once
#include "DxSystem.h"

class Game {
public:
    bool Initialize(const TCHAR* windowTitle);
    bool Finalize();
    void ProcessLoop();
private:
    bool SystemUpdate();
    bool Update();
    void Draw();

};