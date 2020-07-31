#pragma once
#include "DxLib.h"
#include "Singleton.h"

struct coordI {
    int x, y;
};

struct RectSize {
    int width, height;
};

class DxSystem : public Singleton<DxSystem> {
    DxSystem();
    ~DxSystem();
    friend Singleton< DxSystem >;
public:
    bool Initialize(const TCHAR* windowTitle);
    bool Finalize();
    bool Update();

    bool SetWindowSize(const RectSize size);
    RectSize GetWindowSize();

private:
    bool m_hasInitialized,
        m_isFullscreen,
        m_enableChangeSize;

    int  m_styleMode;

    RectSize m_desktopSize, m_windowSize;
    int   m_colorDepth;
};