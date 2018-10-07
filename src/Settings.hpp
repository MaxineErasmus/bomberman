#pragma once
#include "Menu.hpp"


class Settings : public Menu 
{
private: 
public:
    Settings(GameBoi::graphics::Window *window, ResourceManager *resources);
    ~Settings(void);
    Settings(const Settings &src);
    Settings &operator=(const Settings &rhs);

    void init(std::string config);
    GAME_STATE handleInput();
};