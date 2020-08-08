#include "stdafx.h"
#include "SoundMgr.h"

IMPLEMENT_SINGLETON(CSoundMgr)

CSoundMgr::CSoundMgr()
{
	
}

CSoundMgr::~CSoundMgr()
{
	Free();
}

HRESULT CSoundMgr::Ready_SoundDev()
{
	FMOD_System_Create(&m_pSystem);
	FMOD_System_Init(m_pSystem, SOUND_END, FMOD_INIT_NORMAL, nullptr);



	return S_OK;
}

void CSoundMgr::LoadSoundFile(const char* strSoundName)
{



	char szRelative[256] = "../../Resource/Sound/";
	char szFullPath[256] = "";



	_int result = 0;

		strcpy_s(szFullPath, szRelative);
		strcat_s(szFullPath, strSoundName);

		FMOD_SOUND* pSound = nullptr;

		FMOD_RESULT res = FMOD_System_CreateSound(m_pSystem, szFullPath,
			FMOD_DEFAULT, nullptr, &pSound);

		if (FMOD_OK == res)
		{
			int iBuffSize = strlen(strSoundName) + 1;
			_tchar* pSoundKey = new _tchar[iBuffSize];

			MultiByteToWideChar(CP_ACP, 0, strSoundName, iBuffSize, pSoundKey, iBuffSize);

			m_MapSound.insert({ pSoundKey, pSound });
		}


	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::Update_SoundDev()
{
	FMOD_System_Update(m_pSystem);
}

void CSoundMgr::PlaySound_Ready(const _tchar* pSoundKey, SOUNDID eID)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter_find)
		return;
	int i = (int)eID;
	FMOD_System_PlaySound(m_pSystem, iter_find->second,	0,0, &m_pChannelArr[eID]);
	
}

void CSoundMgr::PlayBGM(const _tchar* pSoundKey)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		[&pSoundKey](auto& MyPair)
	{
		return !lstrcmp(pSoundKey, MyPair.first);
	});

	if (m_MapSound.end() == iter_find)
		return;

	FMOD_System_PlaySound(m_pSystem, iter_find->second, 0, 0, &m_pChannelArr[BGM]);

	FMOD_Channel_SetMode(m_pChannelArr[BGM], FMOD_LOOP_NORMAL);
}

void CSoundMgr::StopSound(SOUNDID eID)
{
	FMOD_Channel_Stop(m_pChannelArr[eID]);
}

void CSoundMgr::StopAll()
{
	for(int i = 0; i < SOUND_END; ++i)
		FMOD_Channel_Stop(m_pChannelArr[i]);
}

void CSoundMgr::Play_Effect(const TCHAR * pSoundKey)
{
	auto iter_find = find_if(m_MapSound.begin(), m_MapSound.end(),
		CTag_Finder(pSoundKey));

	if (m_MapSound.end() == iter_find)
		return;

	int iChanel = 2;
	for (; iChanel < SOUND_END; iChanel++)
	{
		if (!IsChanel_Playing((SOUNDID)iChanel))
			break;
	}



	FMOD_System_PlaySound(m_pSystem, iter_find->second, 0, 0, &m_pChannelArr[iChanel]);
}

FMOD_BOOL CSoundMgr::IsChanel_Playing(SOUNDID eID)
{
	FMOD_BOOL bPlaying = 1;
	FMOD_RESULT hr = FMOD_Channel_IsPlaying(m_pChannelArr[eID], &bPlaying);

	if (hr == FMOD_ERR_INVALID_PARAM)
		return 0;

	return bPlaying;
}

//void CSoundMgr::LoadSoundFile(const char* pFilePath)
//{
//	_finddata_t fd = {};
//
//	// pFilePath: "../Sound/*.*"
//	_int handle = _findfirst(pFilePath, &fd);
//
//	if (0 == handle)
//		return;
//
//	_int iLength = strlen(pFilePath);
//
//	_int i = 0;
//
//	for (; i < iLength; ++i)
//	{
//		if ('*' == pFilePath[i])
//			break;
//	}
//
//	char szRelative[256] = "";
//	char szFullPath[256] = "";
//
//	// szRelative: ../Sound/
//	memcpy(szRelative, pFilePath, i);
//
//	_int result = 0;
//
//	while (-1 != result)
//	{
//		strcpy_s(szFullPath, szRelative);
//		strcat_s(szFullPath, fd.name);
//
//		FMOD_SOUND* pSound = nullptr;
//
//		FMOD_RESULT res = FMOD_System_CreateSound(m_pSystem, szFullPath,
//			FMOD_HARDWARE, nullptr, &pSound);
//
//		if (FMOD_OK == res)
//		{
//			int iBuffSize = strlen(fd.name) + 1;
//			_tchar* pSoundKey = new _tchar[iBuffSize];
//
//			MultiByteToWideChar(CP_ACP, 0, fd.name, iBuffSize, pSoundKey, iBuffSize);
//
//			m_MapSound.insert({ pSoundKey, pSound });
//		}
//
//		result = _findnext(handle, &fd);
//	}
//
//	_findclose(handle);
//	FMOD_System_Update(m_pSystem);
//}

void CSoundMgr::Free(void)
{
	for_each(m_MapSound.begin(), m_MapSound.end(),
		[](auto& MyPair)
	{
		delete[] MyPair.first;
		FMOD_Sound_Release(MyPair.second);
	});

	m_MapSound.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);

}
