#pragma once
#include "DxSystem.h"
#include <string>

#define SCORE_EXTENSION u8".sgm"
#define SCORE_DIRECTORY u8"Scores/*.sgm"

struct ScoreInfo_t{
    enum class Levels {
        EASY,
        NORMAL,
        HARD,
        EXPERT,
        LEVEL_NUM
    };

    bool isError = false;
    std::string title;
    std::string artist;
    std::string scorePath;
    std::string musicPath;
    std::string imagePath = u8"Data/Image/sigmabeat.png";
    std::string url;

    int         musicPrevPos = 0;
    int         offset = 0;
    int         songVol = 100;
    int         SEVol = 100;
    double      BPM = 100.0;
    int         level[static_cast<std::size_t>(Levels::LEVEL_NUM)] = {};
    int         bgNum = 1;
    Color       bgColor[4] = { 0x2557EB, 0x2557EB, 0x166161, 0x166161 };
    LONGLONG    noteStartSeek = 0;

    int         imageHandle = -1;

    int         sortOrder = INT_MAX;
};

class ScoreManager : public Singleton<ScoreManager> {
    ScoreManager();
    ~ScoreManager();
    friend Singleton<ScoreManager>;

public:
    bool Initialize();
    bool Finalize();

    bool LoadScoreInfo();

    double GetMusicBPM(unsigned int index);
    int  GetMusicLevel(unsigned int index, size_t level);
    bool GetScorePath(unsigned int index, std::string& replacedStr);
    bool GetMusicPath(unsigned int index, std::string& replacedStr);
    bool GetMusicTitle(unsigned int index, std::string& replacedStr);
    int  GetMusicPrevPos(unsigned int index);
    int  GetMusicVol(unsigned int index);
    int  GetSEVol(unsigned int index);
    int  GetOffset(unsigned int index);
    LONGLONG GetNoteStartSeek(unsigned int index);
    int  GetBgNum(unsigned int index);
    bool GetBgColor(unsigned int index, unsigned int& c1, unsigned int& c2, unsigned int& c3, unsigned int& c4);
    ColorRect_t GetBgColor(unsigned int index);
    bool GetIsError();
    int  GetScoreNum();
    ScoreInfo_t GetFileInfo(unsigned int index);

    void DrawJacketImage(int x, int y, unsigned int index, int transFlag = FALSE);
    void DrawExtendJacketImage(int x1, int y1, int x2, int y2, unsigned int index, int transFlag);
    void DrawExtendJacketImageF(float x1f, float y1f, float x2f, float y2f, unsigned int index, int transFlag);
    int  DrawMusicTitleToHandle(int x, int y, unsigned int color, int fontHandle, unsigned int index, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
    int  DrawMusicTitleToHandleCenter(int y, unsigned int color, int fontHandle, unsigned int index, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
    int  DrawArtistNameToHandle(int x, int y, unsigned int color, int fontHandle, unsigned int index, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
    int  DrawArtistNameToHandleCenter(int y, unsigned int color, int fontHandle, unsigned int index, unsigned int edgeColor = 0U, int verticalFlag = FALSE);

    void DrawScoreInfo(unsigned int index);

private:
    bool IsAvailableIndex(unsigned int index);
    bool SetScorePath();
    bool SkipSpace(const int* fileHandle);
    bool ReadTag(std::string &str, const int* fileHandle);
    bool ReadValue(std::string &str, const int* fileHandle);
    // void ReplaceTagName(std::string& replacedStr);
    int  TagValueToI(const std::string& str, bool allowMinus = false);
    void GetTagValue(const int* fileHandle, std::string &tagName, std::string &tagValue, int index);

    bool LoadJacketImage(int index);

private:
    bool m_hasInitialized, m_isError, m_hasLoadedScore;
    int  m_fileNum;
    int  m_defaultImageHandle;
    const TCHAR* m_defaultImagePath = u8"Data/Image/sigmabeat.png";
    ScoreInfo_t* m_scoreFiles = nullptr;
};

unsigned int GetLevelColor(unsigned int level);
TCHAR* GetGradeStr(int scorePoint);
Color  GetGradeColor(int scorePoint);