//--------------------------------------------------------------------------------------
// File: SDKMisc.h
//
// Various helper functionality that is shared between SDK samples
//
// Copyright (c) Microsoft Corporation. All rights reserved
//--------------------------------------------------------------------------------------
#pragma once
#ifndef SDKMISC_H
#define SDKMISC_H

//------------------------------------------------------------------
// 버텍스, 인덱스 구조체
//------------------------------------------------------------------
struct  CVERTEX_RHW			{ D3DXVECTOR4 pos;		DWORD		color;											enum { FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE };					};	//버텍스 타입 구조체
struct  CVERTEX_DIF			{ D3DXVECTOR3 pos;		DWORD		color;											enum { FVF = D3DFVF_XYZ|D3DFVF_DIFFUSE	};							};
struct	CVERTEX_NORMAL		{ D3DXVECTOR3 pos;		D3DXVECTOR3 normal;											enum { FVF = D3DFVF_XYZ|D3DFVF_NORMAL	};						};
struct	CVERTEX_RHWTEX		{ D3DXVECTOR4 pos;		float		tu,tv;											enum { FVF = D3DFVF_XYZRHW|D3DFVF_TEX1	};						};
struct	CVERTEX_RHWDTEX		{ D3DXVECTOR4 pos;		DWORD		color;		float tu,tv;						enum { FVF = D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1	};		};
struct	CVERTEX_TEX			{ D3DXVECTOR3 pos;		float		tu,tv;											enum { FVF = D3DFVF_XYZ|D3DFVF_TEX1	};							};
struct	CVERTEX_NTEX		{ D3DXVECTOR3 pos;		D3DXVECTOR3 normal;		float tu,tv;						enum { FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1				};	};
struct	CVERTEX_NTTEX		{ D3DXVECTOR3 pos;		D3DXVECTOR3 normal;		float tu1,tv1;		float tu2,tv2;	enum { FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1|D3DFVF_TEX2	};	};
struct	CVERTEX_NDTEX		{ D3DXVECTOR3 pos;		D3DXVECTOR3 normal;		DWORD diff;			D3DXVECTOR2 uv;	enum { FVF = D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_DIFFUSE|D3DFVF_TEX1	};	};
struct ASE_VERTEX_SKIN
{
	enum { FVF = (D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX3|
		D3DFVF_TEXCOORDSIZE2(0)|		// texture
		D3DFVF_TEXCOORDSIZE4(1)|		// blend weight
		D3DFVF_TEXCOORDSIZE4(2)) };	// blend index

	D3DXVECTOR3	p;	// position
	D3DXVECTOR3	n;	// normal
	D3DXVECTOR2	t;	// texture coord
	D3DXVECTOR4	b;	// blend weight
	D3DXVECTOR4	i;	// blend index
};

struct	CINDEX				{ WORD _0, _1, _2;	};
struct	CINDEXL				{ WORD _0, _1;		};

void	SET_VTX_RHW(CVERTEX_RHW &ct, D3DXVECTOR4 p, DWORD clr);
void	SET_VTX_DIF(CVERTEX_DIF &ct, D3DXVECTOR3 p, DWORD clr=0xffffffff);
void	SET_VTX_NORMAL(CVERTEX_NORMAL &ct, D3DXVECTOR3 p, D3DXVECTOR3 n);
void	SET_VTX_RHWTEX(CVERTEX_RHWTEX &ct, D3DXVECTOR4 p, float tu, float tv);
void	SET_VTX_RHWDTEX(CVERTEX_RHWDTEX &ct, D3DXVECTOR4 p, DWORD clr, float tu, float tv);
void	SET_VTX_TEX(CVERTEX_TEX &ct, D3DXVECTOR3 p, float tu, float tv);
void	SET_VTX_NTEX(CVERTEX_NTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, float tu, float tv);
void	SET_VTX_NTTEX(CVERTEX_NTTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, float tu1, float tv1, float tu2, float tv2);
void	SET_VTX_NDTEX(CVERTEX_NDTEX &ct, D3DXVECTOR3 p, D3DXVECTOR3 n, DWORD c, float tu, float tv);

void	SET_IDX(CINDEX &ci, WORD _0, WORD _1, WORD _2);
void	SET_IDXL(CINDEXL &ci, WORD _0, WORD _1);



