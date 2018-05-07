// d3d9.lib
// 	d3dx9d.lib
// 	dxerr.lib
// 	dxguid.lib
// winmm.lib
//-----------------------------------------------------------------------------
#include "DXUT.h"
#include "SDKmisc.h"
#include "GameObject.h"
#include "Camera.h"
#include "SHGame.h"


//-----------------------------------------------------------------------------
// 전역변수들!!
//-----------------------------------------------------------------------------




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//메인에서 쓰일 함수들 선언




//-----------------------------------------------------------------------------
// Name: WinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	UNREFERENCED_PARAMETER( hInst );

	if(SHGameFrame->Create(hInst) == S_OK)
		SHGameFrame->ProcessRun();

	return 0;
}








//////////////////////////////////////////////////////////////////////////
// 맵을 그려준다.
/*
void DrawMap()          
{

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	D3DXMATRIX TeapotWorld, TranseM, scaleM;
	D3DXVECTOR3 tempVec;
	

	
	for(int i=0; i<10 ; i++)
	{
		for(int j=0 ; j<10 ; j++)
		{	
			tempVec = D3DXVECTOR3( j*80.0f, 0.0f,  i*80.0f);
			D3DXMatrixTranslation(&TranseM , tempVec.x , 0.0f ,tempVec.z );
		
			if(G_Frustum.IsSphereIN(&tempVec, G_BS.get_Radius()) == true)
			{
				G_BS.Draw(&TranseM);
			}
			
		}
	}

	SHGlobal_GetD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

*/