#pragma once
#include <string>

class Config 
{
private: 
    static Config *instance;
    std::string mPath;

public:
    Config();
    ~Config(void);
    Config(const Config &src);
    Config &operator=(const Config &rhs);


    static Config* getInstance();

    unsigned int KEY_UP;
    unsigned int KEY_DOWN;
    unsigned int KEY_LEFT;
    unsigned int KEY_RIGHT;
    unsigned int KEY_DROPBOMB;

    bool fullscreen;
    bool sound;
    int volume;
    int res;
    int res_x;
    int res_y;


    void loadConfig(std::string path);
    void saveConfig();
};
