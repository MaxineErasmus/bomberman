#pragma once
#include "GameWorld.hpp"
#include "Font.hpp"
#include <list>

class Setting
{
protected:
    float mX;
    float mY;
    bool mSelected;
    std::string mText;
    std::string mValue;
public: 
    Setting *nextSetting;
    Setting *prevSetting;
    Setting(void);
    Setting(float x, float y, bool selected, std::string text);
    ~Setting(void);
    Setting(const Setting &src);
    Setting &operator=(const Setting &rhs);

    void setValue(std::string value);
    std::string getValue();

    virtual std::string getLiteral();

    float getX();
    float getY();

    void select();
    void deselect();
    bool isSelected();
    std::string getText();

};

class VolumeSetting : public Setting 
{
protected:
public:
    VolumeSetting(float x, float y, bool selected, std::string text);
    ~VolumeSetting(void);
    VolumeSetting(const VolumeSetting &src);
    VolumeSetting &operator=(const VolumeSetting &rhs);

    std::string getLiteral();
    void nextVolume();
};

class ResolutionSetting : public Setting 
{
protected: 
    int mResX[3];
    int mResY[3];
    int mCurrentResolution;
public:
    ResolutionSetting(float x, float y, bool selected, std::string text);
    ~ResolutionSetting(void);
    ResolutionSetting(const ResolutionSetting &src);
    ResolutionSetting &operator=(const ResolutionSetting &rhs);

    void changeResolution(int res);
    void nextResolution();

    int getResX();
    int getResY();
    int getCurrentResolution();

    std::string getLiteral();
};

class Menu : public GameWorld
{
protected: 
    std::list<Setting*> mSettings;
    Setting *mSelectedSetting;

    GameBoi::graphics::Fonts *m_font;
public:
    Menu(void);
    Menu(GameBoi::graphics::Window *window, ResourceManager *resources);
    ~Menu();
    Menu(const Menu &src);
    Menu &operator=(const Menu &rhs);

    virtual void init(std::string config);
    GAME_STATE runLoop();
    virtual GAME_STATE handleInput();

    void drawMenu();
};