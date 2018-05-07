//--------------------------------------------------------------------------------------
// File: SDKmisc.cpp
//
// Various helper functionality that is shared between SDK samples
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#include "DXUT.H"
#include "SDKmisc.h"
#undef min // use __min instead
#undef max // use __max instead


//------------------------------------------------------------------
// 버텍스 구조체 채우기
//------------------------------------------------------------------
void SET_VTX_RHW(CVERTEX_RHW &ct, D3DXVECTOR4 p, DWORD clr)														{	ct.pos=p;	ct.color=clr;	}
void SET_VTX_DIF(CVERTEX_DIF &ct, D3DXVECTOR3 p, DWORD clr)														{	ct.pos=p;	ct.color=clr;	}
void SET_VTX_NORMAL(CVERTEX_NORMAL &ct, D3DXVECTOR3 p, D3DXVECTOR3 n)											{	ct.pos=p;	ct.normal=n;	}
void SET_VTX_RHWTEX(CVERTEX_RHWTEX &ct, D3DXVECTOR4 p, float tu, float tv)										{	ct.pos=p;	ct.tu=tu;		ct.tv=tv;	}
void SET_VTX_RHWDTEX(CVERTEX_RHWDTEX &ct, D3DXVECTOR4 p, DWORD clr, float tu, float tv)							{	ct.pos=p;	ct.color=clr;	ct.tu=tu;	ct.tv=tv;	}
void SET_VTX_TEX(CVERTEX_TEX &ct, D3DXVECTOR3 p, float tu, float tv)											{	ct.pos=p;	ct.tu=tu;		ct.tv=tv;	}
void SET_VTX_NTEX(CVERTEX_NTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, float tu, float tv)							{	ct.pos=p;	ct.normal=n;	ct.tu=tu;	ct.tv=tv;	}
void SET_VTX_NTTEX(CVERTEX_NTTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, float tu1, float tv1, float tu2, float tv2)	{	ct.pos=p;	ct.normal=n;	ct.tu1=tu1;		ct.tv1=tv1;		ct.tu2=tu2;		ct.tv2=tv2;	}
void SET_VTX_NDTEX(CVERTEX_NDTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, DWORD c, float tu, float tv)				{	ct.pos=p;	ct.normal=n;	ct.diff=c;		ct.uv.x=tu;		ct.uv.y=tv;	}

//------------------------------------------------------------------
// 인덱스 구조체 채우기
//------------------------------------------------------------------
void SET_IDX(CINDEX &ci, WORD _0, WORD _1, WORD _2)	{	ci._0=_0; ci._1=_1;	ci._2=_2;	}
void SET_IDXL(CINDEXL &ci, WORD _0, WORD _1)		{	ci._0=_0; ci._1=_1;				}

//------------------------------------------------------------------
//정적 버텍스
//------------------------------------------------------------------
HRESULT Create_Vtx2D(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DTEXTURE9 &tex, int x, int y, int w, int h, WCHAR* str, DWORD dif, float tu0, float tv0, float tu1, float tv1)
{
	//텍스쳐 버텍스버퍼 생성.
	if(str && FAILED(D3DXCreateTextureFromFile(dev,str,&tex))) return E_FAIL;
	if(FAILED(dev->CreateVertexBuffer(sizeof(CVERTEX_RHWDTEX)*4, 0, CVERTEX_RHWDTEX::FVF, D3DPOOL_DEFAULT, &vb, NULL))) return E_FAIL;
	//버텍스 버퍼 초기화.
	CVERTEX_RHWDTEX* pv;
	if(FAILED(vb->Lock(0, 0, (void**)&pv, 0))) return E_FAIL;
	SET_VTX_RHWDTEX(pv[0],D3DXVECTOR4(float(x  )-0.5f,float(y  )-0.5f,0,1.0f),dif,tu0,tv0);
	SET_VTX_RHWDTEX(pv[1],D3DXVECTOR4(float(x+w)-0.5f,float(y  )-0.5f,0,1.0f),dif,tu1,tv0);
	SET_VTX_RHWDTEX(pv[2],D3DXVECTOR4(float(x  )-0.5f,float(y+h)-0.5f,0,1.0f),dif,tu0,tv1);
	SET_VTX_RHWDTEX(pv[3],D3DXVECTOR4(float(x+w)-0.5f,float(y+h)-0.5f,0,1.0f),dif,tu1,tv1);
	vb->Unlock();
	return S_OK;
}
void Render_Vtx2D(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DTEXTURE9 &tex)
{
	if(!vb) return;
	dev->SetTexture(0, (tex) ? tex : NULL);
	dev->SetStreamSource(0, vb, 0, sizeof(CVERTEX_RHWDTEX));
	dev->SetFVF(CVERTEX_RHWDTEX::FVF);
	dev->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
}


//------------------------------------------------------------------
//정적 버텍스 (쉐이더 전용)
//------------------------------------------------------------------
HRESULT Create_Shader_Quad(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, float size)
{
	if(FAILED(dev->CreateVertexBuffer(sizeof(CVERTEX_TEX)*4, 0, CVERTEX_TEX::FVF, D3DPOOL_DEFAULT, &vb, NULL))) return E_FAIL;
	//버텍스 버퍼 초기화.
	CVERTEX_TEX* pv;
	if(FAILED(vb->Lock(0, 0, (void**)&pv, 0))) return E_FAIL;
	SET_VTX_TEX(pv[0],D3DXVECTOR3(+1.f*size,-1.f*size,0),1,1);
	SET_VTX_TEX(pv[1],D3DXVECTOR3(-1.f*size,-1.f*size,0),0,1);
	SET_VTX_TEX(pv[2],D3DXVECTOR3(-1.f*size,+1.f*size,0),0,0);
	SET_VTX_TEX(pv[3],D3DXVECTOR3(+1.f*size,+1.f*size,0),1,0);
	vb->Unlock();
	//인덱스 버퍼
	if(FAILED(dev->CreateIndexBuffer(sizeof(CINDEX)*2, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL))) return E_FAIL;
	//인덱스 버퍼 초기화
	CINDEX* iv;
	if(FAILED(ib->Lock(0, 0, (void**)&iv, 0))) return E_FAIL;
	SET_IDX(iv[0],0,1,2);
	SET_IDX(iv[1],2,3,0);
	ib->Unlock();
	return S_OK;
}
void Render_Shader_Quad(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DVERTEXDECLARATION9 decl)
{
	if(!vb) return;
	dev->SetStreamSource(0, vb, 0, sizeof(CVERTEX_TEX));
	if(decl) dev->SetVertexDeclaration(decl);
	else	 dev->SetFVF(CVERTEX_TEX::FVF);
	dev->SetIndices(ib);
	dev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2); 
}

