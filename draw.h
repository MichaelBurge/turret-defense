// draw.h: interface for the draw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAW_H__55130969_BD47_4DA2_AC78_D50D08110527__INCLUDED_)
#define AFX_DRAW_H__55130969_BD47_4DA2_AC78_D50D08110527__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const bool BACKBUFFER	=false;
const bool PRIMARY		=true;

#include <deque>
using std::deque;
#include <d3d9.h>
#include <d3dx9.h>

class draw {
	deque<IDirect3DTexture9 *> textures;//List of pictures
	deque<D3DXIMAGE_INFO> textureinfo;//List of picture information, for retrieval later
	deque<ID3DXFont *> fonts;//List of fonts
	deque<const char *> picturesource;//Stores file names of pictures
	char *folder;
	int clearcolor;
	D3DCOLOR modulationcolor;//Color for picture modulation
	int currentfont;
	bool target;
	IDirect3D9		*direct3d;//The Direct3D object
	D3DDISPLAYMODE displaymode;//Holds display information
	IDirect3DDevice9 *device;//The device object
	ID3DXSprite *sprite;//Sprite object for drawing pictures
	IDirect3DSurface9 *backbuffer;//Surface for backbuffer
	ID3DXFont *font;//Interface which draws text
	ID3DXLine *line;//Interface for drawing lines
	bool drawing;
public:
	
	HDC lockdc;
	HDC Lock(void);
	void Unlock(void);

	int background;
	int width,height;

	BOOL Clear(void);
	BOOL Flip(void);

	void SetTarget(bool thetarget);
	void SetFolder(char *thefolder);
	void SetFont(int target) {
		currentfont=target;
	}
	void SetModulation(D3DCOLOR color) {
		modulationcolor = color;
	}
	void initialize(HWND hwnd);
	void kill(void);
	void SetClearColor(int color);
	void LoadFont(char *file,char *characters,int charwidth,int charheight,int width,int height);
	void ReloadSurfaces(void);
	void SetBackground(COLORREF color);
	void DrawLine(int x,int y,int x2,int y2, D3DCOLOR color);
	int DrawText(char *text,int x,int y,D3DCOLOR color);
	int DrawText(char *text,int x,int y,int x2,int y2,D3DCOLOR color,int format);
	int DrawText(char *text,int x,int y,int x2,int y2,D3DCOLOR color) {
		return DrawText(text,x,y,x2,y2,color,DT_LEFT);
	}
	
	void DrawNumber(int number,int x,int y,COLORREF color);
	void DrawPictureRepeat(int picture,int picwidth,int picheight,int x,int y, int endx,int endy);
	int StartDraw(void);
	int EndDraw(void);
	int DrawColor(int x,int y,int width,int height,D3DCOLOR color);
	int LoadSurface(const char *file);
	int LoadTexture(const char *file);
	int AddFont(int height,int width,long weight,bool italic,const char fontname[32]);
	int DrawPicture(int x, int y, int picture, RECT *rect);
	int DrawPicture(int x, int y, int thewidth,int theheight,int picture, RECT *rect,bool alpha);
	int DrawPicture(int x, int y, int thewidth,int theheight,int picture, RECT *rect,bool alpha,double rotation);
	int NumberOfPictures(void) {
		return textures.size();
	}
	IDirect3DTexture9 *ReturnTexture(int picture);
	int GetTextureWidth(int picture);
	int GetTextureHeight(int picture);
	
	draw();
	~draw();

};

#endif // !defined(AFX_DRAW_H__55130969_BD47_4DA2_AC78_D50D08110527__INCLUDED_)
