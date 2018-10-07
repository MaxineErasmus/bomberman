#include "Sound.hpp"
#include "Config.hpp"


SoundManager::SoundManager(){
	auto config = Config::getInstance();
	mSoundEngine = irrklang::createIrrKlangDevice();

	std::cout << mSoundEngine->getDriverName() << std::endl;

	mSoundEngine->setSoundVolume(config->volume / 10.0f);

	if (!config->sound)
		mSoundEngine->setSoundVolume(0.0f);

	level1 = mSoundEngine->addSoundSourceFromFile("../Resources/audio/Arcade-Fantasy.wav");
	level2 = mSoundEngine->addSoundSourceFromFile("../Resources/audio/Desert.wav");
	level3 = mSoundEngine->addSoundSourceFromFile("../Resources/audio/Spooky-Island.wav");
	place_bomb = mSoundEngine->addSoundSourceFromFile("../Resources/audio/place.wav");
	player_footsteps = mSoundEngine->addSoundSourceFromFile("../Resources/audio/Player_Footsteps.wav");
	player_death = mSoundEngine->addSoundSourceFromFile("../Resources/audio/player_death.wav");
	explosion = mSoundEngine->addSoundSourceFromFile("../Resources/audio/explosion.wav");  
	enemy_death = mSoundEngine->addSoundSourceFromFile("../Resources/audio/enemy_death.wav");
	NoNo = mSoundEngine->addSoundSourceFromFile("../Resources/audio/NoNo.wav");  

	return;
};

SoundManager::~SoundManager(){
	mSoundEngine->drop();
	return;
};

SoundManager::SoundManager(const SoundManager & _soundmanager){
	static_cast<void>(_soundmanager);
	return;
}

SoundManager & SoundManager::operator=(const SoundManager & _rhs){
	static_cast<void>(_rhs);
	return *this;
}


void SoundManager::play(E_SOUNDS sound, bool loop){
	// std::cout << "play sound: " << sound << std::endl;

	
	switch(sound){
		case E_SOUNDS_LEVEL_1:
			mSoundEngine->play2D(level1,loop);
			break;
		case E_SOUNDS_LEVEL_2:
			mSoundEngine->play2D(level2,loop);
			break;
		case E_SOUNDS_LEVEL_3:
			mSoundEngine->play2D(level3,loop);
			break;
		case E_SOUNDS_PLACE_BOMB:
			mSoundEngine->play2D(place_bomb,loop);
			break;
		case E_SOUNDS_FOOTSTEPS:
			mSoundEngine->play2D(player_footsteps,loop);
			break;
		case E_SOUNDS_PLAYER_DEATH:
			mSoundEngine->play2D(player_death,loop);
			break;
		case E_SOUNDS_EXPLOSION:
			mSoundEngine->play2D(explosion,loop);
			break;
		case E_SOUNDS_ENEMY_DEATH:
			mSoundEngine->play2D(enemy_death,loop);
			break;
		case E_SOUNDS_NoNo:
			mSoundEngine->play2D(NoNo,loop);
			break;
		default:
			break;
	}
	return;
}

void SoundManager::stopAllSound(void){
	mSoundEngine->stopAllSounds();
	mSoundEngine->drop();
}