//------------------------------------------------------------------
//정적 인덱스
//------------------------------------------------------------------
HRESULT Create_Vtx2DI(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DTEXTURE9 &tex, int x, int y, int w, int h, WCHAR* str, DWORD dif, float tu0, float tv0, float tu1, float tv1)
{
	//텍스쳐 버텍스버퍼 생성.
	if(str && FAILED(D3DXCreateTextureFromFile(dev,str,&tex))) return E_FAIL;
	if(FAILED(dev->CreateVertexBuffer(sizeof(CVERTEX_RHWDTEX)*4, 0, CVERTEX_RHWDTEX::FVF, D3DPOOL_DEFAULT, &vb, NULL))) return E_FAIL;
	//버텍스 버퍼 초기화.
	CVERTEX_RHWDTEX* pv;
	if(FAILED(vb->Lock(0, 0, (void**)&pv, 0))) return E_FAIL;
	SET_VTX_RHWDTEX(pv[0],D3DXVECTOR4(float(x  )-0.5f,float(y  )-0.5f,0,1.0f),dif,tu0,tv0);
	SET_VTX_RHWDTEX(pv[1],D3DXVECTOR4(float(x+w)-0.5f,float(y  )-0.5f,0,1.0f),dif,tu1,tv0);
	SET_VTX_RHWDTEX(pv[2],D3DXVECTOR4(float(x  )-0.5f,float(y+h)-0.5f,0,1.0f),dif,tu0,tv1);
	SET_VTX_RHWDTEX(pv[3],D3DXVECTOR4(float(x+w)-0.5f,float(y+h)-0.5f,0,1.0f),dif,tu1,tv1);
	vb->Unlock();
	//인덱스 버퍼
	if(FAILED(dev->CreateIndexBuffer(sizeof(CINDEX)*2, 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &ib, NULL))) return E_FAIL;
	//인덱스 버퍼 초기화
	CINDEX* iv;
	if(FAILED(ib->Lock(0, 0, (void**)&iv, 0))) return E_FAIL;
	SET_IDX(iv[0],0,1,2);
	SET_IDX(iv[1],1,2,3);
	ib->Unlock();
	return S_OK;
}
void Render_Vtx2DI(LPDIRECT3DDEVICE9 dev, LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DTEXTURE9 &tex)
{
	if(!vb || !ib) return;
	dev->SetTexture(0, (tex) ? tex : NULL);
	dev->SetStreamSource(0, vb, 0, sizeof(CVERTEX_RHWDTEX));
	dev->SetFVF(CVERTEX_RHWDTEX::FVF);
	dev->SetIndices(ib);
	dev->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, 4, 0, 2); 
}

//------------------------------------------------------------------
//동적 버텍스 2D평면 그리기
//------------------------------------------------------------------
void Render_QuadUP(LPDIRECT3DDEVICE9 dev, int x, int y, int w, int h, LPDIRECT3DTEXTURE9 tex, float u0, float v0, float u1, float v1)
{
	static CVERTEX_RHWDTEX vts[4];
	SET_VTX_RHWDTEX(vts[0], D3DXVECTOR4 (float(x)  -0.5f, float(y)  -0.5f, 0.0f, 1.0f), 0xffffffff, u0, v0);
	SET_VTX_RHWDTEX(vts[1], D3DXVECTOR4 (float(x+w)-0.5f, float(y)  -0.5f, 0.0f, 1.0f), 0xffffffff, u1, v0);
	SET_VTX_RHWDTEX(vts[2], D3DXVECTOR4 (float(x)  -0.5f, float(y+h)-0.5f, 0.0f, 1.0f), 0xffffffff, u0, v1);
	SET_VTX_RHWDTEX(vts[3], D3DXVECTOR4 (float(x+w)-0.5f, float(y+h)-0.5f, 0.0f, 1.0f), 0xffffffff, u1, v1);

	if (tex) dev->SetTexture(0, tex);
	dev->SetFVF(CVERTEX_RHWDTEX::FVF);
	dev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vts, sizeof(CVERTEX_RHWDTEX));	
}

void Render_Line(LPDIRECT3DDEVICE9 dev, D3DXVECTOR3& src, D3DXVECTOR3& des, DWORD color)
{
	static CVERTEX_DIF vts[4];
	SET_VTX_DIF(vts[0], src, color);
	SET_VTX_DIF(vts[1], des, color);

	dev->SetTexture(0, NULL);
	dev->SetFVF(CVERTEX_DIF::FVF);
	dev->DrawPrimitiveUP(D3DPT_LINELIST, 1, vts, sizeof(CVERTEX_DIF));	
}


//------------------------------------------------------------------
// Draw Debug Text
//------------------------------------------------------------------
WCHAR* ctow(char* val, ...)
{
	static WCHAR wch[MAX_PATH];
	static char  ch[MAX_PATH];
	va_list list;
	va_start(list,val);
	vsprintf_s(ch,val,list);
	va_end(list);
	MultiByteToWideChar(CP_ACP, 0, ch, -1, wch, sizeof(char)*MAX_PATH);
	return wch;
}
//------------------------------------------------------------------
void DrawPrintf(ID3DXFont *font, WCHAR* str, int x, int y, int w, int h, int bold, DWORD color, DWORD flags)
{
	if(!font) return;
	RECT rc;	SetRect(&rc,x,y,x+w,y+h);
	//글씨를 두껍게...
	if(bold>0)
	{
		RECT tc = rc;
		for(int i=-bold; i<=bold; ++i) {
			for(int j=-bold; j<=bold; j++) {
				if(i==0 && j==0) continue;
				tc.left += i;
				tc.top  += j;
				font->DrawTextW(NULL, str, -1, &tc, flags, 0xff000000);
				tc.left = rc.left;
				tc.top  = rc.top;
			}
		}
	}
	font->DrawTextW(NULL, str, -1, &rc, flags, color);
}
//------------------------------------------------------------------
void MsgBox(char* str, ...)
{
	char  ch[MAX_PATH];
	va_list list;
	va_start(list,str);
	vsprintf_s(ch,str,list);
	va_end(list);
	MessageBoxA(NULL, ch, "", MB_OK);
}
//------------------------------------------------------------------
void MsgBox(WCHAR* str, ...)
{
	WCHAR wch[MAX_PATH];
	va_list list;
	va_start(list,str);
	 StringCbVPrintf(wch,MAX_PATH, str, list);
	
	va_end(list);
	MessageBox(NULL, wch, L"", MB_OK);
}

