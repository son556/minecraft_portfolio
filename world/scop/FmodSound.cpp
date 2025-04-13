#include "pch.h"
#include "FmodSound.h"

FMOD::Channel* FmodSound::channel = nullptr;
FMOD::System* FmodSound::system = nullptr;
FMOD::Sound* FmodSound::put_sound = nullptr;
FMOD::Sound* FmodSound::bgm_hal1 = nullptr;
FMOD::Sound* FmodSound::break_glass = nullptr;
FMOD::Sound* FmodSound::break_leaf = nullptr;
FMOD::Sound* FmodSound::selected_sound = nullptr;

void FmodSound::init()
{
	FMOD::System_Create(&(system));
	system->init(32, FMOD_INIT_NORMAL, 0);
	system->createSound("./resources/stone_effect.mp3", 
		FMOD_LOOP_OFF, 0, &put_sound);
	system->createSound("./resources/m_click.mp3", 
		FMOD_LOOP_OFF, 0, &selected_sound);
	system->createSound("./resources/hal1.mp3", FMOD_LOOP_NORMAL, 0, &bgm_hal1);
	system->createSound("./resources/minecraft-glass-break.mp3", FMOD_LOOP_OFF,
		0, &break_glass);
	system->createSound("./resources/Leaf_litter_break1.mp3", FMOD_LOOP_OFF, 0,
		&break_leaf);
}

void FmodSound::playPutBlockSound(BlockType type)
{
	if (type == BlockType::GLASS ||
		type == BlockType::TRANSPARENCY_BLUE ||
		type == BlockType::TRANSPARENCY_GREEN ||
		type == BlockType::TRANSPARENCY_RED) {
		system->playSound(break_glass, 0, false, &channel);
	}
	else if (type == BlockType::OAK_LEAVES)
		system->playSound(break_leaf, 0, false, &channel);
	else
		system->playSound(put_sound, 0, false, &channel);
	system->update();
}

void FmodSound::playBGM()
{
	system->playSound(bgm_hal1, 0, false, &channel);
	system->update();
}

void FmodSound::playSelectedSound()
{
	system->playSound(selected_sound, 0, false, &channel);
	system->update();
}

void FmodSound::release()
{
	system->release();
	put_sound->release();
	bgm_hal1->release();
	selected_sound->release();
	break_glass->release();
	break_leaf->release();
}
