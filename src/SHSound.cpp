#include "DXUT.h"
#include "SHSound.h"

//////////////////////////////////////////////////////////////////////////
// 전역변수 선언부 
CSManager					G_SHSoundMgr; // [2014.12.6 ]



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 class  CSManager 함수정의
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//-----------------------------------------------------------------------------
// CSManager 의 생성자
//-----------------------------------------------------------------------------
CSManager::CSManager()
{m_lpDirectSound=NULL;}

//-----------------------------------------------------------------------------
// CSManager 의 소멸자
//-----------------------------------------------------------------------------
CSManager::~CSManager()
{
	for(int i =0 ; i< eSound_Max ; i++)
	{
		SAFE_RELEASE(m_SoundArr[i].m_SoundBuff)  // 반드시 사운드버퍼들 먼저 다제거하고나서 LPDIRECTSOUND 객체를 제거해야한다.
	}


	SAFE_RELEASE(  m_lpDirectSound );//LPDIRECTSOUND 객체를 제거


}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
bool CSManager::Init(HWND hwnd)
{
        if(DirectSoundCreate (NULL, &m_lpDirectSound, NULL) != DS_OK)
        {         return (false);        }
        
        if(m_lpDirectSound->SetCooperativeLevel (hwnd, DSSCL_PRIORITY) != DS_OK)
        {          return (false);        }
     
		// 여기서 모든 사운드들에게 다에렉트 사운드객체포인터를 넘겨주어야 한다.!! [2014.12.6 ]
		for(int i = 0; i< eSound_Max ; i++)
		{
			m_SoundArr[i].Set_m_LpDirectSound(m_lpDirectSound);
		}


		//////////////////////////////////////////////////////////////////////////
		// 여기서부터 wav파일 로딩
		if(!m_SoundArr[eSound_BGM_Play].LoadWave(L"./Sound/BG/Play.wav")) // 유령 한대 맞았을때[2014.12.6 ]
		{	SHUtil::MsgBoxW( L"Loading Play.wav Fail" ); }

		if(!m_SoundArr[eSound_Enemy_Hit].LoadWave(L"./Sound/EF/N_hit.wav")) // 유령 한대 맞았을때[2014.12.6 ]
		{	SHUtil::MsgBoxW( L"Loading N_hit.wav Fail" ); }

		if(!m_SoundArr[eSound_Enemy_Atk].LoadWave(L"./Sound/EF/N2_Atk.wav")) // 유령 한대 맞았을때  [2014.12.9 ]
		{	SHUtil::MsgBoxW( L"Loading N2_Atk.wav Fail" ); }

		if(!m_SoundArr[eSound_Tedy_Atk].LoadWave(L"./Sound/EF/T_Atk.wav")) // 유령 한대 맞았을때[2014.12.6 ]
		{	SHUtil::MsgBoxW( L"Loading T_Atk.wav Fail" ); }

		if(!m_SoundArr[eSound_BGM_Intro].LoadWave(L"./Sound/BG/Intro.wav")) //조작법설명떄 나올 인트로음악 [2014.12.7 ]
		{	SHUtil::MsgBoxW( L"Loading Intro.wav Fail" ); }
		//m_SoundArr[eSound_Tedy_Atk].m_SoundBuff->SetVolume(-2000);
		
		if(!m_SoundArr[eSound_BGM_Credit].LoadWave(L"./Sound/BG/Credit.wav")) //크레딧에 나올 음악 [2014.12.7 ]
		{	SHUtil::MsgBoxW( L"Loading Credit.wav Fail" ); }
		
		if(!m_SoundArr[eSound_BGM_Title].LoadWave(L"./Sound/BG/Title.wav")) // 타이들에 나올 음악 [2014.12.7 ]
		{	SHUtil::MsgBoxW( L"Loading Title.wav Fail" ); }

		if(!m_SoundArr[eSound_BGM_Fail].LoadWave(L"./Sound/BG/Fail.wav")) // 망했을때 나올 음악 [2014.12.7 ]
		{	SHUtil::MsgBoxW( L"Loading Fail.wav Fail" ); }

		if(!m_SoundArr[eSound_BGM_Clear].LoadWave(L"./Sound/BG/Clear.wav")) // 성공씬에서 나올 음악 [2014.12.7 ]
		{	SHUtil::MsgBoxW( L"Loading Clear.wav Fail" ); }


        return (true);
}