//--------------------------------------------------------------------------------------
// Global/Static Members
//--------------------------------------------------------------------------------------
CDXUTResourceCache& WINAPI DXUTGetGlobalResourceCache()
{
    // Using an accessor function gives control of the construction order
    static CDXUTResourceCache cache;
    return cache;
}


//--------------------------------------------------------------------------------------
// Internal functions forward declarations
//--------------------------------------------------------------------------------------
bool DXUTFindMediaSearchTypicalDirs( WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf, WCHAR* strExePath, WCHAR* strExeName );
bool DXUTFindMediaSearchParentDirs( WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt, WCHAR* strLeafName );
INT_PTR CALLBACK DisplaySwitchToREFWarningProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


//--------------------------------------------------------------------------------------
// Shared code for samples to ask user if they want to use a REF device or quit
//--------------------------------------------------------------------------------------
void DXUTDisplaySwitchingToREFWarning( DXUTDeviceVersion ver )
{
    if( DXUTGetShowMsgBoxOnError() )
    {
        DWORD dwSkipWarning = 0, dwRead = 0, dwWritten = 0;
        HANDLE hFile = NULL;

        // Read previous user settings
        WCHAR strPath[MAX_PATH];
        SHGetFolderPath( DXUTGetHWND(), CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, strPath );

        wcscpy_s( strPath, MAX_PATH, L"\\DXUT\\SkipRefWarning.dat" );
        if( (hFile = CreateFile( strPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL )) != INVALID_HANDLE_VALUE )
        {
            ReadFile( hFile, &dwSkipWarning, sizeof(DWORD), &dwRead, NULL );
            CloseHandle( hFile );
        }

        if( dwSkipWarning == 0 )
        {
            // Compact code to create a custom dialog box without using a template in a resource file.
            // If this dialog were in a .rc file, this would be a lot simpler but every sample calling this function would
            // need a copy of the dialog in its own .rc file. Also MessageBox API could be used here instead, but 
            // the MessageBox API is simpler to call but it can't provide a "Don't show again" checkbox
            typedef struct { DLGITEMTEMPLATE a; WORD b; WORD c; WORD d; WORD e; WORD f; } DXUT_DLG_ITEM; 
            typedef struct { DLGTEMPLATE a; WORD b; WORD c; WCHAR d[2]; WORD e; WCHAR f[16]; DXUT_DLG_ITEM i1; DXUT_DLG_ITEM i2; DXUT_DLG_ITEM i3; DXUT_DLG_ITEM i4; DXUT_DLG_ITEM i5; } DXUT_DLG_DATA; 

            DXUT_DLG_DATA dtp = 
            {                                                                                                                                                  
                {WS_CAPTION|WS_POPUP|WS_VISIBLE|WS_SYSMENU|DS_ABSALIGN|DS_3DLOOK|DS_SETFONT|DS_MODALFRAME|DS_CENTER,0,5,0,0,269,82},0,0,L" ",8,L"MS Shell Dlg 2", 
                {{WS_CHILD|WS_VISIBLE|SS_ICON|SS_CENTERIMAGE,0,7,7,24,24,0x100},0xFFFF,0x0082,0,0,0}, // icon
                {{WS_CHILD|WS_VISIBLE,0,40,7,230,25,0x101},0xFFFF,0x0082,0,0,0}, // static text
                {{WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_DEFPUSHBUTTON,0,80,39,50,14,IDYES},0xFFFF,0x0080,0,0,0}, // Yes button
                {{WS_CHILD|WS_VISIBLE|WS_TABSTOP,0,133,39,50,14,IDNO},0xFFFF,0x0080,0,0,0}, // No button
                {{WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_CHECKBOX,0,7,59,70,16,IDIGNORE},0xFFFF,0x0080,0,0,0}, // checkbox
            }; 

            int nResult = (int) DialogBoxIndirectParam( DXUTGetHINSTANCE(), (DLGTEMPLATE*)&dtp, DXUTGetHWND(), DisplaySwitchToREFWarningProc, (LPARAM) (ver==DXUT_D3D9_DEVICE) ? 9 : 10 );

            if( (nResult & 0x80) == 0x80 ) // "Don't show again" checkbox was checked
            {
                // Save user settings
                dwSkipWarning = 1;
                SHGetFolderPath( DXUTGetHWND(), CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_CURRENT, strPath );
                wcscpy_s( strPath, MAX_PATH, L"\\DXUT" );
                CreateDirectory( strPath, NULL );
                wcscpy_s( strPath, MAX_PATH, L"\\SkipRefWarning.dat" );
                if( (hFile = CreateFile( strPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL )) != INVALID_HANDLE_VALUE )
                {
                    WriteFile( hFile, &dwSkipWarning, sizeof(DWORD), &dwWritten, NULL );
                    CloseHandle( hFile );
                }
            }

            // User choose not to continue
            if( (nResult & 0x0F) == IDNO )
                DXUTShutdown(1);
        }
    }
}


//--------------------------------------------------------------------------------------
// MsgProc for DXUTDisplaySwitchingToREFWarning() dialog box
//--------------------------------------------------------------------------------------
INT_PTR CALLBACK DisplaySwitchToREFWarningProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    { 
        case WM_INITDIALOG:
            // Easier to set text here than in the DLGITEMTEMPLATE
            SetWindowText( hDlg, DXUTGetWindowTitle() );
            SendMessage( GetDlgItem(hDlg, 0x100), STM_SETIMAGE, IMAGE_ICON, (LPARAM)LoadIcon(0, IDI_QUESTION));
            WCHAR sz[512]; StringCchPrintf( sz, 512, L"This program needs to use the Direct3D %d reference device.  This device implements the entire Direct3D %d feature set, but runs very slowly.  Do you wish to continue?", lParam, lParam );
            SetDlgItemText( hDlg, 0x101, sz ); 
            SetDlgItemText( hDlg, IDYES, L"&Yes" );
            SetDlgItemText( hDlg, IDNO, L"&No" );
            SetDlgItemText( hDlg, IDIGNORE, L"&Don't show again" );
            break;

        case WM_COMMAND: 
            switch (LOWORD(wParam)) 
            { 
                case IDIGNORE: CheckDlgButton( hDlg, IDIGNORE, (IsDlgButtonChecked( hDlg, IDIGNORE ) == BST_CHECKED) ? BST_UNCHECKED : BST_CHECKED ); EnableWindow( GetDlgItem( hDlg, IDNO ), (IsDlgButtonChecked( hDlg, IDIGNORE ) != BST_CHECKED) ); break;
                case IDNO: EndDialog(hDlg, (IsDlgButtonChecked( hDlg, IDIGNORE ) == BST_CHECKED) ? IDNO|0x80 : IDNO|0x00 ); return TRUE; 
                case IDCANCEL:
                case IDYES: EndDialog(hDlg, (IsDlgButtonChecked( hDlg, IDIGNORE ) == BST_CHECKED) ? IDYES|0x80 : IDYES|0x00 ); return TRUE; 
            } 
            break;
    } 
    return FALSE; 
} 


