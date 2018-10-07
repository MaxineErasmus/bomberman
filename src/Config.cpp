#include "Config.hpp"
#include "GameWorld.hpp"
#include <iostream>
#include <fstream>
#include <string>


Config::Config()
{

}

Config::~Config(void){
    return;
}

Config::Config(const Config &src){
    static_cast<void>(src);
    return;
}

Config &Config::operator=(const Config &rhs){
    static_cast<void>(rhs);
    return *this;
}

Config* Config::getInstance()
{
    if (instance == 0)
    {
        instance = new Config();
        instance->KEY_UP = GLFW_KEY_UP;
        instance->KEY_DOWN = GLFW_KEY_DOWN;
        instance->KEY_LEFT = GLFW_KEY_LEFT;
        instance->KEY_RIGHT = GLFW_KEY_RIGHT;
        instance->KEY_DROPBOMB = GLFW_KEY_SPACE;
        instance->fullscreen = false;
        instance->res = 0;
        instance->volume = 1;
        instance->sound = 1;
    }

    return instance;
}

void Config::loadConfig(std::string path)
{
    mPath = path;
    std::string line;
    std::ifstream myfile (path);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            if (line.find("KEY_UP") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->KEY_UP = std::stoi(line.substr (++pos));
            }
            else if (line.find("KEY_DOWN") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->KEY_DOWN = std::stoi(line.substr (++pos));
            }
            else if (line.find("KEY_LEFT") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->KEY_LEFT = std::stoi(line.substr (++pos));
            }
            else if (line.find("KEY_RIGHT") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->KEY_RIGHT = std::stoi(line.substr (++pos));
            }
            else if (line.find("DROP_BOMB") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->KEY_DROPBOMB = std::stoi(line.substr (++pos));
            }
            else if (line.find("FULLSCREEN") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->fullscreen = std::stoi(line.substr (++pos));               
            }
            else if (line.find("SOUND") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->sound = std::stoi(line.substr (++pos));               
            }
            else if (line.find("VOLUME") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->volume = std::stoi(line.substr (++pos));               
            }
            else if (line.find("RES") != std::string::npos){
                std::size_t pos = line.find(":");      
                instance->res = std::stoi(line.substr (++pos));    
                
                if (instance->res == 0){
                    res_x = 640;
                    res_y = 480;
                } else if (instance->res == 1){
                    res_x = 800;
                    res_y = 600;
                }
                else if (instance->res == 2){
                    res_x = 1366;
                    res_y = 768;
                }           
            }
            
        }
        myfile.close();
    }

  else std::cout << "Unable to open config file" << std::endl;
}

void Config::saveConfig()
{
  std::ofstream myfile (mPath);
  if (myfile.is_open())
  {
    myfile << "KEY_UP:" << instance->KEY_UP << std::endl;
    myfile << "KEY_DOWN:" << instance->KEY_DOWN << std::endl;
    myfile << "KEY_LEFT:" << instance->KEY_LEFT << std::endl;
    myfile << "KEY_RIGHT:" << instance->KEY_RIGHT << std::endl;
    myfile << "DROP_BOMB:" << instance->KEY_DROPBOMB << std::endl;
    myfile << "FULLSCREEN:" << instance->fullscreen << std::endl;
    myfile << "RES:" << instance->res << std::endl;
    myfile << "SOUND:" << instance->sound << std::endl;
    myfile << "VOLUME:" << instance->volume << std::endl;
    myfile.close();
  }
  else std::cout << "Unable to open file";
}

Config* Config::instance = 0;