//-----------------------------------------------------------------------------
// Arg    : SoundName : enum eSoundName , _bLoop  : 반복되게 할것인가? true or false [2014.12.6 ]
// Return : HRESULT잘못된 인덱스라면 S_FALSE , 성공시 S_OK
// Desc   : 사운드를 재생한다.
//-----------------------------------------------------------------------------
HRESULT		CSManager::PlayWave(int SoundName , bool _bLoop )
{
	if((0 > SoundName ) ||  (eSound_Max <= SoundName))
	{
		SHUtil::MsgBoxW(L"CSManager::StopWave() ,  Fail , 잘못된 인덱스");
		return S_FALSE;
	}

	m_SoundArr[SoundName].PlayWave(_bLoop);

	return S_OK;

}


//-----------------------------------------------------------------------------
// Arg    : SoundName : enum eSoundName  [2014.12.7 ]
// Return : HRESULT잘못된 인덱스라면 S_FALSE , 성공시 S_OK 
// Desc   : 사운드를 멈춘다. 
//-----------------------------------------------------------------------------
HRESULT		CSManager::StopWave(int SoundName)
{
	if((0 > SoundName ) ||  (eSound_Max <= SoundName))
	{	SHUtil::MsgBoxW(L"CSManager::StopWave() ,  Fail , 잘못된 인덱스");
		return S_FALSE;
	}

	m_SoundArr[SoundName].StopWave();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 class SHSound 함수정의
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
SHSound::SHSound()
{
	  m_lpDirectSound = nullptr;
	  m_SoundBuff= nullptr;
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
SHSound::~SHSound()
{
	if(m_SoundBuff != nullptr)
	  SAFE_RELEASE(m_SoundBuff)
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
void SHSound::Lost()
{
		if (m_SoundBuff != NULL)
        {
                m_SoundBuff->Release();
                m_SoundBuff = NULL;
        }
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
bool SHSound::LoadWave (LPWSTR  szFileName)
{
 HMMIO wavefile;
        wavefile = mmioOpen (szFileName, 0, MMIO_READ | MMIO_ALLOCBUF);
        if (wavefile == NULL) 
        {		MessageBox( DXUTGetHWND(),L"mmioOpen Fail", L"DXUT Application", MB_ICONERROR|MB_OK ); 
                return (false);
        }
        
        MMCKINFO parent;
        memset (&parent, 0, sizeof(MMCKINFO));
        parent.fccType = mmioFOURCC ('W', 'A', 'V', 'E');
        mmioDescend (wavefile, &parent, 0, MMIO_FINDRIFF);
        
        MMCKINFO child;
        memset (&child, 0, sizeof(MMCKINFO));
        child.fccType = mmioFOURCC ('f', 'm', 't', ' ');
        mmioDescend (wavefile, &child, &parent, 0);
        
        WAVEFORMATEX wavefmt;
        mmioRead (wavefile, (char*)&wavefmt, sizeof(wavefmt));
        if(wavefmt.wFormatTag != WAVE_FORMAT_PCM) 
        {	MessageBox( DXUTGetHWND(),L"mmioRead Fail", L"DXUT Application", MB_ICONERROR|MB_OK ); 
                return (false);
        }
        
        mmioAscend (wavefile, &child, 0);
        child.ckid = mmioFOURCC ('d', 'a', 't', 'a');
        mmioDescend (wavefile, &child, &parent, MMIO_FINDCHUNK);
        //ㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡㅡ
        DSBUFFERDESC bufdesc;
        memset (&bufdesc, 0, sizeof(DSBUFFERDESC));
        bufdesc.dwSize = sizeof(DSBUFFERDESC);
        
        bufdesc.dwFlags = DSBCAPS_CTRLVOLUME;
        bufdesc.dwBufferBytes = child.cksize;
        bufdesc.lpwfxFormat = &wavefmt;

        if((m_lpDirectSound->CreateSoundBuffer (&bufdesc, &m_SoundBuff, NULL)) != DS_OK)
        {       MessageBox( DXUTGetHWND(),L"CreateSBuffer Fail", L"DXUT Application", MB_ICONERROR|MB_OK ); 
                return (false);
        }
        
        void *write1 = 0, *write2 = 0;
        unsigned long length1,length2;
        m_SoundBuff->Lock (0, child.cksize, &write1, &length1, &write2, &length2, 0);
        if(write1 > 0)
                mmioRead (wavefile, (char*)write1, length1);
        if (write2 > 0)
                mmioRead (wavefile, (char*)write2, length2);
        m_SoundBuff->Unlock (write1, length1, write2, length2);
        
        mmioClose (wavefile, 0);
        
        return (true);
}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   : 반복문에서 사용하면 안된다.(계속 처음부분만재생됨.. )
//-----------------------------------------------------------------------------
void SHSound::PlayWave (DWORD _bLoop)
{
        m_SoundBuff->SetCurrentPosition (0);
        m_SoundBuff->Play (0, 0, _bLoop);
}


//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
void SHSound::StopWave ()
{
	 m_SoundBuff->Stop ();

	
}


/****************************************************** End *****************************************************/