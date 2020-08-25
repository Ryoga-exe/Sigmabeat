#include "DxSystem.h"
#include <cstdarg>

void ErrMsgBx(const TCHAR* errorMsg) {
    MessageBox(NULL, errorMsg, "ERROR", MB_OK | MB_ICONERROR);
}

DxSystem::DxSystem() : m_hasInitialized(false), m_isFullscreen(false), m_enableChangeSize(true),
m_styleMode(7), m_windowSize({640, 480}), m_isMaxSize(false) {
    DxLib::SetOutApplicationLogValidFlag(FALSE);         // 一番先に行う
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8); // 上の次に行う

    DxLib::GetDefaultState(&m_desktopSize.width, &m_desktopSize.height, &m_colorDepth);
    DxLib::SetWindowStyleMode(m_styleMode);
    m_windowPos = { (int)((m_desktopSize.width - m_windowSize.width) / 2), (int)((m_desktopSize.height - m_windowSize.height) / 2) };
    if (m_styleMode != 8) DxLib::SetWindowInitPosition(m_windowPos.width, m_windowPos.height);
    // 仮想フルスクリーンの各種設定
    DxLib::SetWindowSizeChangeEnableFlag(m_enableChangeSize, FALSE);
    DxLib::ChangeWindowMode(TRUE);
    DxLib::SetGraphMode(m_desktopSize.width, m_desktopSize.height, m_colorDepth);
    DxLib::SetWindowSize(m_windowSize.width, m_windowSize.height);
}

DxSystem::~DxSystem() {
    Finalize();
}

bool DxSystem::Initialize(const TCHAR* windowTitle) {
    if (m_hasInitialized) return true;
    if (windowTitle == nullptr) return true;
    DxLib::SetMainWindowText(windowTitle);
    DxLib::SetAlwaysRunFlag(TRUE);
    

    if (DxLib::DxLib_Init() != 0) {
        ErrMsgBx("エラーが発生しました。\nウィンドウの生成に失敗しました。"); // language
        return true;
    }
    if (DxLib::SetDrawScreen(DX_SCREEN_BACK) != 0) {
        ErrMsgBx("エラーが発生しました。\nウィンドウの設定に失敗しました。");
        DxLib::DxLib_End();
        return true;
    }

    DxLib::SetSysCommandOffFlag(TRUE);
    m_hasInitialized = true;
    return false;
}

bool DxSystem::Finalize() {
    if (!m_hasInitialized) return true;
    DxLib::DxLib_End();
    m_hasInitialized = false;
    return false;
}

bool DxSystem::Update() {
    /* !m_enableChangeSize && m_isFullscreen だったら、ここで拡大して描画するやつ */
    /*
    SetDrawArea(0, 0, WindowW, WindowH);

    // 3D描画の中心をウインドウの中心に設定する
    SetCameraScreenCenter(WindowW / 2.0f, WindowH / 2.0f);

    // 3D描画のスケールをウインドウの縦幅によって変更する
    SetDraw3DScale((float)WindowH / DesktopH);
    */
    return false;
}

bool DxSystem::SetFullscreenMode(const bool isFullscreen) {
    if (m_isFullscreen == isFullscreen) return false;
    if (isFullscreen) {
        DxLib::SetWindowStyleMode(m_styleMode);
        DxLib::GetWindowPosition(&m_windowPos.width, &m_windowPos.height);
        DxLib::GetWindowSize(&m_windowSize.width, &m_windowSize.height);
        m_isMaxSize = (DxLib::GetWindowMaxSizeFlag() == TRUE);
        DxLib::SetWindowSizeChangeEnableFlag(FALSE, FALSE);
        // DxLib::SetWindowZOrder(DX_WIN_ZTYPE_TOPMOST);
        DxLib::SetWindowSize(m_desktopSize.width, m_desktopSize.height);
        if (m_styleMode != 2) DxLib::SetWindowStyleMode(2);
        DxLib::SetWindowPosition(0, 0);
    }
    else {
        DxLib::SetWindowSizeChangeEnableFlag(m_enableChangeSize, FALSE);
        //DxLib::SetWindowZOrder(DX_WIN_ZTYPE_NORMAL);
        if (m_isMaxSize && m_styleMode == 7) DxLib::SetWindowStyleMode(8);
        else DxLib::SetWindowStyleMode(m_styleMode);
        DxLib::SetWindowPosition(m_windowPos.width, m_windowPos.height);
        DxLib::SetWindowSize(m_windowSize.width, m_windowSize.height);
    }
    m_isFullscreen = isFullscreen;
    return false;
}
bool DxSystem::ToggleFullscreenMode() {
    return SetFullscreenMode(!m_isFullscreen);
}

