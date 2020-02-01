#pragma once
#include "Engine_Include.h"
#include "Base.h"

typedef struct WaveHeaderType
{
	char			chunkId[4]; 
	unsigned long	chunkSize; 
	char			format[4]; 
	char			subChunkId[4]; 
	unsigned long	subChunkSize; 
	unsigned short	audioFormat; 
	unsigned short	numChannels; 
	unsigned long	sampleRate; 
	unsigned long	bytesPerSecond; 
	unsigned short	blockAlign; 
	unsigned short	bitsPerSample; 
	char			dataChunkId[4]; 
	unsigned long	dataSize;
}SOUNDTYPE;

typedef struct tag3DSound
{
	LPDIRECTSOUNDBUFFER8	m_pSecondary;
	LPDIRECTSOUND3DBUFFER8	m_pSecondary3D;

}SOUND3D;

BEGIN(Engine)

#define MAX_VOLUME 100
#define MIN_VOLUME 0

class ENGINE_DLL CDirectSound : public CBase
{
	DECLARE_SINGLETON(CDirectSound)
private:
	explicit CDirectSound();
	virtual	~CDirectSound();

public:
	HRESULT					Ready_DirectSound(HWND hWnd);
	HRESULT					LoadDirectSound();
	/*____________________________________________________________________
	Normal Sound
	______________________________________________________________________*/
	HRESULT					LoadDirectSoundFile(const wstring wstrFilename);
	void					PlayDirectSoundFile(const wstring wstrSoundKey, 
												bool Loop = false, 
												int Volume = MAX_VOLUME);
	void					StopDirectSoundFile(const wstring wstrSoundKey);
	void					SetDirectSoundVolume(const wstring wstrSoundKey, int volume);
	void					SetDirectSoundFrequency(const wstring wstrSoundKey, float dwValue = 1.f);
	int						GetSoundVolume(const wstring wstrSoundKey);
	bool					IsSoundPlaying(const wstring wstrSoundKey);

	/*____________________________________________________________________
	3D Sound
	______________________________________________________________________*/
	HRESULT					Load3DSoundFile(TCHAR* filename);
	void					Play3DSoundFile(const wstring wstrSoundKey, 
											_vec3 vPos, 
											float Max, 
											float Min, 
											bool Loop = false, 
											int Volume = MAX_VOLUME);
	void					Stop3DSoundFile(const wstring wstrSoundKey);
	void					SetListenerPos(const _vec3& vPos);
	// const _vec3&			GetListenerPos();
	void					Set3DSoundPos(const wstring wstrSoundKey, const _vec3& vPos);
	/*____________________________________________________________________
	Deafult
	______________________________________________________________________*/
	void					PlayDirectSoundEffect(const wstring wstrSoundKey, int Volume = MAX_VOLUME);
private:
	HRESULT					InitDirectSound(HWND hWnd);
	LPDIRECTSOUNDBUFFER8	FindSound(const wstring wstrSoundKey);
	LONG					Vol_to_DB(int vol);
	int DB_To_Vol(LONG Decibel);

private:
	LPDIRECTSOUND8						m_pDirectSound		= nullptr;
	LPDIRECTSOUNDBUFFER					m_pPrimaryBuffer	= nullptr;

	map<wstring, LPDIRECTSOUNDBUFFER8>	m_MapSound;
	map<wstring, SOUND3D*>				m_Map3DSound;

	LPDIRECTSOUND3DLISTENER8			m_pListener			= nullptr;

private:
	virtual void Free();
};

END