/***********************************************************************
				GameSysyem.cpp  
				[2014.11.17 ]
		   
		   게임 시스템 변수나 함수를 작성한다.                                                      
************************************************************************/

#include "DXUT.h"

#include "GameSystem.h"

float			G_GameTimer = 0.0f;

float			G_GirlHP = 100.0f; // 아이의 체력 

int				G_CurSceneType = eSceneType_MainTitle; // 현재 씬 종류 [2014.12.3 ]

SH_Timer		G_ButtonAble;// 타이틀버튼을위한 타이머

/*************************************  end of "GameSysyem.cpp" ****************************************/