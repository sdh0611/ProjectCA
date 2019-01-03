#pragma once


#include "..\..\..\stdafx.h"
#include "..\..\Singleton.hpp"


class CSoundManager : public Singleton<CSoundManager>
{
	DECLARE_SINGLETON(CSoundManager)

public:
	enum SoundType {
		SOUND_BGM = 0, SOUND_SFX_PLAYER, SOUND_SFX_ENEMY, SOUND_SFX_PICKUP, 
		SOUND_SFX_ATTACK, SOUND_SFX_BLOCK, SOUND_SFX_UI
	};


private:
	struct SOUND {
		SOUND(FMOD::Sound* pSound, SoundType type, int iIndex = -1)
			:m_pSound(pSound), m_SoundType(type), m_iChannelIndex(iIndex)
		{
		}
		~SOUND()
		{
			m_pSound->release();
		}
		FMOD::Sound*	m_pSound;
		SoundType		m_SoundType;
		int					m_iChannelIndex;
	};
	typedef std::shared_ptr<SOUND> SoundPtr;
	typedef std::weak_ptr<SOUND> WeakSoundPtr;


public:
	bool Init();
	void Update();
	

public:
	void SetChannelVolume(float fVolume, SoundType type);


public:
	bool LoadSound(const char* strPath, const TSTRING& strSoundName, SoundType type, FMOD_MODE mode);
	bool ReleaseSound(const TSTRING& strSoundName);
	bool IsChannelPlaying(SoundType type);


public:
	void SoundPlay(const TSTRING& strSoundName);
	void ChangeBGM(const TSTRING& strSoundName);
	void GroupSoundPause(bool bPause);
	void GroupSoundPause(bool bPause, SoundType type);
	void StopChannel(SoundType type);



private:
	//bool				CreateChannelGroup(const char* strGroupName);
	WeakSoundPtr FindSound(const TSTRING& strSoundName);


private:
	bool LoadEnemySFX();
	bool LoadPlayerSFX();
	bool LoadPickupSFX();
	bool LoadAttackSFX();
	bool LoadBlockSFX();
	bool LoadUISFX();
	bool LoadBGM();



private:
	FMOD::System*								m_pFMODSystem;
	FMOD::Sound*								m_pBGM;
	FMOD::ChannelGroup*					m_pMasterChannelGroup;
	std::vector<FMOD::Channel*>			m_ChannelList;

private:
	typedef std::unordered_map<TSTRING, SoundPtr> SoundList;
	SoundList									m_SoundTable;
	

};