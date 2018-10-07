#include <irrKlang.h>
#include <iostream>

enum E_SOUNDS 
{
	E_SOUNDS_FOOTSTEPS,
	E_SOUNDS_PLAYER_DEATH,
	E_SOUNDS_EXPLOSION,
	E_SOUNDS_ENEMY_DEATH,
	E_SOUNDS_PLACE_BOMB,
	E_SOUNDS_LEVEL_1,
	E_SOUNDS_LEVEL_2,
	E_SOUNDS_LEVEL_3,
	E_SOUNDS_NoNo,
};

class SoundManager
{

private:
	irrklang::ISoundEngine *mSoundEngine;
	irrklang::ISoundSource * level1;
	irrklang::ISoundSource * level2;
	irrklang::ISoundSource * level3;
	irrklang::ISoundSource * place_bomb;
	irrklang::ISoundSource * player_footsteps;
	irrklang::ISoundSource * player_death;
	irrklang::ISoundSource * explosion;
	irrklang::ISoundSource * enemy_death;
	irrklang::ISoundSource * NoNo;


public:
	SoundManager();
	~SoundManager();
	SoundManager(const SoundManager & _soundmanager);
	SoundManager & operator=(const SoundManager & _rhs);
	

	void play(E_SOUNDS sound, bool loop);
	void stopAllSound(void);
};