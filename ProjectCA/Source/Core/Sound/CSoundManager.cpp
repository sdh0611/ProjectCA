#include "..\..\..\stdafx.h"
#include "..\..\..\Include\Core\Sound\CSoundManager.h"
#include <fmod_errors.h>


CSoundManager::CSoundManager()
{
}

CSoundManager::~CSoundManager()
{
	m_SoundTable.clear();
	m_pMasterChannelGroup->release();
	m_pFMODSystem->release();
	m_pFMODSystem->close();
}

bool CSoundManager::Init()
{
	//FMOD System변수 초기화
	auto result = FMOD::System_Create(&m_pFMODSystem);
	if (result != FMOD_OK)
	{
		printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	result = m_pFMODSystem->init(100, FMOD_INIT_NORMAL, NULL);
	if (result != FMOD_OK)
	{
		printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	//FMOD MasterGroup Channel 초기화
	result = m_pFMODSystem->getMasterChannelGroup(&m_pMasterChannelGroup);
	if (result != FMOD_OK)
	{
		printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}

	//ChannelList 초기화
	m_ChannelList.assign(7, nullptr);

	//Load Sound
	if (!LoadEnemySFX())
	{
		printf("[ERROR] LoadEnemySFX Error.\n");
		return false;
	}

	if (!LoadPlayerSFX())
	{
		printf("[ERROR] LoadPlayerSFX Error.\n");
		return false;
	}

	if (!LoadPickupSFX())
	{
		printf("[ERROR] LoadPickupSFX Error.\n");
		return false;
	}

	if (!LoadAttackSFX())
	{
		printf("[ERROR] LoadAttackSFX Error.\n");
		return false;
	}

	if (!LoadBlockSFX())
	{
		printf("[ERROR] LoadBlockSFX Error.\n");
		return false;
	}

	if (!LoadUISFX())
	{
		printf("[ERROR] LoadUISFX Error.\n");
		return false;
	}

	if (!LoadBGM())
	{
		printf("[ERROR] LoadBGM Error.\n");
		return false;
	}

	return true;
}

void CSoundManager::Update()
{
	m_pFMODSystem->update();
	//bool bPlay;
	//m_ChannelGroupList[SOUND_BGM]->isPlaying(&bPlay);
	//if (!bPlay)
	//{
	//	printf("Play\n");
	//}
}

void CSoundManager::SetChannelVolume(float fVolume, SoundType type)
{
	m_ChannelList[type]->setVolume(fVolume);
}

bool CSoundManager::LoadSound(const char* strPath, const TSTRING& strSoundName, 
	CSoundManager::SoundType type, FMOD_MODE mode)
{
	FMOD::Sound* pSound;
	auto result = m_pFMODSystem->createSound(strPath, mode, 0, &pSound);
	if (result != FMOD_OK)
	{
		printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
		return false;
	}
	
	return m_SoundTable.emplace(strSoundName, std::make_shared<SOUND>(pSound, type)).second;
}

bool CSoundManager::ReleaseSound(const TSTRING & strSoundName)
{
	auto it = m_SoundTable.find(strSoundName);
	if (it != m_SoundTable.end())
	{
		it->second->m_pSound->release();
		m_SoundTable.erase(it);
		return true;
	}

	printf("[ERROR] Sound not exist on SoundTable.\n");

	return false;
}

bool CSoundManager::IsChannelPlaying(SoundType type)
{
	bool bPlaying;
	m_ChannelList[type]->isPlaying(&bPlaying);
	
	return bPlaying;
}

//bool CSoundManager::CreateChannelGroup(const char * strGroupName)
//{
//	FMOD::ChannelGroup* pGroup;
//	auto result = m_pFMODSystem->createChannelGroup(strGroupName, &pGroup);
//	if (result != FMOD_OK)
//	{
//		printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
//		return false;
//	}
//	m_pMasterChannelGroup->addGroup(pGroup);
//	m_ChannelGroupList.emplace_back(pGroup);
//	
//	return true;
//}

void CSoundManager::SoundPlay(const TSTRING & strSoundName)
{
	auto pSound = FindSound(strSoundName);
	if (!pSound.expired())
	{
		//PlaySound
		auto result = m_pFMODSystem->playSound(pSound.lock()->m_pSound,0, false, &m_ChannelList[pSound.lock()->m_SoundType]);
		if (result != FMOD_OK)
		{
			printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
			return;
		}

		//Get channel index
		result = m_ChannelList[pSound.lock()->m_SoundType]->getIndex(&(pSound.lock()->m_iChannelIndex));
		if (result != FMOD_OK)
		{
			printf("[ERROR] (%d) %s\n", result, FMOD_ErrorString(result));
			return;
		}
	}
	

}

void CSoundManager::ChangeBGM(const TSTRING & strSoundName)
{
	StopChannel(SOUND_BGM);
	SoundPlay(strSoundName);
}

void CSoundManager::GroupSoundPause(bool bPause)
{
	m_pMasterChannelGroup->setPaused(bPause);
}

void CSoundManager::GroupSoundPause(bool bPause, SoundType type)
{
	//m_ChannelGroupList[type]->setPaused(bPause);
}

void CSoundManager::StopChannel(SoundType type)
{
	bool bPlaying;
	m_ChannelList[type]->isPlaying(&bPlaying);

	if (bPlaying)
	{
		puts("StopChannel");
		m_ChannelList[type]->stop();
	}
}

CSoundManager::WeakSoundPtr CSoundManager::FindSound(const TSTRING & strSoundName)
{
	auto it = m_SoundTable.find(strSoundName);
	if (it == m_SoundTable.end())
	{
		return WeakSoundPtr();
	}

	return it->second;
}


bool CSoundManager::LoadEnemySFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_stomp.wav", TEXT("SFXStomp"), SOUND_SFX_ENEMY, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_spin_stomp.wav", TEXT("SFXSpinStomp"), SOUND_SFX_ENEMY, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_shell_ricochet.wav", TEXT("SFXShellRicochet"), SOUND_SFX_ENEMY, FMOD_DEFAULT))
	{
		return false;
	}


	return true;
}

bool CSoundManager::LoadPlayerSFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_spin_jump.wav", TEXT("SFXSpinJump"), SOUND_SFX_PLAYER, FMOD_DEFAULT))
	{
		return false;
	}
	
	if (!LoadSound("./Resources/Sound/SFX/smw_jump.wav", TEXT("SFXJump"), SOUND_SFX_PLAYER, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_kick.wav", TEXT("SFXKick"), SOUND_SFX_PLAYER, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_pipe.wav", TEXT("SFXPipe"), SOUND_SFX_PLAYER, FMOD_DEFAULT))
	{
		return false;
	}
	return true;
}