bool DxSystem::SetWindowSize(const RectSize size) {
    if (size.width <= 0 || size.height <= 0) return true;
    if (m_isFullscreen) return true;
    m_windowSize = size;
    if (!m_hasInitialized) {
        m_windowPos = { (int)((m_desktopSize.width - size.width) / 2), (int)((m_desktopSize.height - size.height) / 2) };
        if (m_styleMode != 8) DxLib::SetWindowInitPosition(m_windowPos.width, m_windowPos.height);
    }
    return DxLib::SetWindowSize(size.width, size.height) == 0 ? false : true;
}
bool DxSystem::SetWindowSizeChangeEnable(const bool enable) {
    if (m_enableChangeSize == enable) return false;
    m_enableChangeSize = enable;
    return DxLib::SetWindowSizeChangeEnableFlag(enable, FALSE) == 0 ? false : true;
}

RectSize DxSystem::GetWindowSize() {
    if (!m_enableChangeSize) {
        return m_windowSize;
    }
    RectSize size;
    DxLib::GetWindowSize(&size.width, &size.height);
    return size;
}

int  operator""_sec(const long double second) {
    return (int)(second * 1000.0);
}
int  DrawBg(unsigned int color) {
    RectSize winSize = DxSystem::Inst()->GetWindowSize();
    return DxLib::DrawBox(0, 0, winSize.width, winSize.height, color, TRUE);
}
int  DrawBg(int grHandle, ColorRect_t color, int isTrans) {
    DxLib::VERTEX2D m_vertex[4];
    const WORD m_index[6] = { 0,1,2,2,3,0 };
    const DxLib::COLOR_U8 m_colorTable[4] = {
        color.col1.GetU8(255), color.col2.GetU8(255), color.col3.GetU8(255), color.col4.GetU8(255)
    };
    RectSize winSize = DxSystem::Inst()->GetWindowSize();
    const DxLib::VECTOR m_posTable[4] = {
        {0.f, 0.f, 0.f}, {(float)winSize.width , 0.f, 0.f},
        {(float)winSize.width, (float)winSize.height, 0.f}, {0.f, (float)winSize.height, 0.f}
    };
    for (int i = 0; i < 4; i++) {
        m_vertex[i].u = m_vertex[i].v = 1.f;
        m_vertex[i].rhw = 1.f;
        m_vertex[i].dif = m_colorTable[i];
        m_vertex[i].pos = m_posTable[i];
    }
    m_vertex[0].u = m_vertex[0].v = m_vertex[1].v = m_vertex[3].u = 0.f;
    m_vertex[1].u = m_vertex[2].u = m_vertex[2].v = m_vertex[3].v = 1.f;
    return DxLib::DrawPolygonIndexed2D(m_vertex, 4, m_index, 2, grHandle, isTrans);
}
int  GetFileNum(const TCHAR* path, bool doCountDir) {
    int fileNum = 0;
    FILEINFO fileInfo;
    DWORD_PTR findHandle = DxLib::FileRead_findFirst(path, &fileInfo);
    if (findHandle != (DWORD_PTR)-1) {
        do {
            if (!doCountDir && fileInfo.DirFlag) continue;
            fileNum++;
        } while (DxLib::FileRead_findNext(findHandle, &fileInfo) >= 0);
    }
    else {
        return -1;
    }
    DxLib::FileRead_findClose(findHandle);
    return fileNum;
}
bool FileRead_isBr(const int* fileHandle, TCHAR ch) {
    if (fileHandle == nullptr || *fileHandle == NULL) return false;
    if (ch == u8'\n') return true; // LF
    if (ch == u8'\r') {            // CR or CRLF
        TCHAR buffer = DxLib::FileRead_getc(*fileHandle);
        if (buffer == u8'\n') return true; // CRLF
        else {
            DxLib::FileRead_seek(*fileHandle, -1, SEEK_CUR);  // CR
            return true;
        }
    }
    return false;
}
int  DrawStringToHandleCenter(int y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    return DxLib::DrawStringToHandle((DxSystem::Inst()->GetWindowSize().width - DxLib::GetDrawStringWidthToHandle(string, -1, fontHandle)) / 2, y, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawStringFToHandleCenter(float y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    return DxLib::DrawStringFToHandle((DxSystem::Inst()->GetWindowSize().width - DxLib::GetDrawStringWidthToHandle(string, -1, fontHandle)) / 2.f, y, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawStringToHandleRight(int y, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    return DxLib::DrawStringToHandle((DxSystem::Inst()->GetWindowSize().width - DxLib::GetDrawStringWidthToHandle(string, -1, fontHandle)), y, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawStringToHandleBoxCenter(int x1, int y1, int x2, int y2, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    int sizeX, sizeY, lineNum;
    int _x1 = x1, _y1 = y1, _x2 = x2, _y2 = y2;
    if (x1 > x2) {
        _x1 = x2;
        _x2 = x1;
    }
    if (y1 > y2) {
        _y1 = y2;
        _y2 = y1;
    }
    DxLib::GetDrawStringSizeToHandle(&sizeX, &sizeY, &lineNum, string, -1, fontHandle);
    return DxLib::DrawStringToHandle(_x1 + ((_x2 - _x1) - sizeX) / 2, _y1 + ((_y2 - _y1) - sizeY) / 2, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawZoomStringToHandleBoxCenter(int x1, int y1, int x2, int y2, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    int sizeX, sizeY, lineNum;
    int _x1 = x1, _y1 = y1, _x2 = x2, _y2 = y2;
    if (x1 > x2) {
        _x1 = x2;
        _x2 = x1;
    }
    if (y1 > y2) {
        _y1 = y2;
        _y2 = y1;
    }
    DxLib::GetDrawStringSizeToHandle(&sizeX, &sizeY, &lineNum, string, -1, fontHandle);
    return DxLib::DrawExtendStringToHandle((int)(_x1 + ((_x2 - _x1) - sizeX * zoomRate) / 2), (int)(_y1 + ((_y2 - _y1) - sizeY * zoomRate) / 2), zoomRate, zoomRate, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawFormatStringToHandleCenter(int y, unsigned int color, int fontHandle, const TCHAR* string, ...) {
    va_list argp;
    va_start(argp, string);
    TCHAR pszBuf[256];
    vsnprintfDx(pszBuf, sizeof(pszBuf), string, argp);
    va_end(argp);
    return DrawStringToHandleCenter(y, pszBuf, color, fontHandle, 0U, FALSE);
}
int  DrawZoomStringToHandleCenter(int y, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    return DxLib::DrawExtendStringToHandle((int)((DxSystem::Inst()->GetWindowSize().width - (DxLib::GetDrawStringWidthToHandle(string, -1, fontHandle) * zoomRate)) / 2), y, zoomRate, zoomRate, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawZoomStringFToHandleCenter(float y, double zoomRate, const TCHAR* string, unsigned int color, int fontHandle, unsigned int edgeColor, int verticalFlag) {
    return DxLib::DrawExtendStringFToHandle((float)(DxSystem::Inst()->GetWindowSize().width - (DxLib::GetDrawStringWidthToHandle(string, -1, fontHandle) * zoomRate)) / 2.f, y, zoomRate, zoomRate, string, color, fontHandle, edgeColor, verticalFlag);
}
int  DrawRhombus(int x, int y, int r, unsigned int color, int fillFlag) {
    return DxLib::DrawQuadrangle(x, y - r, x - r, y, x, y + r, x + r, y, color, TRUE);
}
int  DrawRhombusAA(float x, float y, float r, unsigned int color, int fillFlag, float lineThickness) {
    return DxLib::DrawQuadrangleAA(x, y - r, x - r, y, x, y + r, x + r, y, color, fillFlag, lineThickness);
}
int  DrawGaugeBoxAA(float x1, float y1, float x2, float y2, int currentValue, int maxValue, unsigned int color, int direction) {
    float _x1 = x1, _y1 = y1, _x2 = x2, _y2 = y2;
    if (x1 > x2) {
        _x1 = x2;
        _x2 = x1;
    }
    if (y1 > y2) {
        _y1 = y2;
        _y2 = y1;
    }
    if (currentValue > maxValue) currentValue = maxValue;
    return DxLib::DrawBoxAA(_x1, _y1, _x1 + (_x2 - _x1) * currentValue / maxValue, _y2, color, TRUE);
}
int  DrawGraphOnPolygon3DRect(DxLib::VECTOR pos1, DxLib::VECTOR pos2, int grHandle, int transFlag) {
    DxLib::VERTEX3D vertex[6];
    for (int i = 0; i < 6; i++) {
        vertex[i].su = vertex[i].sv = 0.f;
        vertex[i].norm = DxLib::VGet(0.f, 0.f, -1.f);
        vertex[i].dif = DxLib::GetColorU8(255, 255, 255, 255);
        vertex[i].spc = DxLib::GetColorU8(0, 0, 0, 255);
    }
    vertex[0].u = vertex[0].v = vertex[1].v = vertex[2].u = vertex[4].u = vertex[5].v = 0.f;
    vertex[1].u = vertex[2].v = vertex[3].u = vertex[3].v = vertex[4].v = vertex[5].u = 1.f;

    vertex[0].pos = pos1;
    vertex[1].pos = vertex[5].pos = DxLib::VGet(pos2.x, pos1.y, pos1.z);
    vertex[2].pos = vertex[4].pos = DxLib::VGet(pos1.x, pos2.y, pos2.z);
    vertex[3].pos = pos2;
    return DxLib::DrawPolygon3D(vertex, 2, grHandle, TRUE);
}