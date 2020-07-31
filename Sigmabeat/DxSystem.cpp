#include "DxSystem.h"

DxSystem::DxSystem() : m_hasInitialized(false), m_isFullscreen(false), m_enableChangeSize(false),
m_styleMode(7), m_windowSize({640, 480}) {
    DxLib::SetOutApplicationLogValidFlag(FALSE);         // 一番先に行う
    DxLib::SetUseCharCodeFormat(DX_CHARCODEFORMAT_UTF8); // 上の次に行う

    DxLib::GetDefaultState(&m_desktopSize.width, &m_desktopSize.height, &m_colorDepth);
    DxLib::SetWindowStyleMode(m_styleMode);
    DxLib::SetWindowInitPosition((int)((m_desktopSize.width - m_windowSize.width) / 2), (int)((m_desktopSize.height - m_windowSize.height) / 2));
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


    DxLib::DxLib_Init();

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


    return false;
}

bool DxSystem::SetWindowSize(const RectSize size) {
    if (size.width <= 0 || size.height <= 0) return true;
    m_windowSize = size;
    return DxLib::SetWindowSize(size.width, size.height) == 0 ? false : true;
}

RectSize DxSystem::GetWindowSize() {
    if (!m_enableChangeSize) {
        return m_windowSize;
    }
    RectSize size;
    DxLib::GetWindowSize(&size.width, &size.height);
    return size;
}