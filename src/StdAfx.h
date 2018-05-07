#pragma once
#include <stdio.h>
#include <Windows.h>
#include <mmsystem.h>
#include <d3dx9.h>
#include <DxErr.h>

#include <string.h>
#include <commctrl.h> 
#include <direct.h>
#include <assert.h>


// #include <iostream>
// #include <vector>
// #include <algorithm>

//////////////////////////////////////////////////////////////////////////
//여기서부터 내가만든파일
#include "DXUT.h"
#include "SHDefine.h"
#include "SHlog.h" //로그파일 남기기
#include "SHUtility.h"
#include "SHGame.h"
#include "Camera.h"

////////////////////////////////////////////////////////////////////////
#pragma warning( disable : 4996 ) // disable deprecated warning 
//#include <strsafe.h>
#pragma warning( default : 4996 )

#pragma warning(disable:4244) // anonymous unions warning
using namespace std;