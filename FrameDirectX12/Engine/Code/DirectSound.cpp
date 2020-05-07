#include "DirectSound.h"

USING(Engine)
IMPLEMENT_SINGLETON(CDirectSound)

CDirectSound::CDirectSound()
{
}

CDirectSound::~CDirectSound()
{

}

HRESULT CDirectSound::Ready_DirectSound(HWND hWnd)
{
	FAILED_CHECK(InitDirectSound(hWnd));

	return S_OK;
}

HRESULT CDirectSound::LoadDirectSound()
{
	// Path로드
	wstring strFilePath = L"../../Data/Data_SoundPath/SoundPath.txt";

	wifstream fIn;
	fIn.open(strFilePath.c_str());

	if (fIn.fail())
		return E_FAIL;

	fIn.imbue(std::locale("kor"));

	TCHAR szFileName[MAX_STR] = L"";
	while (!fIn.eof())
	{
		fIn.getline(szFileName, MAX_STR);
		LoadDirectSoundFile(szFileName);
	}
	fIn.close();

	return S_OK;
}

HRESULT CDirectSound::InitDirectSound(HWND hWnd)
{
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	if (FAILED(DirectSoundCreate8(NULL, &m_pDirectSound, NULL)))
	{
		MSGERR_BOX(L"DirectSound Create Failed");
		return E_FAIL;
	}

	if (FAILED(m_pDirectSound->SetCooperativeLevel(hWnd, DSSCL_PRIORITY)))
	{
		MSGERR_BOX(L"DirectSound SetCooperativeLevel Failed");
		return E_FAIL;
	}


	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	if (FAILED(m_pDirectSound->CreateSoundBuffer(&bufferDesc, &m_pPrimaryBuffer, NULL)))
	{
		MSGERR_BOX(L"DirectSound CreateSoundBuffer Failed");
		return E_FAIL;
	}

	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 48000;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	if(FAILED(m_pPrimaryBuffer->SetFormat(&waveFormat)))
	{
		MSGERR_BOX(L"DirectSound SetFormat Failed");
		return E_FAIL;
	}

	if (FAILED(m_pPrimaryBuffer->QueryInterface(IID_IDirectSound3DListener8, (LPVOID*)&m_pListener)))
	{
		MSGERR_BOX(L"DirectSound Set Listener Failed");
		return E_FAIL;
	}

	m_pListener->SetPosition(0.f, 0.f, 0.f, DS3D_IMMEDIATE);

	return S_OK;
}

LPDIRECTSOUNDBUFFER8 CDirectSound::FindSound(const wstring wstrSoundKey)
{
	auto iter_find = m_MapSound.find(wstrSoundKey);

	if (iter_find == m_MapSound.end())
		return nullptr;

	return iter_find->second;
}

LONG CDirectSound::Vol_to_DB(int vol)
{
	if (vol == 100)
		return 0;
	else if (vol == 0)
		return -10000;
	else
		return (LONG)(2000.f * log10(vol / 100.f));
}

int CDirectSound::DB_To_Vol(LONG Decibel)
{
	if (Decibel == -10000)
		return 0;
	else if (Decibel == 0)
		return 100;
	else
		return (int)(powf(10.f, Decibel / 2000.f) * 100.f);
}