//--------------------------------------------------------------------------------------
// Returns pointer to static media search buffer
//--------------------------------------------------------------------------------------
WCHAR* DXUTMediaSearchPath()
{
    static WCHAR s_strMediaSearchPath[MAX_PATH] = {0};
    return s_strMediaSearchPath;

}   

//--------------------------------------------------------------------------------------
LPCWSTR WINAPI DXUTGetMediaSearchPath()
{
    return DXUTMediaSearchPath();
}


//--------------------------------------------------------------------------------------
HRESULT WINAPI DXUTSetMediaSearchPath( LPCWSTR strPath )
{
    HRESULT hr;

    WCHAR* s_strSearchPath = DXUTMediaSearchPath();

    hr = StringCchCopy( s_strSearchPath, MAX_PATH, strPath );   
    if( SUCCEEDED(hr) )
    {
        // append slash if needed
        size_t ch;
        hr = StringCchLength( s_strSearchPath, MAX_PATH, &ch );
        if( SUCCEEDED(hr) && s_strSearchPath[ch-1] != L'\\')
        {
            hr = wcscpy_s( s_strSearchPath, MAX_PATH, L"\\" );
        }
    }

    return hr;
}


//--------------------------------------------------------------------------------------
// Tries to find the location of a SDK media file
//       cchDest is the size in WCHARs of strDestPath.  Be careful not to 
//       pass in sizeof(strDest) on UNICODE builds.
//--------------------------------------------------------------------------------------
HRESULT WINAPI DXUTFindDXSDKMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename )
{
    bool bFound;
    WCHAR strSearchFor[MAX_PATH];
    
    if( NULL==strFilename || strFilename[0] == 0 || NULL==strDestPath || cchDest < 10 )
        return E_INVALIDARG;

    // Get the exe name, and exe path
    WCHAR strExePath[MAX_PATH] = {0};
    WCHAR strExeName[MAX_PATH] = {0};
    WCHAR* strLastSlash = NULL;
    GetModuleFileName( NULL, strExePath, MAX_PATH );
    strExePath[MAX_PATH-1]=0;
    strLastSlash = wcsrchr( strExePath, TEXT('\\') );
    if( strLastSlash )
    {
        StringCchCopy( strExeName, MAX_PATH, &strLastSlash[1] );

        // Chop the exe name from the exe path
        *strLastSlash = 0;

        // Chop the .exe from the exe name
        strLastSlash = wcsrchr( strExeName, TEXT('.') );
        if( strLastSlash )
            *strLastSlash = 0;
    }

    // Typical directories:
    //      .\
    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
    //      %EXE_DIR%\..\..\%EXE_NAME%

    // Typical directory search
    bFound = DXUTFindMediaSearchTypicalDirs( strDestPath, cchDest, strFilename, strExePath, strExeName );
    if( bFound )
        return S_OK;

    // Typical directory search again, but also look in a subdir called "\media\" 
    StringCchPrintf( strSearchFor, MAX_PATH, L"media\\%s", strFilename ); 
    bFound = DXUTFindMediaSearchTypicalDirs( strDestPath, cchDest, strSearchFor, strExePath, strExeName );
    if( bFound )
        return S_OK;

    WCHAR strLeafName[MAX_PATH] = {0};

    // Search all parent directories starting at .\ and using strFilename as the leaf name
    StringCchCopy( strLeafName, MAX_PATH, strFilename ); 
    bFound = DXUTFindMediaSearchParentDirs( strDestPath, cchDest, L".", strLeafName );
    if( bFound )
        return S_OK;

    // Search all parent directories starting at the exe's dir and using strFilename as the leaf name
    bFound = DXUTFindMediaSearchParentDirs( strDestPath, cchDest, strExePath, strLeafName );
    if( bFound )
        return S_OK;

    // Search all parent directories starting at .\ and using "media\strFilename" as the leaf name
    StringCchPrintf( strLeafName, MAX_PATH, L"media\\%s", strFilename ); 
    bFound = DXUTFindMediaSearchParentDirs( strDestPath, cchDest, L".", strLeafName );
    if( bFound )
        return S_OK;

    // Search all parent directories starting at the exe's dir and using "media\strFilename" as the leaf name
    bFound = DXUTFindMediaSearchParentDirs( strDestPath, cchDest, strExePath, strLeafName );
    if( bFound )
        return S_OK;

    // On failure, return the file as the path but also return an error code
    StringCchCopy( strDestPath, cchDest, strFilename );

    return DXUTERR_MEDIANOTFOUND;
}


