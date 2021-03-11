// tileset.h: interface for the tileset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TILESET_H__DCE34B91_E3BE_4A10_A6ED_01A5D6EC1A19__INCLUDED_)
#define AFX_TILESET_H__DCE34B91_E3BE_4A10_A6ED_01A5D6EC1A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

enum TileMessage {TILE_MOVED,TILE_DISPLACED};//Enumeration of different types of messages
typedef bool (*TileFunction)(TileMessage message);//Callback function for tiles
typedef unsigned long D3DCOLOR;//In case D3DCOLOR is undefined at this point

struct tiletype {//Structure containing data for a type of tile
	sprite *graphic;
	unsigned int tilebelow;
	TileFunction callback;
	tiletype(sprite *thegraphic,unsigned int thetilebelow,TileFunction thecallback) {
		graphic = thegraphic;
		tilebelow = thetilebelow;
		callback = thecallback;
	}
};
struct tile {//Class containing data for each individual tile
	unsigned int typeoftile;
	sprite graphic;
	tile(unsigned int type) {
		typeoftile=type;
	}
	tile(void) {
		typeoftile=0;
	}
};
class tileset  
{
	tile ***grid;
	D3DCOLOR ***modulationgrid;
	unsigned int width;
	unsigned int height;
	unsigned int layercount;
	unsigned int m_x,m_y,m_drawwidth,m_drawheight;
	deque<tiletype> tilelist;
	sprite transparent;//Sprite for transparent tiles
	typedef void (*SearchFunction)(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
public:
	//Finds if the tile at 'a' is within a range of any of a type of tile.
	bool TileNearTile(unsigned int type,unsigned int radius, POINT& a);
	unsigned int GetTile(unsigned int x,unsigned int y,unsigned int layer) {
		return grid[layer][x][y].typeoftile;
	}
	unsigned int AddTile(sprite *image,unsigned int tileunder,TileFunction callbackfunction);
	void Move(unsigned int layer,unsigned int x,unsigned int y,unsigned int destx,unsigned int desty);
	void Resize(unsigned int layers,unsigned int columns,unsigned int rows);
	void DrawTileSet(unsigned int x,unsigned int y,unsigned int width,unsigned int height);
	void SetTile(unsigned int type,unsigned int x,unsigned int y,unsigned int layer) {
		grid[layer][x][y] = tile(type);//Set tile at location the type given in the 'type' argument
		grid[layer][x][y].graphic = *(tilelist[type].graphic);//Set sprite to the sprite of that tile's type
	}
	void SetColorModulation(D3DCOLOR color);
	void ColorModulateCircle(unsigned int layer,POINT& circlestart,unsigned int radius,D3DCOLOR color);
	inline void SetDrawPos(unsigned int xpos,unsigned int ypos,unsigned int thedrawwidth,unsigned int thedrawheight);
	void SetGrid(unsigned int type,unsigned int layer);
	void SetTileModulation(D3DCOLOR color,unsigned int x,unsigned int y,unsigned int layer) {
		if (x<width&&y<width&&layer<layercount)
			modulationgrid[layer][x][y] = color;
	}
	unsigned int GetType(unsigned int x,unsigned int y,unsigned int layer) {
		return grid[layer][x][y].typeoftile;
	}
	void SearchTiles(unsigned int type,SearchFunction callback,void *extra);
	inline bool GetTileUnderPosition(unsigned int x,unsigned int y,unsigned int *retx,unsigned int *rety);
	tileset();
	~tileset();

};


#endif // !defined(AFX_TILESET_H__DCE34B91_E3BE_4A10_A6ED_01A5D6EC1A19__INCLUDED_)
 