HRESULT CDirectSound::LoadDirectSoundFile(const wstring wstrFilename)
{
	int						error;
	FILE*					filePtr;
	unsigned int			count;
	WaveHeaderType			waveFileHeader;
	WAVEFORMATEX			waveFormat;
	DSBUFFERDESC			bufferDesc;
	IDirectSoundBuffer*		tempBuffer;
	unsigned char*			waveData;
	unsigned char*			bufferPtr;
	unsigned long			bufferSize;


	TCHAR szFile[256] = L"../../Resource/Sound/";
	lstrcat(szFile, wstrFilename.c_str());
	lstrcat(szFile, L".wav");

	char cTemp[256];

	WideCharToMultiByte(CP_ACP, 0, szFile, 256, cTemp, 256, NULL, NULL);

	// Open the wave file in binary.
	error = fopen_s(&filePtr, cTemp, "rb");
	if (error != 0)
		return E_FAIL;

	// Read in the wave file header.
	count = (_uint)fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
		return E_FAIL;

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return E_FAIL;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return E_FAIL;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return E_FAIL;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	/*if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return E_FAIL;
	}*/

	// Check that the wave file was recorded in stereo format.
	/*if (waveFileHeader.numChannels != 2)
	{
		return E_FAIL;
	}*/

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	/*if (waveFileHeader.sampleRate != 48000)
	{
		return E_FAIL;
	}*/

	// Ensure that the wave file was recorded in 16 bit format.
	/*if (waveFileHeader.bitsPerSample != 16)
	{
		return E_FAIL;
	}*/

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return E_FAIL;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag		= WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec	= waveFileHeader.sampleRate;
	waveFormat.wBitsPerSample	= waveFileHeader.bitsPerSample;
	waveFormat.nChannels		= waveFileHeader.numChannels;
	waveFormat.nBlockAlign		= (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec	= waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize			= 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	if (FAILED(m_pDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return E_FAIL;
	}

	IDirectSoundBuffer8* secondaryBuffer = 0;
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondaryBuffer)))
	{
		return E_FAIL;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	ZeroMemory(waveData, sizeof(unsigned char) * waveFileHeader.dataSize);

	// Read in the wave file data into the newly created buffer.
	count = (_uint)fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return E_FAIL;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return E_FAIL;
	}

	// Lock the secondary buffer to write wave data into it.
	if (FAILED((secondaryBuffer)->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return E_FAIL;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	if (FAILED((secondaryBuffer)->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return E_FAIL;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	m_MapSound.emplace(wstrFilename, secondaryBuffer);

	return S_OK;
}

HRESULT CDirectSound::Load3DSoundFile(TCHAR * filename)
{
	int						error;
	FILE*					filePtr;
	unsigned int			count;
	WaveHeaderType			waveFileHeader;
	WAVEFORMATEX			waveFormat;
	DSBUFFERDESC			bufferDesc;
	IDirectSoundBuffer*		tempBuffer;
	unsigned char*			waveData;
	unsigned char*			bufferPtr;
	unsigned long			bufferSize;

	TCHAR szFile[256] = L"../../Resource/Sound/";
	lstrcat(szFile, filename);
	lstrcat(szFile, L".wav");

	char cTemp[256];

	WideCharToMultiByte(CP_ACP, 0, szFile, 256, cTemp, 256, NULL, NULL);

	// Open the wave file in binary.
	error = fopen_s(&filePtr, cTemp, "rb");
	if (error != 0)
	{
		return E_FAIL;
	}

	// Read in the wave file header.
	count = (_uint)fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return E_FAIL;
	}

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return E_FAIL;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return E_FAIL;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return E_FAIL;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return E_FAIL;
	}

	// Check that the wave file was recorded in Mono format.
	/*if (waveFileHeader.numChannels != 1)
	{
		return E_FAIL;
	}*/

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		return E_FAIL;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return E_FAIL;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return E_FAIL;
	}

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 1;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_CTRL3D | DSBCAPS_CTRLFREQUENCY | DSBCAPS_MUTE3DATMAXDISTANCE;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	if (FAILED(m_pDirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL)))
	{
		return E_FAIL;
	}

	LPDIRECTSOUNDBUFFER8 secondaryBuffer = 0;
	// Test the buffer format against the direct sound 8 interface and create the secondary buffer.
	if (FAILED(tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondaryBuffer)))
	{
		return E_FAIL;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return E_FAIL;
	}

	// Read in the wave file data into the newly created buffer.
	count = (_uint)fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return E_FAIL;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return E_FAIL;
	}

	// Lock the secondary buffer to write wave data into it.
	if (FAILED(secondaryBuffer->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0)))
	{
		return E_FAIL;
	}

	// Copy the wave data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	if (FAILED(secondaryBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0)))
	{
		return E_FAIL;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	// 3D 사운드 버퍼 Get
	LPDIRECTSOUND3DBUFFER8 m_p3DBuffer = 0;
	if (FAILED(secondaryBuffer->QueryInterface(IID_IDirectSound3DBuffer8, (void**)&m_p3DBuffer)))
	{
		return E_FAIL;
	}

	SOUND3D* p3DSound = new SOUND3D;
	p3DSound->m_pSecondary = secondaryBuffer;
	p3DSound->m_pSecondary3D = m_p3DBuffer;

	m_Map3DSound.emplace(filename, p3DSound);

	return S_OK;
}

void CDirectSound::PlayDirectSoundFile(const wstring wstrSoundKey,
	bool Loop/* = false*/, int Volume/* = DSBVOLUME_MAX*/)
{
	LPDIRECTSOUNDBUFFER8 pSound = FindSound(wstrSoundKey);

	if (pSound == nullptr)
		return;

	// Set position at the beginning of the sound buffer.
	pSound->SetCurrentPosition(0);

	pSound->SetVolume(Vol_to_DB(Volume));

	// Play the contents of the secondary sound buffer.
	if (Loop)
		pSound->Play(0, 0, DSBPLAY_LOOPING);
	else
		pSound->Play(0, 0, 0);
}

