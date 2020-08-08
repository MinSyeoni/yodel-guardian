#pragma once

#include "Include.h"
#include "Base.h"

class  CSoundMgr : public CBase
{
	DECLARE_SINGLETON(CSoundMgr)
public:
	enum SOUNDID { BGM, OBJECT = 97,PLAYER = 98,MONSTER = 99,SOUND_END = 100 };

private:
	CSoundMgr();
	~CSoundMgr();

public:
	HRESULT Ready_SoundDev();
public:
	void LoadSoundFile(const char* strSoundName);
	void Update_SoundDev();
	void PlaySound_Ready(const _tchar* pSoundKey, SOUNDID eID);
	void PlayBGM(const _tchar* pSoundKey);
	void StopSound(SOUNDID eID);
	void StopAll();
	void		Play_Effect(const TCHAR* pSoundKey);

public:
	FMOD_BOOL	IsChanel_Playing(SOUNDID eID);



private:
	FMOD_SYSTEM*	m_pSystem;
	FMOD_CHANNEL*	m_pChannelArr[SOUND_END];
	FMOD_CHANNELGROUP* m_pGroup = 0;
	

	map<const _tchar*, FMOD_SOUND*>	m_MapSound;

public:
	virtual void	Free(void);

};

