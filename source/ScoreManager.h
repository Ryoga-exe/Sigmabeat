#pragma once
#include "DxSystem.h"
#include <string>

#define SCORE_EXTENSION L".sgm"
#define SCORE_DIRECTORY L"Scores/*.sgm"

struct ScoreInfo_t{
	enum class Levels {
		EASY,
		NORMAL,
		HARD,
		EXPERT,
		LEVEL_NUM
	};

	bool isError = false;
	std::tstring title;
	std::tstring artist;
	std::tstring scorePath;
	std::tstring musicPath;
	std::tstring imagePath = L"Data/Image/sigmabeat.png";
	std::tstring url;


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
};

class ScoreManager : public Singleton<ScoreManager> {
	ScoreManager();
	~ScoreManager();
	friend Singleton<ScoreManager>;

public:
	bool Initialize();
	bool Finalize();

	bool LoadScoreInfo();

	double GetMusicBPM(unsigned int number);
	int  GetMusicLevel(unsigned int number, size_t level);
	bool GetScorePath(unsigned int number, std::string& replacedStr);
	bool GetMusicPath(unsigned int number, std::string& replacedStr);
	bool GetMusicTitle(unsigned int number, std::string& replacedStr);
	int  GetMusicPrevPos(unsigned int number);
	int  GetMusicVol(unsigned int number);
	int  GetSEVol(unsigned int number);
	int  GetOffset(unsigned int number);
	LONGLONG GetNoteStartSeek(unsigned int number);
	int  GetBgNum(unsigned int number);
	bool GetBgColor(unsigned int number, unsigned int& c1, unsigned int& c2, unsigned int& c3, unsigned int& c4);
	ColorRect_t GetBgColor(unsigned int number);
	bool GetIsError();
	int  GetScoreNum();
	ScoreInfo_t GetFileInfo(unsigned int number);

	void DrawJacketImage(int x, int y, unsigned int number, int transFlag);
	void DrawExtendJacketImage(int x1, int y1, int x2, int y2, unsigned int number, int transFlag);
	void DrawExtendJacketImageF(float x1f, float y1f, float x2f, float y2f, unsigned int number, int transFlag);
	int  DrawMusicTitleToHandle(int x, int y, unsigned int color, int fontHandle, unsigned int number, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
	int  DrawMusicTitleToHandleCenter(int y, unsigned int color, int fontHandle, unsigned int number, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
	int  DrawArtistNameToHandle(int x, int y, unsigned int color, int fontHandle, unsigned int number, unsigned int edgeColor = 0U, int verticalFlag = FALSE);
	int  DrawArtistNameToHandleCenter(int y, unsigned int color, int fontHandle, unsigned int number, unsigned int edgeColor = 0U, int verticalFlag = FALSE);

	void DrawScoreInfo(unsigned int number);

private:
	bool SetScorePath();
	bool SkipSpace(const int* fileHandle);
	bool ReadTag(std::tstring &str, const int* fileHandle);
	bool ReadValue(std::tstring &str, const int* fileHandle);
	void ReplaceTagName(std::string& replacedStr);
	int  TagValueToI(const std::tstring& str, bool allowMinus = false);
	void GetTagValue(const int* fileHandle, std::tstring &tagName, std::tstring &tagValue, int index);

	bool LoadJacketImage(int currentNum);

private:
	bool m_hasInitialized, m_isError, m_hasLoadedScore;
	int  m_fileNum;
	int  m_defaultImageHandle;
	const TCHAR* m_defaultImagePath = L"Data/Image/sigmabeat.png";
	ScoreInfo_t* m_scoreFiles = nullptr;
};

unsigned int GetLevelColor(unsigned int level);
TCHAR* GetGradeStr(int scorePoint);
Color GetGradeColor(int scorePoint);