//--------------------------------------------------------------------------------------
// Search a set of typical directories
//--------------------------------------------------------------------------------------
bool DXUTFindMediaSearchTypicalDirs( WCHAR* strSearchPath, int cchSearch, LPCWSTR strLeaf, 
                                     WCHAR* strExePath, WCHAR* strExeName )
{
    // Typical directories:
    //      .\
    //      ..\
    //      ..\..\
    //      %EXE_DIR%\
    //      %EXE_DIR%\..\
    //      %EXE_DIR%\..\..\
    //      %EXE_DIR%\..\%EXE_NAME%
    //      %EXE_DIR%\..\..\%EXE_NAME%
    //      DXSDK media path

    // Search in .\  
    StringCchCopy( strSearchPath, cchSearch, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\  
    StringCchPrintf( strSearchPath, cchSearch, L"..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, L"..\\..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in ..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, L"..\\..\\%s", strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\ 
    StringCchPrintf( strSearchPath, cchSearch, L"%s\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\..\ 
    StringCchPrintf( strSearchPath, cchSearch, L"%s\\..\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in the %EXE_DIR%\..\..\ 
    StringCchPrintf( strSearchPath, cchSearch, L"%s\\..\\..\\%s", strExePath, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in "%EXE_DIR%\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintf( strSearchPath, cchSearch, L"%s\\..\\%s\\%s", strExePath, strExeName, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in "%EXE_DIR%\..\..\%EXE_NAME%\".  This matches the DirectX SDK layout
    StringCchPrintf( strSearchPath, cchSearch, L"%s\\..\\..\\%s\\%s", strExePath, strExeName, strLeaf ); 
    if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
        return true;

    // Search in media search dir 
    WCHAR* s_strSearchPath = DXUTMediaSearchPath();
    if( s_strSearchPath[0] != 0 )
    {
        StringCchPrintf( strSearchPath, cchSearch, L"%s%s", s_strSearchPath, strLeaf ); 
        if( GetFileAttributes( strSearchPath ) != 0xFFFFFFFF )
            return true;
    }

    return false;
}



//--------------------------------------------------------------------------------------
// Search parent directories starting at strStartAt, and appending strLeafName
// at each parent directory.  It stops at the root directory.
//--------------------------------------------------------------------------------------
bool DXUTFindMediaSearchParentDirs( WCHAR* strSearchPath, int cchSearch, WCHAR* strStartAt, WCHAR* strLeafName )
{
    WCHAR strFullPath[MAX_PATH] = {0};
    WCHAR strFullFileName[MAX_PATH] = {0};
    WCHAR strSearch[MAX_PATH] = {0};
    WCHAR* strFilePart = NULL;

    GetFullPathName( strStartAt, MAX_PATH, strFullPath, &strFilePart );
    if( strFilePart == NULL )
        return false;
   
    while( strFilePart != NULL && *strFilePart != '\0' )
    {
        StringCchPrintf( strFullFileName, MAX_PATH, L"%s\\%s", strFullPath, strLeafName ); 
        if( GetFileAttributes( strFullFileName ) != 0xFFFFFFFF )
        {
            StringCchCopy( strSearchPath, cchSearch, strFullFileName ); 
            return true;
        }

        StringCchPrintf( strSearch, MAX_PATH, L"%s\\..", strFullPath ); 
        GetFullPathName( strSearch, MAX_PATH, strFullPath, &strFilePart );
    }

    return false;
}


//--------------------------------------------------------------------------------------
// CDXUTResourceCache
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
CDXUTResourceCache::~CDXUTResourceCache()
{
    OnDestroyDevice();

    m_TextureCache.RemoveAll();
    m_EffectCache.RemoveAll();
    m_FontCache.RemoveAll();
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DTEXTURE9 *ppTexture )
{
    return CreateTextureFromFileEx( pDevice, pSrcFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                                    0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                    0, NULL, NULL, ppTexture );
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *ppTexture )
{
    WCHAR szSrcFile[MAX_PATH];
    MultiByteToWideChar( CP_ACP, 0, pSrcFile, -1, szSrcFile, MAX_PATH );
    szSrcFile[MAX_PATH-1] = 0;

    return CreateTextureFromFile( pDevice, szSrcFile, ppTexture );
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_FILE &&
            !lstrcmpW( Entry.wszSource, pSrcFile ) &&
            Entry.Width == Width &&
            Entry.Height == Height &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_TEXTURE )
        {
            // A match is found. Obtain the IDirect3DTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DTexture9, (LPVOID*)ppTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateTextureFromFileEx( pDevice, pSrcFile, Width, Height, MipLevels, Usage, Format,
                                      Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_FILE;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcFile );
    NewEntry.Width = Width;
    NewEntry.Height = Height;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_TEXTURE;
    (*ppTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}

//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DTEXTURE9 *ppTexture )
{
    return CreateTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, D3DX_DEFAULT, D3DX_DEFAULT,
                                        D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
                                        D3DX_DEFAULT, 0, NULL, NULL, ppTexture );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_RESOURCE &&
            Entry.hSrcModule == hSrcModule &&
            !lstrcmpW( Entry.wszSource, pSrcResource ) &&
            Entry.Width == Width &&
            Entry.Height == Height &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_TEXTURE )
        {
            // A match is found. Obtain the IDirect3DTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DTexture9, (LPVOID*)ppTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, Width, Height, MipLevels, Usage,
                                          Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_RESOURCE;
    NewEntry.hSrcModule = hSrcModule;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcResource );
    NewEntry.Width = Width;
    NewEntry.Height = Height;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_TEXTURE;
    (*ppTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateCubeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture )
{
    return CreateCubeTextureFromFileEx( pDevice, pSrcFile, D3DX_DEFAULT, D3DX_DEFAULT, 0,
                                        D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                        0, NULL, NULL, ppCubeTexture );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateCubeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_FILE &&
            !lstrcmpW( Entry.wszSource, pSrcFile ) &&
            Entry.Width == Size &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_CUBETEXTURE )
        {
            // A match is found. Obtain the IDirect3DCubeTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DCubeTexture9, (LPVOID*)ppCubeTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateCubeTextureFromFileEx( pDevice, pSrcFile, Size, MipLevels, Usage, Format, Pool, Filter,
                                          MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_FILE;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcFile );
    NewEntry.Width = Size;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_CUBETEXTURE;
    (*ppCubeTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateCubeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture )
{
    return CreateCubeTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, D3DX_DEFAULT, D3DX_DEFAULT,
                                            0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                            0, NULL, NULL, ppCubeTexture );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateCubeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_RESOURCE &&
            Entry.hSrcModule == hSrcModule &&
            !lstrcmpW( Entry.wszSource, pSrcResource ) &&
            Entry.Width == Size &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_CUBETEXTURE )
        {
            // A match is found. Obtain the IDirect3DCubeTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DCubeTexture9, (LPVOID*)ppCubeTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateCubeTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, Size, MipLevels, Usage, Format,
                                              Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppCubeTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_RESOURCE;
    NewEntry.hSrcModule = hSrcModule;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcResource );
    NewEntry.Width = Size;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_CUBETEXTURE;
    (*ppCubeTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateVolumeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture )
{
    return CreateVolumeTextureFromFileEx( pDevice, pSrcFile, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
                                          0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT,
                                          0, NULL, NULL, ppVolumeTexture );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateVolumeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_FILE &&
            !lstrcmpW( Entry.wszSource, pSrcFile ) &&
            Entry.Width == Width &&
            Entry.Height == Height &&
            Entry.Depth == Depth &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_VOLUMETEXTURE )
        {
            // A match is found. Obtain the IDirect3DVolumeTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DVolumeTexture9, (LPVOID*)ppTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateVolumeTextureFromFileEx( pDevice, pSrcFile, Width, Height, Depth, MipLevels, Usage, Format,
                                            Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_FILE;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcFile );
    NewEntry.Width = Width;
    NewEntry.Height = Height;
    NewEntry.Depth = Depth;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_VOLUMETEXTURE;
    (*ppTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateVolumeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture )
{
    return CreateVolumeTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, D3DX_DEFAULT, D3DX_DEFAULT,
                                              D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED,
                                              D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppVolumeTexture );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateVolumeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_TextureCache.GetSize(); ++i )
    {
        DXUTCache_Texture &Entry = m_TextureCache[i];
        if( Entry.Location == DXUTCACHE_LOCATION_RESOURCE &&
            Entry.hSrcModule == hSrcModule &&
            !lstrcmpW( Entry.wszSource, pSrcResource ) &&
            Entry.Width == Width &&
            Entry.Height == Height &&
            Entry.Depth == Depth &&
            Entry.MipLevels == MipLevels &&
            Entry.Usage9 == Usage &&
            Entry.Format9 == Format &&
            Entry.Pool9 == Pool &&
            Entry.Type9 == D3DRTYPE_VOLUMETEXTURE )
        {
            // A match is found. Obtain the IDirect3DVolumeTexture9 interface and return that.
            return Entry.pTexture9->QueryInterface( IID_IDirect3DVolumeTexture9, (LPVOID*)ppVolumeTexture );
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateVolumeTextureFromResourceEx( pDevice, hSrcModule, pSrcResource, Width, Height, Depth, MipLevels, Usage,
                                                Format, Pool, Filter, MipFilter, ColorKey, pSrcInfo, pPalette, ppVolumeTexture );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Texture NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_RESOURCE;
    NewEntry.hSrcModule = hSrcModule;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcResource );
    NewEntry.Width = Width;
    NewEntry.Height = Height;
    NewEntry.Depth = Depth;
    NewEntry.MipLevels = MipLevels;
    NewEntry.Usage9 = Usage;
    NewEntry.Format9 = Format;
    NewEntry.Pool9 = Pool;
    NewEntry.Type9 = D3DRTYPE_VOLUMETEXTURE;
    (*ppVolumeTexture)->QueryInterface( IID_IDirect3DBaseTexture9, (LPVOID*)&NewEntry.pTexture9 );

    m_TextureCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateFont( LPDIRECT3DDEVICE9 pDevice, UINT Height, UINT Width, UINT Weight, UINT MipLevels, bool Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCTSTR pFacename, LPD3DXFONT *ppFont )
{
    D3DXFONT_DESCW Desc;
    
    Desc.Height = Height;
    Desc.Width = Width;
    Desc.Weight = Weight;
    Desc.MipLevels = MipLevels;
    Desc.Italic = Italic;
    Desc.CharSet = (BYTE)CharSet;
    Desc.OutputPrecision = (BYTE)OutputPrecision;
    Desc.Quality = (BYTE)Quality;
    Desc.PitchAndFamily = (BYTE)PitchAndFamily;
    StringCchCopy( Desc.FaceName, LF_FACESIZE, pFacename );

    return CreateFontIndirect( pDevice, &Desc, ppFont );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC *pDesc, LPD3DXFONT *ppFont )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_FontCache.GetSize(); ++i )
    {
        DXUTCache_Font &Entry = m_FontCache[i];

        if( Entry.Width == pDesc->Width &&
            Entry.Height == pDesc->Height &&
            Entry.Weight == pDesc->Weight &&
            Entry.MipLevels == pDesc->MipLevels &&
            Entry.Italic == pDesc->Italic &&
            Entry.CharSet == pDesc->CharSet &&
            Entry.OutputPrecision == pDesc->OutputPrecision &&
            Entry.Quality == pDesc->Quality &&
            Entry.PitchAndFamily == pDesc->PitchAndFamily &&
            CompareString( LOCALE_USER_DEFAULT, NORM_IGNORECASE,
                           Entry.FaceName, -1,
                           pDesc->FaceName, -1 ) == CSTR_EQUAL )
        {
            // A match is found.  Increment the reference and return the ID3DXFont object.
            Entry.pFont->AddRef();
            *ppFont = Entry.pFont;
            return S_OK;
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateFontIndirect( pDevice, pDesc, ppFont );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Font NewEntry;
    (D3DXFONT_DESC &)NewEntry = *pDesc;
    NewEntry.pFont = *ppFont;
    NewEntry.pFont->AddRef();

    m_FontCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateEffectFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_EffectCache.GetSize(); ++i )
    {
        DXUTCache_Effect &Entry = m_EffectCache[i];

        if( Entry.Location == DXUTCACHE_LOCATION_FILE &&
            !lstrcmpW( Entry.wszSource, pSrcFile ) &&
            Entry.dwFlags == Flags )
        {
            // A match is found.  Increment the ref coutn and return the ID3DXEffect object.
            *ppEffect = Entry.pEffect;
            (*ppEffect)->AddRef();
            return S_OK;
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateEffectFromFile( pDevice, pSrcFile, pDefines, pInclude, Flags, pPool, ppEffect, ppCompilationErrors );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Effect NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_FILE;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcFile );
    NewEntry.dwFlags = Flags;
    NewEntry.pEffect = *ppEffect;
    NewEntry.pEffect->AddRef();

    m_EffectCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::CreateEffectFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors )
{
    // Search the cache for a matching entry.
    for( int i = 0; i < m_EffectCache.GetSize(); ++i )
    {
        DXUTCache_Effect &Entry = m_EffectCache[i];

        if( Entry.Location == DXUTCACHE_LOCATION_RESOURCE &&
            Entry.hSrcModule == hSrcModule &&
            !lstrcmpW( Entry.wszSource, pSrcResource ) &&
            Entry.dwFlags == Flags )
        {
            // A match is found.  Increment the ref coutn and return the ID3DXEffect object.
            *ppEffect = Entry.pEffect;
            (*ppEffect)->AddRef();
            return S_OK;
        }
    }

    HRESULT hr;

    // No matching entry.  Load the resource and create a new entry.
    hr = D3DXCreateEffectFromResource( pDevice, hSrcModule, pSrcResource, pDefines, pInclude, Flags,
                                       pPool, ppEffect, ppCompilationErrors );
    if( FAILED( hr ) )
        return hr;

    DXUTCache_Effect NewEntry;
    NewEntry.Location = DXUTCACHE_LOCATION_RESOURCE;
    NewEntry.hSrcModule = hSrcModule;
    StringCchCopy( NewEntry.wszSource, MAX_PATH, pSrcResource );
    NewEntry.dwFlags = Flags;
    NewEntry.pEffect = *ppEffect;
    NewEntry.pEffect->AddRef();

    m_EffectCache.Add( NewEntry );
    return S_OK;
}


//--------------------------------------------------------------------------------------
// Device event callbacks
//--------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::OnCreateDevice( IDirect3DDevice9 *pd3dDevice )
{
    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::OnResetDevice( IDirect3DDevice9 *pd3dDevice )
{
    // Call OnResetDevice on all effect and font objects
    for( int i = 0; i < m_EffectCache.GetSize(); ++i )
        m_EffectCache[i].pEffect->OnResetDevice();
    for( int i = 0; i < m_FontCache.GetSize(); ++i )
        m_FontCache[i].pFont->OnResetDevice();


    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::OnLostDevice()
{
    // Call OnLostDevice on all effect and font objects
    for( int i = 0; i < m_EffectCache.GetSize(); ++i )
        m_EffectCache[i].pEffect->OnLostDevice();
    for( int i = 0; i < m_FontCache.GetSize(); ++i )
        m_FontCache[i].pFont->OnLostDevice();

    // Release all the default pool textures
    for( int i = m_TextureCache.GetSize() - 1; i >= 0; --i )
        if( m_TextureCache[i].Pool9 == D3DPOOL_DEFAULT )
        {
            SAFE_RELEASE( m_TextureCache[i].pTexture9 );
            m_TextureCache.Remove( i );  // Remove the entry
        }

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTResourceCache::OnDestroyDevice()
{
    // Release all resources
    for( int i = m_EffectCache.GetSize() - 1; i >= 0; --i )
    {
        SAFE_RELEASE( m_EffectCache[i].pEffect );
        m_EffectCache.Remove( i );
    }
    for( int i = m_FontCache.GetSize() - 1; i >= 0; --i )
    {
        SAFE_RELEASE( m_FontCache[i].pFont );
        m_FontCache.Remove( i );
    }
    for( int i = m_TextureCache.GetSize() - 1; i >= 0; --i )
    {
        SAFE_RELEASE( m_TextureCache[i].pTexture9 );
        m_TextureCache.Remove( i );
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
// Desc: Returns a view matrix for rendering to a face of a cubemap.
//--------------------------------------------------------------------------------------
D3DXMATRIX WINAPI DXUTGetCubeMapViewMatrix( DWORD dwFace )
{
    D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    switch( dwFace )
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
    }

    // Set the view transform for this cubemap surface
    D3DXMATRIXA16 mView;
    D3DXMatrixLookAtLH( &mView, &vEyePt, &vLookDir, &vUpDir );
    return mView;
}


//--------------------------------------------------------------------------------------
CDXUTLineManager::CDXUTLineManager()
{
    m_pd3dDevice = NULL;
    m_pD3DXLine = NULL;
}


//--------------------------------------------------------------------------------------
CDXUTLineManager::~CDXUTLineManager()
{
    OnDeletedDevice();
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::OnCreatedDevice( IDirect3DDevice9* pd3dDevice )
{
    m_pd3dDevice = pd3dDevice;

    HRESULT hr;
    hr = D3DXCreateLine( m_pd3dDevice, &m_pD3DXLine );
    if( FAILED(hr) )
        return hr;

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::OnResetDevice()
{
    if( m_pD3DXLine )
        m_pD3DXLine->OnResetDevice();

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::OnRender()
{
    HRESULT hr;
    if( NULL == m_pD3DXLine )
        return E_INVALIDARG;

    bool bDrawingHasBegun = false;
    float fLastWidth = 0.0f;
    bool bLastAntiAlias = false;
    
    for( int i=0; i<m_LinesList.GetSize(); i++ )
    {
        LINE_NODE* pLineNode = m_LinesList.GetAt(i);
        if( pLineNode )
        {
            if( !bDrawingHasBegun || 
                fLastWidth != pLineNode->fWidth || 
                bLastAntiAlias != pLineNode->bAntiAlias )
            {
                if( bDrawingHasBegun )
                {
                    hr = m_pD3DXLine->End();
                    if( FAILED(hr) )
                        return hr;
                }

                m_pD3DXLine->SetWidth( pLineNode->fWidth );
                m_pD3DXLine->SetAntialias( pLineNode->bAntiAlias );

                fLastWidth = pLineNode->fWidth;
                bLastAntiAlias = pLineNode->bAntiAlias;

                hr = m_pD3DXLine->Begin();
                if( FAILED(hr) )
                    return hr;
                bDrawingHasBegun = true;
            }

            hr = m_pD3DXLine->Draw( pLineNode->pVertexList, pLineNode->dwVertexListCount, pLineNode->Color );
            if( FAILED(hr) )
                return hr;
        }
    }

    if( bDrawingHasBegun )
    {
        hr = m_pD3DXLine->End();
        if( FAILED(hr) )
            return hr;
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::OnLostDevice()
{
    if( m_pD3DXLine )
        m_pD3DXLine->OnLostDevice();

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::OnDeletedDevice()
{
    RemoveAllLines();
    SAFE_RELEASE( m_pD3DXLine );

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::AddLine( int* pnLineID, D3DXVECTOR2* pVertexList, DWORD dwVertexListCount, D3DCOLOR Color, float fWidth, float fScaleRatio, bool bAntiAlias )
{
    if( pVertexList == NULL || dwVertexListCount == 0 )
        return E_INVALIDARG;

    LINE_NODE* pLineNode = new LINE_NODE;
    if( pLineNode == NULL )
        return E_OUTOFMEMORY;
    ZeroMemory( pLineNode, sizeof(LINE_NODE) );

    pLineNode->nLineID = m_LinesList.GetSize();
    pLineNode->Color = Color;
    pLineNode->fWidth = fWidth;
    pLineNode->bAntiAlias = bAntiAlias;
    pLineNode->dwVertexListCount = dwVertexListCount;

    if( pnLineID )
        *pnLineID = pLineNode->nLineID;

    pLineNode->pVertexList = new D3DXVECTOR2[dwVertexListCount];
    if( pLineNode->pVertexList == NULL )
    {
        delete pLineNode;
        return E_OUTOFMEMORY;
    }
    for( DWORD i=0; i<dwVertexListCount; i++ )
    {
        pLineNode->pVertexList[i] = pVertexList[i] * fScaleRatio;
    }

    m_LinesList.Add( pLineNode );

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::AddRect( int* pnLineID, RECT rc, D3DCOLOR Color, float fWidth, float fScaleRatio, bool bAntiAlias )
{
    if( fWidth > 2.0f )
    {
        D3DXVECTOR2 vertexList[8];

        vertexList[0].x = (float)rc.left;
        vertexList[0].y = (float)rc.top - (fWidth/2.0f);

        vertexList[1].x = (float)rc.left;
        vertexList[1].y = (float)rc.bottom + (fWidth/2.0f);

        vertexList[2].x = (float)rc.left;
        vertexList[2].y = (float)rc.bottom - 0.5f;

        vertexList[3].x = (float)rc.right;
        vertexList[3].y = (float)rc.bottom - 0.5f;

        vertexList[4].x = (float)rc.right;
        vertexList[4].y = (float)rc.bottom + (fWidth/2.0f);

        vertexList[5].x = (float)rc.right;
        vertexList[5].y = (float)rc.top - (fWidth/2.0f);

        vertexList[6].x = (float)rc.right;
        vertexList[6].y = (float)rc.top;

        vertexList[7].x = (float)rc.left;
        vertexList[7].y = (float)rc.top;
        
        return AddLine( pnLineID, vertexList, 8, Color, fWidth, fScaleRatio, bAntiAlias );
    }
    else
    {
        D3DXVECTOR2 vertexList[5];
        vertexList[0].x = (float)rc.left;
        vertexList[0].y = (float)rc.top;

        vertexList[1].x = (float)rc.left;
        vertexList[1].y = (float)rc.bottom;

        vertexList[2].x = (float)rc.right;
        vertexList[2].y = (float)rc.bottom;

        vertexList[3].x = (float)rc.right;
        vertexList[3].y = (float)rc.top;

        vertexList[4].x = (float)rc.left;
        vertexList[4].y = (float)rc.top;
        
        return AddLine( pnLineID, vertexList, 5, Color, fWidth, fScaleRatio, bAntiAlias );
    }
}



//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::RemoveLine( int nLineID )
{
    for( int i=0; i<m_LinesList.GetSize(); i++ )
    {
        LINE_NODE* pLineNode = m_LinesList.GetAt(i);
        if( pLineNode && pLineNode->nLineID == nLineID )
        {
            SAFE_DELETE_ARRAY( pLineNode->pVertexList );
            delete pLineNode;
            m_LinesList.SetAt(i, NULL);
        }
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTLineManager::RemoveAllLines()
{
    for( int i=0; i<m_LinesList.GetSize(); i++ )
    {
        LINE_NODE* pLineNode = m_LinesList.GetAt(i);
        if( pLineNode )
        {
            SAFE_DELETE_ARRAY( pLineNode->pVertexList );
            delete pLineNode;
        }
    }
    m_LinesList.RemoveAll();

    return S_OK;
}


//--------------------------------------------------------------------------------------
CDXUTTextHelper::CDXUTTextHelper( ID3DXFont* pFont, ID3DXSprite* pSprite, int nLineHeight )     { Init( pFont, pSprite, nLineHeight ); }
CDXUTTextHelper::~CDXUTTextHelper()
{
}

//--------------------------------------------------------------------------------------
void CDXUTTextHelper::Init( ID3DXFont* pFont9, ID3DXSprite* pSprite9, int nLineHeight )
{
    m_pFont9 = pFont9;
    m_pSprite9 = pSprite9;
    m_clr = D3DXCOLOR(1,1,1,1);
    m_pt.x = 0; 
    m_pt.y = 0; 
    m_nLineHeight = nLineHeight;
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTTextHelper::DrawFormattedTextLine( const WCHAR* strMsg, ... )
{
    WCHAR strBuffer[512];
    
    va_list args;
    va_start(args, strMsg);
    StringCchVPrintf( strBuffer, 512, strMsg, args );
    strBuffer[511] = L'\0';
    va_end(args);

    return DrawTextLine( strBuffer );
}


//--------------------------------------------------------------------------------------
HRESULT CDXUTTextHelper::DrawTextLine( const WCHAR* strMsg )
{
    if( NULL == m_pFont9 ) 
        return DXUT_ERR_MSGBOX( L"DrawTextLine", E_INVALIDARG );

    HRESULT hr;
    RECT rc;
    SetRect( &rc, m_pt.x, m_pt.y, 0, 0 ); 
    if( m_pFont9 )
        hr = m_pFont9->DrawText( m_pSprite9, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    //if( FAILED(hr) )
    //    return DXTRACE_ERR_MSGBOX( L"DrawText", hr );

    m_pt.y += m_nLineHeight;

    return S_OK;
}


HRESULT CDXUTTextHelper::DrawFormattedTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg, ... )
{
    WCHAR strBuffer[512];
    
    va_list args;
    va_start(args, strMsg);
    StringCchVPrintf( strBuffer, 512, strMsg, args );
    strBuffer[511] = L'\0';
    va_end(args);

    return DrawTextLine( rc, dwFlags, strBuffer );
}


HRESULT CDXUTTextHelper::DrawTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg )
{
    if( NULL == m_pFont9 ) 
        return DXUT_ERR_MSGBOX( L"DrawTextLine", E_INVALIDARG );

    HRESULT hr;
    if( m_pFont9 )
        hr = m_pFont9->DrawText( m_pSprite9, strMsg, -1, &rc, dwFlags, m_clr );
    //if( FAILED(hr) )
    //    return DXTRACE_ERR_MSGBOX( L"DrawText", hr );

    m_pt.y += m_nLineHeight;

    return S_OK;
}


//--------------------------------------------------------------------------------------
void CDXUTTextHelper::Begin()
{
    if( m_pSprite9 )
        m_pSprite9->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );
}
void CDXUTTextHelper::End()
{
    if( m_pSprite9 )
        m_pSprite9->End();
}

//--------------------------------------------------------------------------------------
HRESULT DXUTSnapD3D9Screenshot( LPCTSTR szFileName )
{
    HRESULT hr = S_OK;
    IDirect3DDevice9* pDev = DXUTGetD3D9Device();
    if( !pDev )
        return E_FAIL;

    IDirect3DSurface9* pBackBuffer = NULL;
    V_RETURN( pDev->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer ) );

    return D3DXSaveSurfaceToFile( szFileName, D3DXIFF_BMP, pBackBuffer, NULL, NULL );
}

//--------------------------------------------------------------------------------------
HRESULT DXUTSnapD3D10Screenshot( LPCTSTR szFileName )
{
	return S_OK;
} 

void CreatePlane(D3DXPLANE* Plane, D3DXVECTOR3* normalVec, D3DXVECTOR3* P)
{
	Plane->a=normalVec->x; Plane->b=normalVec->y; Plane->c=normalVec->z;
	Plane->d=-1.0f*D3DXVec3Dot(P,normalVec);

}
bool CheckPlanePoint(D3DXPLANE* Plane,D3DXVECTOR3* P)// 점이 평면앞에있으면 true, 뒤에있으면 false
{	
	float T;
	T=Plane->a*P->x + Plane->b*P->y + Plane->c*P->z + Plane->d;
	if(T>0.00000f) return true;
	else return false;
}