void CDirectSound::Play3DSoundFile(const wstring wstrSoundKey, _vec3 vPos,
	float Max, float Min, bool Loop, int Volume)
{
	auto iter_find = m_Map3DSound.find(wstrSoundKey);

	if (iter_find == m_Map3DSound.end())
		return;

	SOUND3D* pBuffer = iter_find->second;

	pBuffer->m_pSecondary->SetCurrentPosition(0);
	pBuffer->m_pSecondary->SetVolume(Vol_to_DB(Volume));

	pBuffer->m_pSecondary3D->SetPosition(vPos.x, vPos.y, vPos.z, DS3D_IMMEDIATE);
	pBuffer->m_pSecondary3D->SetMaxDistance(Max, DS3D_IMMEDIATE);
	pBuffer->m_pSecondary3D->SetMinDistance(Min, DS3D_IMMEDIATE);

	if (Loop)
		pBuffer->m_pSecondary->Play(0, 0, DSBPLAY_LOOPING);
	else
		pBuffer->m_pSecondary->Play(0, 0, 0);
}

void CDirectSound::PlayDirectSoundEffect(const wstring wstrSoundKey, int Volume)
{
	LPDIRECTSOUNDBUFFER8 pSound = FindSound(wstrSoundKey);

	if (pSound == nullptr)
		return;

	LPDIRECTSOUNDBUFFER pBuffer;
	m_pDirectSound->DuplicateSoundBuffer(pSound, &pBuffer);

	pBuffer->SetCurrentPosition(0);

	pBuffer->SetVolume(Vol_to_DB(Volume));

	pBuffer->Play(0, 0, 0);
}

void CDirectSound::StopDirectSoundFile(const wstring wstrSoundKey)
{
	LPDIRECTSOUNDBUFFER8 pSound = FindSound(wstrSoundKey);

	if (pSound == nullptr)
		return;

	pSound->Stop();
}

void CDirectSound::Stop3DSoundFile(const wstring wstrSoundKey)
{
	auto iter_find = m_Map3DSound.find(wstrSoundKey);

	if (iter_find == m_Map3DSound.end())
		return;

	iter_find->second->m_pSecondary->Stop();
}

void CDirectSound::SetDirectSoundVolume(const wstring wstrSoundKey, int Volume)
{
	LPDIRECTSOUNDBUFFER8 pSound = FindSound(wstrSoundKey);

	if (pSound == nullptr)
		return;

	pSound->SetVolume(Vol_to_DB(Volume));
}

void CDirectSound::SetListenerPos(const _vec3 & vPos)
{
	m_pListener->SetPosition(vPos.x, vPos.y, vPos.z, DS3D_IMMEDIATE);
}

//const _vec3 & CDirectSound::GetListenerPos()
//{
//	// TODO: 여기에 반환 구문을 삽입합니다.
//	_vec3* vPos = nullptr;
//
//	m_pListener->GetPosition(vPos);
//
//	return *vPos;
//}

void CDirectSound::Set3DSoundPos(const wstring wstrSoundKey, const _vec3 & vPos)
{
	auto iter_find = m_Map3DSound.find(wstrSoundKey);

	if (iter_find == m_Map3DSound.end())
		return;

	iter_find->second->m_pSecondary3D->SetPosition(vPos.x, vPos.y, vPos.z, DS3D_IMMEDIATE);
}

void CDirectSound::SetDirectSoundFrequency(const wstring wstrSoundKey, float dwValue)
{
	LPDIRECTSOUNDBUFFER8 pSound = FindSound(wstrSoundKey);

	if (pSound == nullptr)
		return;

	DWORD dwCurFreq;
	pSound->GetFrequency(&dwCurFreq);

	pSound->SetFrequency(DWORD((float)dwCurFreq * dwValue));
}

int CDirectSound::GetSoundVolume(const wstring wstrSoundKey)
{
	auto iter_find = FindSound(wstrSoundKey);

	if (iter_find == nullptr)
		return -1;

	LONG Volume;
	iter_find->GetVolume(&Volume);

	int iVolume = (int)DB_To_Vol(Volume);

	return iVolume;
}

bool CDirectSound::IsSoundPlaying(const wstring wstrSoundKey)
{
	auto iter_find = FindSound(wstrSoundKey);

	if (iter_find == nullptr)
		return false;

	DWORD dwState;
	iter_find->GetStatus(&dwState);

	if (dwState & DSBSTATUS_PLAYING)
		return true;

	return false;
}

void CDirectSound::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy DirectSound");
#endif

	for_each(m_MapSound.begin(), m_MapSound.end(), CDeleteMap());
	m_MapSound.clear();

	for_each(m_Map3DSound.begin(), m_Map3DSound.end(),
		[](auto& MyPair)
	{
		Safe_Release(MyPair.second->m_pSecondary);
		Safe_Release(MyPair.second->m_pSecondary3D);
		Safe_Delete(MyPair.second);
	});
	m_Map3DSound.clear();

	if (m_pListener)
		Safe_Release(m_pListener);

	// Release the primary sound buffer pointer.
	if (m_pPrimaryBuffer)
		Safe_Release(m_pPrimaryBuffer);

	// Release the direct sound interface pointer.
	if (m_pDirectSound)
		Safe_Release(m_pDirectSound);
}