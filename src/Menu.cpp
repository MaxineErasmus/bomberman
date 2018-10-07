#include "Menu.hpp"
#include <unistd.h>

Menu::Menu(void){
	return;
}

void Menu::init(std::string config)
{
    //loadResources();
    config = "";
	std::cout << "MENU INIT START" << std::endl;
    // Create and load all the settings
    m_font = new GameBoi::graphics::Fonts("../Resources/Fonts/press.ttf");

	// ============================== CREATE THE SETTINGS =========================================== //
    Setting *new_game = new Setting(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, true, "START NEW GAME");
    mSettings.push_back(new_game);

    Setting *resume_game = new Setting(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 100, false, "RESUME GAME" + config);
    mSettings.push_back(resume_game);

	Setting *settings = new Setting(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 200, false, "SETTINGS");
	mSettings.push_back(settings);

	Setting *quit_game = new Setting(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 300, false, "QUIT GAME");
	mSettings.push_back(quit_game);

	//=============================== LINK THE SETTINGS ============================================== //
	mSelectedSetting = new_game;

	new_game->nextSetting = resume_game;
	new_game->prevSetting = quit_game;
	resume_game->nextSetting = settings;
	resume_game->prevSetting = new_game;
	settings->prevSetting = resume_game;
	settings->nextSetting = quit_game;
	quit_game->nextSetting = new_game;
	quit_game->prevSetting = settings;
	std::cout << "MENU INIT END" << std::endl;
}

Menu::Menu(GameBoi::graphics::Window *window, ResourceManager *mResources)
:GameWorld(window, mResources)
{
	m_Timer = GameBoi::Timer::Instance();
	// setup the builder
	//mEntityBuilder = new Builder();
	//mEntityBuilder->create(this, &mResourceManager);
	// setup the camera
	//m_camera = new Camera(glm::vec3(4.0f, 6.0f, 0.0f));
	//m_camera->LookAt(glm::vec3(4.0f, 1.0f, 4.0f));
	// setup the window
	//m_window = new GameBoi::graphics::Window("GameBoi!", SCREEN_WIDTH, SCREEN_HEIGHT);;
	m_Timer->resetElapsed();
}


Menu::~Menu(void)
{
	return;
}

Menu::Menu(const Menu &src)
{
	static_cast<void>(src);
	return;
}

Menu &Menu::operator=(const Menu &rhs)
{
	static_cast<void>(rhs);
	return *this;
}

GAME_STATE Menu::runLoop()
{
    m_Timer->Reset();

    
    m_Timer->update();
	GameBoi::graphics::Fonts font("../Resources/Fonts/press.ttf");

    while (!m_window->closed())
    {
		m_Timer->update();
		m_window->clear();

        // DRAW SHIT HERE
		GAME_STATE state = handleInput();
		if (state != GAME_STATE::STATE_NONE){
			std::cout << "GAME STATE = " << state << std::endl;
			std::cout << "BAILING" << std::endl;
			return state;
		}

        drawMenu();
        m_window->update();

    }

	return GAME_STATE::STATE_EXIT;
}

GAME_STATE Menu::handleInput()
{
	if (m_Timer->getElapsed() >= 0.25f)
	{
		if (m_window->isKeyPressed(GLFW_KEY_S)) 
		{
			mSelectedSetting = mSelectedSetting->nextSetting;
			m_Timer->resetElapsed();
		}
		else if (m_window->isKeyPressed(GLFW_KEY_W)) {
			mSelectedSetting = mSelectedSetting->prevSetting;
			m_Timer->resetElapsed();		
		}
		else if (m_window->isKeyPressed(GLFW_KEY_ENTER)){
			if (mSelectedSetting->getText() == "QUIT GAME"){
				return GAME_STATE::STATE_EXIT;
			}
			else if (mSelectedSetting->getText() == "RESUME GAME"){
				std::cout << "RESUMING GAME" << std::endl;
				return GAME_STATE::STATE_RESUME;
			}
			else if (mSelectedSetting->getText() == "START NEW GAME"){
				std::cout << "STARTING GAME" << std::endl;
				return GAME_STATE::STATE_START;
			}
			else if (mSelectedSetting->getText() == "SETTINGS"){
				return GAME_STATE::STATE_SETTINGS;
			}

		}
		else if (m_window->isKeyPressed(GLFW_KEY_H)){
			exit(0);
		}
	}
	return GAME_STATE::STATE_NONE;
}

