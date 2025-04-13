#pragma once

#ifdef _DEBUG
#pragma comment(lib, "../Libraries/Lib/lib/x64/fmodL_vc.lib")
#else
#pragma comment(lib, "../Libraries/Lib/lib/x64/fmod_vc.lib")
#endif


#include "fmod_common.h"
#include "fmod.hpp"

class FmodSound
{
public:
	static void init();
	static void playPutBlockSound(BlockType type);
	static void playBGM();
	static void playSelectedSound();
	static void release();

private:
	FmodSound() = delete;
	~FmodSound() = delete;
	FmodSound(const FmodSound&) = delete;
	FmodSound& operator=(const FmodSound&) = delete;

private:
	static FMOD::Channel* channel;
	static FMOD::System* system;
	static FMOD::Sound* put_sound;
	static FMOD::Sound* break_glass;
	static FMOD::Sound* break_leaf;
	static FMOD::Sound* bgm_hal1;
	static FMOD::Sound* selected_sound;
};

