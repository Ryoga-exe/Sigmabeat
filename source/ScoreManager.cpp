#include "ScoreManager.h"
#include <stdexcept>

ScoreManager::ScoreManager() : m_hasInitialized(false), m_isError(false), m_hasLoadedScore(false),
m_fileNum(0), m_defaultImageHandle(-1) {
}

ScoreManager::~ScoreManager() {
    Finalize();
}

bool ScoreManager::SetScorePath() {
    FILEINFO fileInfo;
    DWORD_PTR findHandle = DxLib::FileRead_findFirst(SCORE_DIRECTORY, &fileInfo);
    if (findHandle != (DWORD_PTR)-1) {
        int i = 0;
        do {
            if (fileInfo.DirFlag) continue;
            if (strstrDx(fileInfo.Name, SCORE_EXTENSION) == NULL) continue;
            m_scoreFiles[i].scorePath.clear();
            m_scoreFiles[i].scorePath = L"Scores/";
            m_scoreFiles[i].scorePath += fileInfo.Name;
            i++;
        } while (DxLib::FileRead_findNext(findHandle, &fileInfo) >= 0 || i < m_fileNum);
    }
    else {
        m_isError = true;
        return true;
    }
    DxLib::FileRead_findClose(findHandle);
    return false;
}

bool ScoreManager::Initialize() {
    bool prevLoadFlag = DxLib::GetUseASyncLoadFlag();
    int  prevSoundDataType = DxLib::GetCreateSoundDataType();
    if (prevLoadFlag) DxLib::SetUseASyncLoadFlag(FALSE);
    if (prevSoundDataType != DX_SOUNDDATATYPE_MEMPRESS) DxLib::SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

    m_fileNum = ::GetFileNum(SCORE_DIRECTORY);
    m_defaultImageHandle = DxLib::LoadGraph(m_defaultImagePath);

    if (m_fileNum == -1) {
        m_fileNum = 0;
    }
    else {
        try {
            m_scoreFiles = new ScoreInfo_t[m_fileNum];
        }
        catch (std::bad_alloc) {
            m_isError = true;
            ErrMsgBx(L"Error!!\nMemory Allocation Failed.");
        }
    }

    if (prevLoadFlag) DxLib::SetUseASyncLoadFlag(TRUE);
    if (prevSoundDataType != DX_SOUNDDATATYPE_MEMPRESS) DxLib::SetCreateSoundDataType(prevSoundDataType);

    m_hasInitialized = true;
    return m_isError;
}

bool ScoreManager::Finalize() {
    if (!m_hasInitialized) return true;
    if (m_defaultImageHandle != -1) DxLib::DeleteGraph(m_defaultImageHandle);
    if (!m_isError && m_fileNum != 0) {
        for (int i = 0; i < m_fileNum; i++) {
            if (m_scoreFiles[i].imageHandle != -1) DxLib::DeleteGraph(m_scoreFiles[i].imageHandle);
        }
        delete[] m_scoreFiles;
        m_scoreFiles = nullptr;
    }
    m_hasInitialized = m_hasLoadedScore = false;
    return false;
}

