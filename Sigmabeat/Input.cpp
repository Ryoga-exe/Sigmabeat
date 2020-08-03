#include "DxLib.h"
#include "Input.h"
#include <math.h>

Keyboard::Keyboard() {
    memset(m_keyPressingCount, 0, sizeof(m_keyPressingCount));
    memset(m_keyReleasingCount, 0, sizeof(m_keyReleasingCount));
}

bool Keyboard::Update(bool doDrawKeyCode) {
    char nowKeyStatus[KEY_NUM];
    int counting = 0;
    DxLib::GetHitKeyStateAll(nowKeyStatus);
    for (int i = 0; i < KEY_NUM; i++) {
        if (nowKeyStatus[i] != 0) {
            if (m_keyReleasingCount[i] > 0) {
                m_keyReleasingCount[i] = 0;
            }
            m_keyPressingCount[i]++;
            if (doDrawKeyCode) {
                DxLib::DrawFormatString(0, 16 * counting, 0xffffff, L"%d", i);
                counting++;
            }
        }
        else {
            if (m_keyPressingCount[i] > 0) {
                m_keyPressingCount[i] = 0;
            }
            m_keyReleasingCount[i]++;
        }
    }
    return true;
}

int Keyboard::GetPressingCount(int keyCode) {
    if (!Keyboard::IsAvailableCode(keyCode)) return -1;
    return m_keyPressingCount[keyCode];
}

int Keyboard::GetReleasingCount(int keyCode) {
    if (!Keyboard::IsAvailableCode(keyCode)) return -1;
    return m_keyReleasingCount[keyCode];
}

Keyboard::State Keyboard::GetStatus(int keyCode) {
    if (!Keyboard::IsAvailableCode(keyCode)) return State::None;
    if (Keyboard::GetPressingCount(keyCode) == 1) return State::Pressed;
    if (Keyboard::GetPressingCount(keyCode) > 1) return State::Held;
    if (Keyboard::GetReleasingCount(keyCode) == 1) State::Released;
    return State::None;
}

bool Keyboard::IsAvailableCode(int keyCode) {
    if (!(0 <= keyCode && keyCode < KEY_NUM)) return false;
    return true;
}

/*-----------------------------------------*/

Mouse::Mouse() : m_buttonPressingCount(), m_buttonReleasingCount(), m_x(), m_y() {
}

bool Mouse::Update() {
    int nowButtonState = DxLib::GetMouseInput();
    DxLib::GetMousePoint(&m_x, &m_y);
    for (int i = 0; i < KEY_NUM; i++) {
        if ((nowButtonState >> i) & 1) {
            if (m_buttonReleasingCount[i] > 0) {
                m_buttonReleasingCount[i] = 0;
            }
            m_buttonPressingCount[i]++;
        }
        else {
            if (m_buttonPressingCount[i] > 0) {
                m_buttonPressingCount[i] = 0;
            }
            m_buttonReleasingCount[i]++;
        }
    }
    return true;
}

int Mouse::GetPressingCount(int keyCode) {
    if (!isAvailableCode(keyCode)) {
        return -1;
    }
    return m_buttonPressingCount[keyCode];
}

int Mouse::GetReleasingCount(int keyCode) {
    if (!isAvailableCode(keyCode)) {
        return -1;
    }
    return m_buttonReleasingCount[keyCode];
}

bool Mouse::IsMouseHoverRect(int x1, int y1, int x2, int y2) {
    int _x1 = x1, _y1 = y1, _x2 = x2, _y2 = y2;
    if (x1 > x2) {
        _x1 = x2;
        _x2 = x1;
    }
    if (y1 > y2) {
        _y1 = y2;
        _y2 = y1;
    }
    if (_x1 < m_x && _x2 > m_x && _y1 < m_y && _y2 > m_y) {
        return true;
    }
    return false;
}

bool Mouse::IsMouseHoverCircle(int x, int y, int r) {
    if (r > sqrt(pow(m_x - x, 2) + pow(m_y - y, 2))) {
        return true;
    }
    return false;
}

bool Mouse::isAvailableCode(int keyCode) {
    if (!(0 <= keyCode && keyCode < KEY_NUM)) {
        return false;
    }
    return true;
}

/*-----------------------------------------*/

bool input::UpdateAll(bool drawKeyCodeFlag) {
    Keyboard::Inst()->Update(drawKeyCodeFlag);
    Mouse::Inst()->Update();
    return false;
}