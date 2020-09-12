#pragma once
#include "../DxSystem.h"
#include "../Singleton.h"
#include <string>
#include <vector>
#include <tuple>

/*
namespace std {
    string to_string(string _Val) {
        return _Val;
    }
};
*/

class Language : public Singleton<Language> {
    Language();
    ~Language();
    friend Singleton<Language>;
public:
    bool Initialize();
    bool Finalize();

    bool ChangeLocale(std::string locate);
    std::string GetString(std::string key, ... /* std:string value (ex. "value = 4, num = 5" ) */);

private:
    bool LoadFile();

    std::string              m_locate;
    std::vector<std::string> m_strings;
    
    std::string m_filePath;
    std::string m_fileExtension;
};