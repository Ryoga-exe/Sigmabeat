#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "Input.h"
#include "Time/Timer.h"
#include "Easing/Easing.h"
#include "Color/Color.h"
#include "TString/TString.h"

struct CoordI {
    int x, y;
};

struct RectSize {
    int width, height;
};

void ErrMsgBx(const TCHAR* errorMsg);

class DxSystem : public Singleton<DxSystem> {
    DxSystem();
    ~DxSystem();
    friend Singleton<DxSystem>;
public:
    bool Initialize(const TCHAR* windowTitle);
    bool Finalize();
    bool Update();

    bool SetFullscreenMode(const bool isFullscreen);
    bool ToggleFullscreenMode();
    bool SetWindowSize(const RectSize size);
    bool SetWindowSizeChangeEnable(const bool enable);

    bool GetIsFullscreen() { return m_isFullscreen; }
    RectSize GetWindowSize();

private:
    bool m_hasInitialized,
         m_isFullscreen,
         m_enableChangeSize,
         m_isMaxSize;

    int  m_styleMode;
    int  m_colorDepth;

    RectSize m_desktopSize, m_windowSize;
    RectSize m_windowPos;
};

int  operator""_sec(const long double second);
int  DrawBg(unsigned int color);
int  GetFileNum(const TCHAR* path, bool doCountDir = false);
bool FileRead_isBr(const int* fileHandle, TCHAR ch);