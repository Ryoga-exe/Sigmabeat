#pragma once
#include "Singleton.h"
#include <string>

struct ScoreInfo_t{
	enum class Levels {
		EASY,
		NORMAL,
		HARD,
		EXPERT,
		LEVEL_NUM
	};

	bool isError = false;
	std::string Title;
	std::string Artist;
	std::string ScorePath;
	std::string MusicPath;
	std::string ImagePath = "../Data/Image/sigmabeat.png";
	std::string URL;

	
};