void Menu::drawMenu()
{
    for (auto setting : mSettings)
    {
		mResourceManager->getShader("shader_text")->use();
        m_font->RenderText(*mResourceManager->getShader("shader_text"),"*", mSelectedSetting->getX() - 100, mSelectedSetting->getY(), 0.5f, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
        m_font->RenderText(*mResourceManager->getShader("shader_text"),setting->getText(), setting->getX(), setting->getY(), 0.5f, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    //font.RenderText(*mResourceManager.getShader("shader_text"),"*", SCREEN_WIDTH/2 - 180, SCREEN_HEIGHT/2+200, 0.5f, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
    //font.RenderText(*mResourceManager.getShader("shader_text"),"START NEW GAME", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2+200, 0.5, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
    //font.RenderText(*mResourceManager.getShader("shader_text"),"RESUME GAME", SCREEN_WIDTH/2 -100, SCREEN_HEIGHT/2+100, 0.5, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
    //font.RenderText(*mResourceManager.getShader("shader_text"),"SETTINGS", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 0.5, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
    //font.RenderText(*mResourceManager.getShader("shader_text"),"QUIT", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2-100, 0.5, glm::vec3(1, 1, 0), SCREEN_WIDTH, SCREEN_HEIGHT);
}


// ====================================================== RESOLUTION SETTINGS ===================================================== //

ResolutionSetting::ResolutionSetting(float x, float y, bool selected, std::string text)
:Setting(x,y,selected,text)
{
	mCurrentResolution = 0;

	mResX[0] = 640;
	mResY[0] = 480;

	mResX[1] = 800;
	mResY[1] = 600;

	mResX[2] = 1366;
	mResY[2] = 768;
}

ResolutionSetting::~ResolutionSetting(void){
	return;
}

ResolutionSetting::ResolutionSetting(const ResolutionSetting &src){
	static_cast<void>(src);
	return;
}

ResolutionSetting &ResolutionSetting::operator=(const ResolutionSetting &rhs){
	static_cast<void>(rhs);
	return *this;
}

std::string ResolutionSetting::getLiteral()
{
	return std::to_string(mResX[mCurrentResolution]) + " x " + std::to_string(mResY[mCurrentResolution]);
}

int ResolutionSetting::getCurrentResolution()
{
	return mCurrentResolution;
}

void ResolutionSetting::changeResolution(int res)
{
	mCurrentResolution = res;
}

int ResolutionSetting::getResX()
{
	return mResX[mCurrentResolution];
}

int ResolutionSetting::getResY()
{
	return mResY[mCurrentResolution];
}

void ResolutionSetting::nextResolution()
{
	mCurrentResolution++;
	if (mCurrentResolution == 3){
		mCurrentResolution = 0;
	}
}

// ====================================================== VOLUME SETTINGS ===================================================== //

VolumeSetting::VolumeSetting(float x, float y, bool selected, std::string text)
:Setting(x,y,selected,text)
{

}


VolumeSetting::~VolumeSetting(void){
	return;
}

VolumeSetting::VolumeSetting(const VolumeSetting &src){
	static_cast<void>(src);
	return;
}

VolumeSetting &VolumeSetting::operator=(const VolumeSetting &rhs){
	static_cast<void>(rhs);
	return *this;
}

std::string VolumeSetting::getLiteral()
{
	return std::to_string(std::stoi(mValue) * 10) + "%";
}

void VolumeSetting::nextVolume()
{
	int volume = std::stoi(mValue);
	volume += 1;

	if (volume > 10){
		volume = 0;
	}

	mValue = std::to_string(volume);
}

// ====================================================== DEFAULT SETTINGS ===================================================== //

Setting::Setting(void){
	return;
}


Setting::Setting(float x, float y, bool selected, std::string text)
:mX(x), mY(y), mSelected(selected), mText(text), mValue("")
{

}

Setting::~Setting(void){
	return;
}

Setting::Setting(const Setting &src){
	static_cast<void>(src);
	return;
}

Setting &Setting::operator=(const Setting &rhs){
	static_cast<void>(rhs);
	return *this;
}

std::string Setting::getLiteral()
{
	std::string value = "";

	if (mValue == "0"){
		value = "OFF";
	}
	else if (mValue == "1"){
		value = "ON";
	}
	else if (mValue == "9" || mValue == "265"){
		value =  "UP ARROW";
	}
	else if (mValue == "8" || mValue == "264"){
		value =  "DOWN ARROW";
	}
	else if (mValue == "6" || mValue == "263"){
		value = "LEFT ARROW";
	}
	else if (mValue == "7" || mValue == "262"){
		value = "RIGHT ARROW";
	}
	else if (mValue == "32"){
		value = "SPACEBAR";
	}
	else if (mValue == "NA"){
		value = "NA";
	}
	else if (mValue == "PRESS KEY"){
		value = "PRESS KEY";
	}
	else if (mValue != "")
	{
		value = (char(std::stoi(mValue)));
	}
	return value;
}

float Setting::getX()
{
    return mX;
}
float Setting::getY()
{
    return mY;
}

void Setting::select()
{
    mSelected = true;
}

void Setting::deselect()
{
    mSelected = false;
}

bool Setting::isSelected()
{
    return mSelected;
}

void Setting::setValue(std::string value)
{
	mValue = value;
}

std::string Setting::getValue()
{
	if (mValue == "9"){
		mValue =  "265";
	}
	else if (mValue == "8"){
		mValue =  "264";
	}
	else if (mValue == "6"){
		mValue = "263";
	}
	else if (mValue == "7"){
		mValue = "262";
	}
	return mValue;
}

std::string Setting::getText()
{
    return mText + getLiteral();
}