bool ScoreManager::SkipSpace(const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR chbuf = DxLib::FileRead_getc(*fileHandle);
        if (FileRead_isBr(fileHandle, chbuf)) return true;
        else if (chbuf != L' ' && chbuf != L'\t') {
            DxLib::FileRead_seek(*fileHandle, -1, SEEK_CUR);
            break;
        }
    }
    return false;
}
bool ScoreManager::ReadTag(std::tstring &str, const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR tagBuffer = toupper(DxLib::FileRead_getc(*fileHandle));
        if (tagBuffer == L'\0' || tagBuffer == L' ' || tagBuffer == L'\t') {
            break;
        }
        else if (FileRead_isBr(fileHandle, tagBuffer)) {
            return true;
        }
        str += tagBuffer;
    }
    return false;
}
bool ScoreManager::ReadValue(std::tstring &str, const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR chbuf = DxLib::FileRead_getc(*fileHandle);
        if (FileRead_isBr(fileHandle, chbuf)) break;
        else str += chbuf;
    }
    return false;
}
int  ScoreManager::TagValueToI(const std::tstring& str, bool allowMinus) {
    int ret = 0;
    try {
        ret = std::stoi(str);
        if (!allowMinus && ret < 0) {
            ret = 0;
        }
    }
    catch (const std::invalid_argument) {
        ret = 0;
    }
    catch (const std::out_of_range) {
        ret = 0;
    }
    return ret;
}
void ScoreManager::GetTagValue(const int* fileHandle, std::tstring &tagName, std::tstring &tagValue, int index) {
    if (tagName == L"TITLE") {
        m_scoreFiles[index].title.clear();
        m_scoreFiles[index].title = tagValue;
    }
    else if (tagName == L"ARTIST") {
        m_scoreFiles[index].artist.clear();
        m_scoreFiles[index].artist = tagValue;
    }
    else if (tagName == L"MUSIC") {
        m_scoreFiles[index].musicPath.clear();
        m_scoreFiles[index].musicPath = tagValue;
    }
    else if (tagName == L"IMAGE") {
        m_scoreFiles[index].imagePath.clear();
        m_scoreFiles[index].imagePath = tagValue;
    }
    else if (tagName == L"URL") {
        m_scoreFiles[index].url.clear();
        m_scoreFiles[index].url = tagValue;
    }
    else if (tagName == L"DEMOSTART") {
        m_scoreFiles[index].musicPrevPos = TagValueToI(tagValue);
    }
    else if (tagName == L"OFFSET") {
        m_scoreFiles[index].offset = TagValueToI(tagValue, true);
    }
    else if (tagName == L"MUSICVOL") {
        int buffer = TagValueToI(tagValue);
        m_scoreFiles[index].songVol = buffer > 100 ? 100 : buffer;
    }
    else if (tagName == L"SEVOL") {
        int buffer = TagValueToI(tagValue);
        m_scoreFiles[index].SEVol = buffer > 100 ? 100 : buffer;
    }
    else if (tagName == L"BPM") {
        m_scoreFiles[index].BPM = TagValueToI(tagValue);
    }
    else if (tagName == L"EASY") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EASY)] = TagValueToI(tagValue);
    }
    else if (tagName == L"NORMAL") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::NORMAL)] = TagValueToI(tagValue);
    }
    else if (tagName == L"HARD") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::HARD)] = TagValueToI(tagValue);
    }
    else if (tagName == L"EXPERT") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EXPERT)] = TagValueToI(tagValue);
    }
    else if (tagName == L"LEVEL") {
        std::tstring numBuf;
        int lvIndex = 0;
        for (std::size_t i = 0; (i <= tagValue.length() && lvIndex < static_cast<std::size_t>(ScoreInfo_t::Levels::LEVEL_NUM)) && DxLib::FileRead_eof(*fileHandle) == NULL; i++) {
            if (tagValue[i] >= L'0' && tagValue[i] <= L'9') {
                numBuf += tagValue[i];
            }
            else if (tagValue[i] == L' ' || tagValue[i] == L'\t') {
                continue;
            }
            else {
                m_scoreFiles[index].level[lvIndex] = TagValueToI(numBuf);
                numBuf.clear();
                lvIndex++;
            }
        }
    }
    else if (tagName == L"BGNUM") {
        m_scoreFiles[index].bgNum = TagValueToI(tagValue);
    }
    else if (tagName == L"BGCOLOR") {

    }
}


bool ScoreManager::LoadScoreInfo() {
    if (m_isError)         return true;
    if (!m_hasInitialized) return true;
    if (m_hasLoadedScore)  return false;
    if (m_fileNum == 0)    return false;

    if (SetScorePath()) {
        Finalize();
        return true;
    }

    for (int i = 0; i < m_fileNum; i++) {
        int fileHandle = DxLib::FileRead_open(m_scoreFiles[i].scorePath.c_str());
        if (fileHandle == NULL) {
            m_scoreFiles[i].isError = true;
            continue;
        }
        bool isComment = false;
        while (DxLib::FileRead_eof(fileHandle) == NULL) {
            TCHAR charBuffer = DxLib::FileRead_getc(fileHandle);

            // <comment>
            if (charBuffer == L'/') {
                if (isComment) {          // '/' Ç™ìÒÇ¬ë±Ç≠Ç∆Ç´
                    while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL) {
                        isComment = false;
                        continue;
                    }
                }
                else isComment = true;   // '/' Ç™àÍâÒñ⁄Ç…èoÇƒÇ´ÇΩÇ∆Ç´
                continue;
            }
            else if (charBuffer == L'*') {
                if (isComment) {         // "/*" Ç™èoÇƒÇ´ÇΩ
                    bool isEndOfComment = false;
                    while (DxLib::FileRead_eof(fileHandle) == NULL) {
                        charBuffer = DxLib::FileRead_getc(fileHandle);
                        if (charBuffer == L'*') isEndOfComment = true;
                        else if (isEndOfComment && charBuffer == L'/') {
                            isComment = false;
                            break;
                        }
                        else isEndOfComment = false;
                    }
                    continue;
                }
            }
            else isComment = false;
            // </comment>

            if (charBuffer == L'{') {
                m_scoreFiles[i].noteStartSeek = DxLib::FileRead_tell(fileHandle);
                break;
            }
            if (charBuffer == L' ' || charBuffer == L'\t') continue;
            if (charBuffer != L'#') {
                while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL);
                continue;
            }

            std::tstring tagName;

            if (ReadTag(tagName, &fileHandle)) continue;
            if (SkipSpace(&fileHandle)) continue;

            std::tstring valueBuffer;

            ReadValue(valueBuffer, &fileHandle);

            

        }


        DxLib::FileRead_close(fileHandle);
    }


    return false;
}