bool CSoundManager::LoadPickupSFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_power-up.wav", TEXT("SFXPowerUp"), SOUND_SFX_PICKUP, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_power-up_appears.wav", TEXT("SFXPowerUpAppear"), SOUND_SFX_PICKUP, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_reserve_item_release.wav", TEXT("SFXReserveItemReleaseUp"), SOUND_SFX_PICKUP, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_coin.wav", TEXT("SFXCoin"), SOUND_SFX_PICKUP, FMOD_DEFAULT))
	{
		return false;
	}

	return true;
}

bool CSoundManager::LoadAttackSFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_fireball.wav", TEXT("SFXFireball"), SOUND_SFX_ATTACK, FMOD_DEFAULT))
	{
		return false;
	}
	
	return true;
}

bool CSoundManager::LoadBlockSFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_break_block.wav", TEXT("SFXBreakBlock"), SOUND_SFX_BLOCK, FMOD_DEFAULT))
	{
		return false;
	}

	return true;
}

bool CSoundManager::LoadUISFX()
{
	if (!LoadSound("./Resources/Sound/SFX/smw_switch_activated.wav", TEXT("SFXButtonActivated"), SOUND_SFX_UI, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_map_move_to_spot.wav", TEXT("SFXMovePointer"), SOUND_SFX_UI, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/SFX/smw_message_block.wav", TEXT("SFXUIActivated"), SOUND_SFX_UI, FMOD_DEFAULT))
	{
		return false;
	}

	return true;
}

bool CSoundManager::LoadBGM()
{
	if (!LoadSound("./Resources/Sound/BGM/smw_title.mp3", TEXT("BGMTitle"), SOUND_BGM, FMOD_LOOP_NORMAL))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/BGM/smw_athletic.mp3", TEXT("BGMAthletic"), SOUND_BGM, FMOD_LOOP_NORMAL))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/BGM/smw_overworld.mp3", TEXT("BGMOverworld"), SOUND_BGM, FMOD_LOOP_NORMAL))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/BGM/smw_game_over.wav", TEXT("BGMGameOver"), SOUND_BGM, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/BGM/smw_lost_a_life.wav", TEXT("BGMDead"), SOUND_BGM, FMOD_DEFAULT))
	{
		return false;
	}

	if (!LoadSound("./Resources/Sound/BGM/smw_castle_clear.wav", TEXT("BGMCastleClear"), SOUND_BGM, FMOD_DEFAULT))
	{
		return false;
	}

	return true;
}