//------------------------------------------------------------------
// 
//------------------------------------------------------------------
HRESULT Create_Shader_Quad(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, float size=1.f);
HRESULT	Create_Vtx2D(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DTEXTURE9 &tex, int x, int y, int w, int h, WCHAR* str=NULL, DWORD dif=0xffffffff, float tu0=0.f, float tv0=0.f, float tu1=1.f, float tv1=1.f);
HRESULT	Create_Vtx2DI(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DTEXTURE9 &tex, int x, int y, int w, int h, WCHAR* str=NULL, DWORD dif=0xffffffff, float tu0=0.f, float tv0=0.f, float tu1=1.f, float tv1=1.f);
void	Render_Vtx2D(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DTEXTURE9 &tex);
void	Render_Vtx2DI(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DTEXTURE9 &tex);
void	Render_Shader_Quad(LPDIRECT3DVERTEXBUFFER9 &vb, LPDIRECT3DINDEXBUFFER9 &ib, LPDIRECT3DVERTEXDECLARATION9 decl=NULL);
void	Render_QuadUP(LPDIRECT3DDEVICE9 dev, int x, int y, int w, int h, LPDIRECT3DTEXTURE9 tex, float u0=0.f, float v0=0.f, float u1=1.f, float v1=1.f);
void	Render_Line(LPDIRECT3DDEVICE9 dev, D3DXVECTOR3& src, D3DXVECTOR3& des, DWORD color);



//------------------------------------------------------------------
// Debug Text
//------------------------------------------------------------------
WCHAR*  ctow(char* val, ...);
void	DrawPrintf(WCHAR* str, int x, int y, int w, int h, int bold=0, DWORD color=0xffffffff, DWORD flags=DT_LEFT);
void	MsgBox(char* str, ...);
void	MsgBox(WCHAR* str, ...);


//-----------------------------------------------------------------------------
// Resource cache for textures, fonts, meshs, and effects.  
// Use DXUTGetGlobalResourceCache() to access the global cache
//-----------------------------------------------------------------------------

enum DXUTCACHE_SOURCELOCATION { DXUTCACHE_LOCATION_FILE, DXUTCACHE_LOCATION_RESOURCE };

struct DXUTCache_Texture
{
    DXUTCACHE_SOURCELOCATION Location;
    WCHAR wszSource[MAX_PATH];
    HMODULE hSrcModule;
    UINT Width;
    UINT Height;
    UINT Depth;
    UINT MipLevels;
    UINT MiscFlags;
    union
    {
        DWORD Usage9;
    };
    union
    {
        D3DFORMAT Format9;
    };
    union
    {
        D3DPOOL Pool9;
        UINT CpuAccessFlags;
    };
    union
    {
        D3DRESOURCETYPE Type9;
        UINT BindFlags;
    };
    IDirect3DBaseTexture9 *pTexture9;

    DXUTCache_Texture()
    {
        pTexture9 = NULL;
    }
};

struct DXUTCache_Font : public D3DXFONT_DESC
{
    ID3DXFont *pFont;
};

struct DXUTCache_Effect
{
    DXUTCACHE_SOURCELOCATION Location;
    WCHAR wszSource[MAX_PATH];
    HMODULE hSrcModule;
    DWORD dwFlags;
    ID3DXEffect *pEffect;
};


class CDXUTResourceCache
{
public:
    ~CDXUTResourceCache();

