#include "ScoreManager.h"

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
    while (DxLib::FileRead_eof(*fileHandle) == 0) {
        TCHAR charBuffer = DxLib::FileRead_getc(*fileHandle);
        if (FileRead_isBr(fileHandle, charBuffer)) return true;
        else if (charBuffer != ' ' && charBuffer != '\t') {
            FileRead_seek(*fileHandle, -1, SEEK_CUR);
            break;
        }
    }
    return false;
}
bool ScoreManager::ReadTag(std::tstring &str, const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR tagBuffer = toupper(DxLib::FileRead_getc(*fileHandle));
        if (tagBuffer == '\0' || tagBuffer == ' ' || tagBuffer == '\t') {
            break;
        }
        else if (FileRead_isBr(fileHandle, tagBuffer)) {
            return true;
        }
        str += tagBuffer;
    }
    return false;
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
            if (charBuffer == '/') {
                if (isComment) {          // '/' Ç™ìÒÇ¬ë±Ç≠Ç∆Ç´
                    while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL) {
                        isComment = false;
                        continue;
                    }
                }
                else isComment = true;   // '/' Ç™àÍâÒñ⁄Ç…èoÇƒÇ´ÇΩÇ∆Ç´
                continue;
            }
            else if (charBuffer == '*') {
                if (isComment) {         // "/*" Ç™èoÇƒÇ´ÇΩ
                    bool isEndOfComment = false;
                    while (DxLib::FileRead_eof(fileHandle) == NULL) {
                        charBuffer = DxLib::FileRead_getc(fileHandle);
                        if (charBuffer == '*') isEndOfComment = true;
                        else if (isEndOfComment && charBuffer == '/') {
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

            if (charBuffer == '{') {
                m_scoreFiles[i].noteStartSeek = DxLib::FileRead_tell(fileHandle);
                break;
            }
            if (charBuffer == ' ' || charBuffer == '\t') continue;
            if (charBuffer != '#') {
                while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL);
                continue;
            }

            std::tstring tagName;

            if (ReadTag(tagName, &fileHandle)) {
                continue;
            }



        }


        DxLib::FileRead_close(fileHandle);
    }


    return false;
}