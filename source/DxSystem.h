#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "Input.h"
#include "Time/Timer.h"
#include "Easing/Easing.h"
#include "Color/Color.h"

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
int  DrawBg(int grHandle, ColorRect_t color, int isTrans);
int  GetFileNum(const TCHAR* path, bool doCountDir = false);
bool FileRead_isBr(const int* fileHandle, TCHAR ch);
int  DrawStringToHandleCenter(int y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawStringFToHandleCenter(float y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawStringToHandleRight(int y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawStringToHandleBoxCenter(int x1, int y1, int x2, int y2, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawZoomStringToHandleBoxCenter(int x1, int y1, int x2, int y2, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawFormatStringToHandleCenter(int y, unsigned int color, int fontHandle, const TCHAR* string, ...);
int  DrawZoomStringToHandleCenter(int y, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawZoomStringFToHandleCenter(float y, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
int  DrawRhombus(int x, int y, int r, unsigned int color, int fillFlag);
int  DrawRhombusAA(float x, float y, float r, unsigned int color, int fillFlag, float lineThickness = 1.0f);
int  DrawGaugeBoxAA(float x1, float y1, float x2, float y2, int currentValue, int maxValue, unsigned int color, int direction = 0);
int  DrawGraphOnPolygon3DRect(DxLib::VECTOR pos1, DxLib::VECTOR pos2, int grHandle, int transFlag);