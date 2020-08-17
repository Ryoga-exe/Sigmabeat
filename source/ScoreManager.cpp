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
            m_scoreFiles[i].scorePath = u8"Scores/";
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
            ErrMsgBx("Error!!\nMemory Allocation Failed.");
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
        else if (chbuf != u8' ' && chbuf != u8'\t') {
            DxLib::FileRead_seek(*fileHandle, -1, SEEK_CUR);
            break;
        }
    }
    return false;
}
bool ScoreManager::ReadTag(std::string &str, const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR tagBuffer = toupper(DxLib::FileRead_getc(*fileHandle));
        if (tagBuffer == u8'\0' || tagBuffer == u8' ' || tagBuffer == u8'\t') {
            break;
        }
        else if (FileRead_isBr(fileHandle, tagBuffer)) {
            return true;
        }
        str += tagBuffer;
    }
    return false;
}
bool ScoreManager::ReadValue(std::string &str, const int* fileHandle) {
    if (fileHandle == nullptr || *fileHandle == NULL) return true;
    while (DxLib::FileRead_eof(*fileHandle) == NULL) {
        TCHAR chbuf = DxLib::FileRead_getc(*fileHandle);
        if (FileRead_isBr(fileHandle, chbuf)) break;
        else str += chbuf;
    }
    return false;
}
int  ScoreManager::TagValueToI(const std::string& str, bool allowMinus) {
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
void ScoreManager::GetTagValue(const int* fileHandle, std::string &tagName, std::string &tagValue, int index) {
    if (tagName == u8"TITLE") {
        m_scoreFiles[index].title.clear();
        m_scoreFiles[index].title = tagValue;
    }
    else if (tagName == u8"ARTIST") {
        m_scoreFiles[index].artist.clear();
        m_scoreFiles[index].artist = tagValue;
    }
    else if (tagName == u8"MUSIC") {
        m_scoreFiles[index].musicPath.clear();
        m_scoreFiles[index].musicPath = tagValue;
    }
    else if (tagName == u8"IMAGE") {
        m_scoreFiles[index].imagePath.clear();
        m_scoreFiles[index].imagePath = tagValue;
    }
    else if (tagName == u8"URL") {
        m_scoreFiles[index].url.clear();
        m_scoreFiles[index].url = tagValue;
    }
    else if (tagName == u8"DEMOSTART") {
        m_scoreFiles[index].musicPrevPos = TagValueToI(tagValue);
    }
    else if (tagName == u8"OFFSET") {
        m_scoreFiles[index].offset = TagValueToI(tagValue, true);
    }
    else if (tagName == u8"MUSICVOL") {
        int buffer = TagValueToI(tagValue);
        m_scoreFiles[index].songVol = buffer > 100 ? 100 : buffer;
    }
    else if (tagName == u8"SEVOL") {
        int buffer = TagValueToI(tagValue);
        m_scoreFiles[index].SEVol = buffer > 100 ? 100 : buffer;
    }
    else if (tagName == u8"BPM") {
        m_scoreFiles[index].BPM = TagValueToI(tagValue);
    }
    else if (tagName == u8"EASY") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EASY)] = TagValueToI(tagValue);
    }
    else if (tagName == u8"NORMAL") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::NORMAL)] = TagValueToI(tagValue);
    }
    else if (tagName == u8"HARD") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::HARD)] = TagValueToI(tagValue);
    }
    else if (tagName == u8"EXPERT") {
        m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EXPERT)] = TagValueToI(tagValue);
    }
    else if (tagName == u8"LEVEL") {
        std::string numBuf;
        int lvIndex = 0;
        for (std::size_t i = 0; (i <= tagValue.length() && lvIndex < static_cast<std::size_t>(ScoreInfo_t::Levels::LEVEL_NUM)) && DxLib::FileRead_eof(*fileHandle) == NULL; i++) {
            if (tagValue[i] >= u8'0' && tagValue[i] <= u8'9') {
                numBuf += tagValue[i];
            }
            else if (tagValue[i] == u8' ' || tagValue[i] == u8'\t') {
                continue;
            }
            else {
                m_scoreFiles[index].level[lvIndex] = TagValueToI(numBuf);
                numBuf.clear();
                lvIndex++;
            }
        }
    }
    else if (tagName == u8"BGNUM") {
        m_scoreFiles[index].bgNum = TagValueToI(tagValue);
    }
    else if (tagName == u8"BGCOLOR") {
        std::string strBuf;
        int clNum = 0;
        unsigned int arrBuf[4] = { 0xffffffa, 0xffffffa, 0xffffffa, 0xffffffa };
        for (size_t i = 0; (i <= tagValue.length() && clNum < 4) && DxLib::FileRead_eof(*fileHandle) == NULL; i++) {
            if ((tagValue[i] >= u8'0' && tagValue[i] <= u8'9') || (tagValue[i] >= u8'a' && tagValue[i] <= u8'f')) {
                strBuf += tagValue[i];
            }
            else if (tagValue[i] == u8' ' || tagValue[i] == u8'\t' || tagValue[i] == u8'#') {
                continue;
            }
            else {
                try {
                    arrBuf[clNum] = std::stoi(strBuf, nullptr, 16);
                }
                catch (const std::invalid_argument) {
                    arrBuf[clNum] = 0xffffffa;
                }
                catch (const std::out_of_range) {
                    arrBuf[clNum] = 0xffffffa;
                }
                if (arrBuf[clNum] < 0 || arrBuf[clNum] > 0xffffffa) arrBuf[clNum] = 0xffffffa;
                strBuf.clear();
                clNum++;
            }
        }
        unsigned int colBuf[4] = { 0xffffffa, 0xffffffa, 0xffffffa, 0xffffffa };
        std::size_t j;
        for (std::size_t i = j = 0; i < 4; i++) {
            if (arrBuf[i] != 0xffffffa) {
                colBuf[j] = arrBuf[i];
                j++;
            }
        }
        switch (j) {
        case 0:
            for (std::size_t i = 0; i < 4; i++) m_scoreFiles[index].bgColor[i] = DxLib::GetRand(0xffffff);
            break;
        case 1:
            for (std::size_t i = 0; i < 4; i++) m_scoreFiles[index].bgColor[i] = colBuf[0];
            break;
        case 2:
            m_scoreFiles[index].bgColor[0] = m_scoreFiles[index].bgColor[1] = colBuf[0];
            m_scoreFiles[index].bgColor[2] = m_scoreFiles[index].bgColor[3] = colBuf[1];
            break;
        case 3:
            m_scoreFiles[index].bgColor[0] = m_scoreFiles[index].bgColor[1] = colBuf[0];
            m_scoreFiles[index].bgColor[2] = colBuf[1];
            m_scoreFiles[index].bgColor[3] = colBuf[2];
            break;
        case 4:
            for (size_t i = 0; i < 4; i++) m_scoreFiles[index].bgColor[i] = colBuf[i];
            break;
        }
    }
    else {
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
            if (charBuffer == u8'/') {
                if (isComment) {          // '/' Ç™ìÒÇ¬ë±Ç≠Ç∆Ç´
                    while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL) {
                        isComment = false;
                        continue;
                    }
                }
                else isComment = true;   // '/' Ç™àÍâÒñ⁄Ç…èoÇƒÇ´ÇΩÇ∆Ç´
                continue;
            }
            else if (charBuffer == u8'*') {
                if (isComment) {         // "/*" Ç™èoÇƒÇ´ÇΩ
                    bool isEndOfComment = false;
                    while (DxLib::FileRead_eof(fileHandle) == NULL) {
                        charBuffer = DxLib::FileRead_getc(fileHandle);
                        if (charBuffer == u8'*') isEndOfComment = true;
                        else if (isEndOfComment && charBuffer == u8'/') {
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

            if (charBuffer == u8'{') {
                m_scoreFiles[i].noteStartSeek = DxLib::FileRead_tell(fileHandle);
                break;
            }
            if (charBuffer == u8' ' || charBuffer == u8'\t') continue;
            if (charBuffer != u8'#') {
                while (FileRead_isBr(&fileHandle, DxLib::FileRead_getc(fileHandle)) && DxLib::FileRead_eof(fileHandle) == NULL);
                continue;
            }

            std::string tagName;

            if (ReadTag(tagName, &fileHandle)) continue;
            if (SkipSpace(&fileHandle)) continue;

            std::string valueBuffer;

            ReadValue(valueBuffer, &fileHandle);

            // ReplaceTagName(tagName);

            GetTagValue(&fileHandle, tagName, valueBuffer, i);

        }
        DxLib::FileRead_close(fileHandle);

        // LoadJacketImage(i);
    }
    m_hasLoadedScore = true;
    return false;
}






void ScoreManager::DrawScoreInfo(unsigned int index) {
    if (m_isError || m_fileNum == 0 || index > (unsigned)m_fileNum || m_scoreFiles == nullptr) {
        return;
    }
    DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
    DrawBox(0, 0, 700, 304, 0U, TRUE);
    DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    DrawFormatString(0, 0, 0xffffff,   u8"Title         = %s",   m_scoreFiles[index].title.c_str());
    DrawFormatString(0, 16, 0xffffff,  u8"Artist        = %s",   m_scoreFiles[index].artist.c_str());
    DrawFormatString(0, 32, 0xffffff,  u8"MusicPath     = %s",   m_scoreFiles[index].musicPath.c_str());
    DrawFormatString(0, 48, 0xffffff,  u8"ImagePath     = %s",   m_scoreFiles[index].imagePath.c_str());
    DrawFormatString(0, 64, 0xffffff,  u8"MusicPrevPos  = %d",   m_scoreFiles[index].musicPrevPos);
    DrawFormatString(0, 80, 0xffffff,  u8"NoteStartSeek = %lld", m_scoreFiles[index].noteStartSeek);
    DrawFormatString(0, 96, 0xffffff,  u8"SongVol       = %d",   m_scoreFiles[index].songVol);
    DrawFormatString(0, 112, 0xffffff, u8"SEVol         = %d",   m_scoreFiles[index].SEVol);
    DrawFormatString(0, 128, 0xffffff, u8"URL           = %s",   m_scoreFiles[index].url.c_str());
    DrawFormatString(0, 144, 0xffffff, u8"Offset        = %d",   m_scoreFiles[index].offset);
    DrawFormatString(0, 160, 0xffffff, u8"BPM           = %lf",  m_scoreFiles[index].BPM);
    DrawFormatString(0, 176, 0xffffff, u8"Level:Easy    = %d",   m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EASY)]);
    DrawFormatString(0, 192, 0xffffff, u8"Level:Normal  = %d",   m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::NORMAL)]);
    DrawFormatString(0, 208, 0xffffff, u8"Level:Hard    = %d",   m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::HARD)]);
    DrawFormatString(0, 224, 0xffffff, u8"Level:Expert  = %d",   m_scoreFiles[index].level[static_cast<std::size_t>(ScoreInfo_t::Levels::EXPERT)]);
    DrawFormatString(0, 240, 0xffffff, u8"BgColor:1     = #%x",  m_scoreFiles[index].bgColor[0].Get());
    DrawFormatString(0, 256, 0xffffff, u8"BgColor:2     = #%x",  m_scoreFiles[index].bgColor[1].Get());
    DrawFormatString(0, 272, 0xffffff, u8"BgColor:3     = #%x",  m_scoreFiles[index].bgColor[2].Get());
    DrawFormatString(0, 288, 0xffffff, u8"BgColor:4     = #%x",  m_scoreFiles[index].bgColor[3].Get());
}