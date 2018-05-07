/***********************************************************************
				GameSysyem.h  
				[2014.11.17 ]
		   
		   게임 시스템 변수나 함수를 작성한다.                                                      
************************************************************************/
#pragma  once

extern float	  G_GameTimer ;

extern float	  G_GirlHP ; // 아이의 체력 

extern int		  G_CurSceneType; // 현재 씬 종류 [2014.12.3 ]

extern SH_Timer		G_ButtonAble;// 타이틀버튼을위한 타이머
//////////////////////////////////////////////////////////////////////////
// 씬 종류
enum eScene_Type //[2014.12.3 ]
{
	eSceneType_Loading,
	eSceneType_MainTitle,
	eSceneType_MainGame,
	eSceneType_Clear,
	eSceneType_Fail,
	eSceneType_Credit

};

/*************************************  end of "GameSysyem.h" ****************************************/