#include "DxSystem.h"

void ErrMsgBx(const TCHAR* errorMsg) {
    MessageBox(NULL, errorMsg, L"ERROR", MB_OK | MB_ICONERROR);
}

DxSystem::DxSystem() : m_hasInitialized(false), m_isFullscreen(false), m_enableChangeSize(true),
m_styleMode(7), m_windowSize({640, 480}), m_isMaxSize(false) {
    DxLib::SetOutApplicationLogValidFlag(FALSE);         // 一番先に行う
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8); // 上の次に行う
    DxLib::SetUseCharSet(DX_CHARSET_UTF8);

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
        ErrMsgBx(L"エラーが発生しました。\nウィンドウの生成に失敗しました。"); // language
        return true;
    }
    if (DxLib::SetDrawScreen(DX_SCREEN_BACK) != 0) {
        ErrMsgBx(L"エラーが発生しました。\nウィンドウの設定に失敗しました。");
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
int GetFileNum(const TCHAR* path, bool doCountDir) {
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
    if (fileHandle == nullptr || *fileHandle == 0) return false;
    if (ch == '\n') return true; // LF
    if (ch == '\r') {            // CR or CRLF
        TCHAR buffer = DxLib::FileRead_getc(*fileHandle);
        if (buffer == '\n') return true; // CRLF
        else {
            DxLib::FileRead_seek(*fileHandle, -1, SEEK_CUR);  // CR
            return true;
        }
    }
    return false;
}