    HRESULT CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DTEXTURE9 *ppTexture );
    HRESULT CreateTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCSTR pSrcFile, LPDIRECT3DTEXTURE9 *ppTexture );
    HRESULT CreateTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture );
    HRESULT CreateTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DTEXTURE9 *ppTexture );
    HRESULT CreateTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DTEXTURE9 *ppTexture );
    HRESULT CreateCubeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
    HRESULT CreateCubeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
    HRESULT CreateCubeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
    HRESULT CreateCubeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Size, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DCUBETEXTURE9 *ppCubeTexture );
    HRESULT CreateVolumeTextureFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
    HRESULT CreateVolumeTextureFromFileEx( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppTexture );
    HRESULT CreateVolumeTextureFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
    HRESULT CreateVolumeTextureFromResourceEx( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, UINT Width, UINT Height, UINT Depth, UINT MipLevels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, DWORD Filter, DWORD MipFilter, D3DCOLOR ColorKey, D3DXIMAGE_INFO *pSrcInfo, PALETTEENTRY *pPalette, LPDIRECT3DVOLUMETEXTURE9 *ppVolumeTexture );
    HRESULT CreateFont( LPDIRECT3DDEVICE9 pDevice, UINT Height, UINT Width, UINT Weight, UINT MipLevels, bool Italic, DWORD CharSet, DWORD OutputPrecision, DWORD Quality, DWORD PitchAndFamily, LPCTSTR pFacename, LPD3DXFONT *ppFont );
    HRESULT CreateFontIndirect( LPDIRECT3DDEVICE9 pDevice, CONST D3DXFONT_DESC *pDesc, LPD3DXFONT *ppFont );
    HRESULT CreateEffectFromFile( LPDIRECT3DDEVICE9 pDevice, LPCTSTR pSrcFile, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );
    HRESULT CreateEffectFromResource( LPDIRECT3DDEVICE9 pDevice, HMODULE hSrcModule, LPCTSTR pSrcResource, const D3DXMACRO *pDefines, LPD3DXINCLUDE pInclude, DWORD Flags, LPD3DXEFFECTPOOL pPool, LPD3DXEFFECT *ppEffect, LPD3DXBUFFER *ppCompilationErrors );

public:
    HRESULT OnCreateDevice( IDirect3DDevice9 *pd3dDevice );
    HRESULT OnResetDevice( IDirect3DDevice9 *pd3dDevice );
    HRESULT OnLostDevice();
    HRESULT OnDestroyDevice();

protected:
    friend CDXUTResourceCache& WINAPI DXUTGetGlobalResourceCache();
    friend HRESULT WINAPI DXUTInitialize3DEnvironment();
    friend HRESULT WINAPI DXUTReset3DEnvironment();
    friend void WINAPI DXUTCleanup3DEnvironment( bool bReleaseSettings );

    CDXUTResourceCache() { }

    CGrowableArray< DXUTCache_Texture > m_TextureCache;
    CGrowableArray< DXUTCache_Effect > m_EffectCache;
    CGrowableArray< DXUTCache_Font > m_FontCache;
};

CDXUTResourceCache& WINAPI DXUTGetGlobalResourceCache();


//--------------------------------------------------------------------------------------
// Manages the insertion point when drawing text
//--------------------------------------------------------------------------------------
class CDXUTTextHelper
{
public:
    CDXUTTextHelper( ID3DXFont* pFont9 = NULL, ID3DXSprite* pSprite9 = NULL, int nLineHeight = 15 );
    ~CDXUTTextHelper();

    void Init( ID3DXFont* pFont9 = NULL, ID3DXSprite* pSprite9 = NULL, int nLineHeight = 15 );

    void SetInsertionPos( int x, int y ) { m_pt.x = x; m_pt.y = y; }
    void SetForegroundColor( D3DXCOLOR clr ) { m_clr = clr; }

    void Begin();
    HRESULT DrawFormattedTextLine( const WCHAR* strMsg, ... );
    HRESULT DrawTextLine( const WCHAR* strMsg );
    HRESULT DrawFormattedTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg, ... );
    HRESULT DrawTextLine( RECT &rc, DWORD dwFlags, const WCHAR* strMsg );
    void End();

protected:
    ID3DXFont*   m_pFont9;
    ID3DXSprite* m_pSprite9;
    D3DXCOLOR    m_clr;
    POINT        m_pt;
    int          m_nLineHeight;
};


//--------------------------------------------------------------------------------------
// Manages a persistent list of lines and draws them using ID3DXLine
//--------------------------------------------------------------------------------------
class CDXUTLineManager
{
public:
    CDXUTLineManager();
    ~CDXUTLineManager();

    HRESULT OnCreatedDevice( IDirect3DDevice9* pd3dDevice );
    HRESULT OnResetDevice();
    HRESULT OnRender();
    HRESULT OnLostDevice();
    HRESULT OnDeletedDevice();

