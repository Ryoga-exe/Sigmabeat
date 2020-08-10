#include "Timer.h"

Timer::Timer() : m_isPaused(false), m_startTime(0) {
    Start();
}

void Timer::Start() {
    m_isPaused = false;
    m_startTime = DxLib::GetNowCount();
}

int  Timer::Elapse() {
    if (m_isPaused) return m_pausedTime - m_startTime;
    return DxLib::GetNowCount() - m_startTime;
}

bool Timer::Pause() {
    if (m_isPaused) return true;
    m_pausedTime = DxLib::GetNowCount();
    m_isPaused = true;
    return false;
}

bool Timer::Resume() {
    if (!m_isPaused) return true;
    m_startTime = DxLib::GetNowCount() - (m_pausedTime - m_startTime);
    m_isPaused = false;
    return false;
}

bool Timer::TogglePause() {
    if (m_isPaused) {
        Resume();
        return false;
    }
    Pause();
    return true;
}