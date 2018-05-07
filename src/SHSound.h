/************************************************************************/
/*					일단 다이렉트 사운드 만
					//[2014.12.6 ]
*/
/************************************************************************/

#pragma  once
#pragma comment(lib, "./common/dsound.lib")

class CSManager;
//////////////////////////////////////////////////////////////////////////
// enum 

enum eSoundName
{	
	
	eSound_Enemy_Hit = 0,   // 적피격    반드시 0에서부터 시작!!
	eSound_Enemy_Atk, //  유령 공격   [2014.12.9 ]
	eSound_Tedy_Atk, // 곰돌이 일반공격

	eSound_Skill_Card,
	eSound_Skill_Rock,
	eSound_Skill_Hat,


	eSound_BGM_Play,  // 게임플레이 할때..
	eSound_BGM_Intro, // 조작법설명할때..
	eSound_BGM_Credit, // 크래딧 BGM
	eSound_BGM_Title, // 메인타이틀 BGM
	eSound_BGM_Fail, //실패씬 BGM
	eSound_BGM_Clear, //성공씬 BGM
	eSound_Max
};



/************************************************************************/
/*                     class SHSound
*/
/************************************************************************/
class SHSound
{
	friend class CSManager;

public:
	LPDIRECTSOUND                m_lpDirectSound;
	LPDIRECTSOUNDBUFFER			 m_SoundBuff;

	MMCKINFO					parent;
	MMCKINFO					child;
	WAVEFORMATEX				wavefmt;
	HMMIO						wavefile;

public:
	
	SHSound();
	~SHSound();
	void						Lost();
	
	bool						LoadWave (WCHAR *szFileName);
	// _bLoop : false면 한번만 //[2014.12.6 ]
	void						PlayWave (DWORD _bLoop);

	void						StopWave ();

public:// get.. set

	void						Set_m_LpDirectSound(LPDIRECTSOUND _LPD3dSound){m_lpDirectSound = _LPD3dSound;};						

};



/************************************************************************/
/*                     class CSManager
					//[2014.12.7 ]
*/
/************************************************************************/
class CSManager
{
private:
	SHSound						 m_SoundArr[eSound_Max]; //[2014.12.6 ]
	LPDIRECTSOUND                m_lpDirectSound;
	

public:
	CSManager();
	~CSManager();
	// 반드시 이함수를 게임초기에 한번 호출해야한다. 
	bool						Init (HWND hwnd);


public: // 일반함수 

	// SoundName : enum eSoundName , _bLoop  : 반복되게 할것인가? true or false [2014.12.6 ]
	HRESULT						PlayWave(int SoundName , bool _bLoop );
    // Stop Wav [2014.12.7 ]
	HRESULT						StopWave(int SoundName);
};


//////////////////////////////////////////////////////////////////////////
// 사운드메니저 전역변수
extern CSManager						G_SHSoundMgr; // [2014.12.6 ]

/****************************************************** End *****************************************************/