    HRESULT AddLine( int* pnLineID, D3DXVECTOR2* pVertexList, DWORD dwVertexListCount, D3DCOLOR Color, float fWidth, float fScaleRatio, bool bAntiAlias );
    HRESULT AddRect( int* pnLineID, RECT rc, D3DCOLOR Color, float fWidth, float fScaleRatio, bool bAntiAlias );
    HRESULT RemoveLine( int nLineID );
    HRESULT RemoveAllLines();

protected:
    struct LINE_NODE
    {
        int      nLineID;
        D3DCOLOR Color;
        float    fWidth;
        bool     bAntiAlias;
        float    fScaleRatio;
        D3DXVECTOR2* pVertexList;
        DWORD    dwVertexListCount;
    };

    CGrowableArray<LINE_NODE*> m_LinesList;
    IDirect3DDevice9* m_pd3dDevice;
    ID3DXLine* m_pD3DXLine;
};


//--------------------------------------------------------------------------------------
// Shared code for samples to ask user if they want to use a REF device or quit
//--------------------------------------------------------------------------------------
void DXUTDisplaySwitchingToREFWarning( DXUTDeviceVersion ver );

//--------------------------------------------------------------------------------------
// Tries to finds a media file by searching in common locations
//--------------------------------------------------------------------------------------
HRESULT WINAPI DXUTFindDXSDKMediaFileCch( WCHAR* strDestPath, int cchDest, LPCWSTR strFilename );
HRESULT WINAPI DXUTSetMediaSearchPath( LPCWSTR strPath );
LPCWSTR WINAPI DXUTGetMediaSearchPath();


//--------------------------------------------------------------------------------------
// Returns a view matrix for rendering to a face of a cubemap.
//--------------------------------------------------------------------------------------
D3DXMATRIX WINAPI DXUTGetCubeMapViewMatrix( DWORD dwFace );


//--------------------------------------------------------------------------------------
// Takes a screen shot of a 32bit D3D10 back buffer and saves the images to a BMP file
//--------------------------------------------------------------------------------------
HRESULT DXUTSnapD3D9Screenshot( LPCTSTR szFileName );
HRESULT DXUTSnapD3D10Screenshot( LPCTSTR szFileName );

//--------------------------------------------------------------------------------------
// Simple helper stack class
//--------------------------------------------------------------------------------------
template <class T>
class CDXUTStack
{
private:
    UINT m_MemorySize;
    UINT m_NumElements;
    T*	m_pData;

    bool EnsureStackSize( UINT64 iElements )
    {
        if( m_MemorySize > iElements )
            return true;

        T* pTemp = new T[ (size_t)(iElements*2 + 256) ];
        if( !pTemp )
            return false;

        if( m_NumElements )
        {
            CopyMemory( pTemp, m_pData, (size_t)(m_NumElements*sizeof(T)) );
        }
    
        if( m_pData ) delete []m_pData;
        m_pData = pTemp;
        return true;
    }

public:
    CDXUTStack() { m_pData = NULL; m_NumElements = 0; m_MemorySize = 0; }
    ~CDXUTStack() { if( m_pData ) delete []m_pData; }

    UINT GetCount() { return m_NumElements; }
    T GetAt( UINT i ) { return m_pData[i]; }
    T GetTop()
    {
        if( m_NumElements < 1 )
            return NULL;

        return m_pData[ m_NumElements-1 ];
    }

    T GetRelative( INT i )
    {
        INT64 iVal = m_NumElements-1 + i;
        if( iVal < 0 )
            return NULL;
        return m_pData[ iVal ];
    }

    bool Push( T pElem )
    {
        if(!EnsureStackSize( m_NumElements+1 ) )
            return false;
        
        m_pData[m_NumElements] = pElem;
        m_NumElements++;

        return true;
    }

    T Pop()
    {
        if( m_NumElements < 1 )
            return NULL;

        m_NumElements --;
        return m_pData[m_NumElements];
    }
};
template<typename TContainer>
inline void stl_wipe(TContainer& container)
{
	for( TContainer::iterator i = container.begin() ; i != container.end() ; ++i )
		delete *i;
	container.clear();
}

void CreatePlane(D3DXPLANE* Plane, D3DXVECTOR3* normalVec, D3DXVECTOR3* P);
bool CheckPlanePoint(D3DXPLANE* Plane,D3DXVECTOR3* P);
#endif