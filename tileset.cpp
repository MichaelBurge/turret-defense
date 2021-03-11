// tileset.cpp: implementation of the tileset class.
//
//////////////////////////////////////////////////////////////////////
#include <deque>
using namespace std;
#include "windows.h"
#include "sprite.h"
#include "object.h"
#include "tileset.h"
#include "debug.h"
#include "algorithms.h"
extern logger debugger;


//////////////////////////////////////////////////////////////////////
// Methods
//////////////////////////////////////////////////////////////////////
//Constructor
tileset::tileset()
{
	grid=NULL;
	width=0;
	height=0;
	layercount=0;
	m_x = 0;
	m_y = 0;
	m_drawwidth = 0;
	m_drawheight= 0;
}
//Destructor
tileset::~tileset()
{
	if (grid) {//If the variable is already pointing to something...
		for (unsigned int temp=0;temp<layercount;temp++) { //Loop through layers
			for (unsigned int temp2=0;temp2<width;temp2++) {//Loop through columns
				delete [] grid[temp][temp2];//Delete columns
				delete [] modulationgrid[temp][temp2];//Delete modulation grid columns
			}
			delete [] grid[temp];
			delete [] modulationgrid[temp];//Delete modulation grid rows
		}
		delete [] grid;//Delete layers in grid
		delete [] modulationgrid;//Delete modulation grid layers
	}
}

//Resize grid
void tileset::Resize(unsigned int layers,unsigned int columns,unsigned int rows) {
	if (grid) {//If the variable is already pointing to something...
		for (unsigned int temp=0;temp<layers;temp++) { //Loop through layers
			for (unsigned int temp2=0;temp2<width;temp2++) {//Loop through columns
				delete [] grid[temp][temp2];//Delete columns
				delete [] modulationgrid[temp][temp2];//Delete modulation grid columns
			}
			delete [] grid[temp];
			delete [] modulationgrid[temp];//Delete modulation grid rows
		}
		delete [] grid;//Delete layers in grid
		delete [] modulationgrid;//Delete modulation grid layers
	}
	//Tile grid
	grid = new tile**[layers];//Allocate layers
	for (unsigned int temp=0;temp<layers;temp++) {//Loop through layers, adding columns
		grid[temp] = new tile*[rows];//Allocate column
		for (unsigned int temp2=0;temp2<columns;temp2++)
			grid[temp][temp2] = new tile[rows];
	}
	for (temp=0;temp<layers;temp++)//Loop through layers
		for (unsigned int temp2=0;temp2<columns;temp2++)//Loop through columns
			for (unsigned int temp3=0;temp3<rows;temp3++)//Loop through all rows in that column
				grid[temp][temp2][temp3].typeoftile=0;//Set the tile at current row/column to 0
	//Modulation grid
	modulationgrid = new D3DCOLOR**[layers];//Allocate layers
	for (temp=0;temp<layers;temp++) {//Loop through layers, adding columns
		modulationgrid[temp] = new D3DCOLOR*[rows];//Allocate column
		for (unsigned int temp2=0;temp2<columns;temp2++)
			modulationgrid[temp][temp2] = new D3DCOLOR[rows];
	}
	for (temp=0;temp<layers;temp++)//Loop through layers
		for (unsigned int temp2=0;temp2<columns;temp2++)//Loop through columns
			for (unsigned int temp3=0;temp3<rows;temp3++)//Loop through all rows in that column
				modulationgrid[temp][temp2][temp3]=0xFFFFFFFF;//Set the modulation color to normal
	width=columns;//Grid's width is set to the number of columns
	height=rows;//Grid's height is set to the number of rows
	layercount=layers;
}
//Move a tile to another location, calling callback functions for both tiles, and setting the moved tile to the tile below
void tileset::Move(unsigned int layer,unsigned int x,unsigned int y,unsigned int destx,unsigned int desty) {
	if (tilelist[grid[layer][x][y].typeoftile].callback)//If the tile to be moved has a callback...
			tilelist[grid[layer][x][y].typeoftile].callback(TILE_MOVED);//Call the callback, telling it it's to be moved
	if (tilelist[grid[layer][destx][desty].typeoftile].callback)//If the destination tile has a callback...
			tilelist[grid[layer][destx][desty].typeoftile].callback(TILE_DISPLACED);//Call it telling the callback it's to be displaced
	grid[layer][destx][desty].typeoftile = grid[layer][x][y].typeoftile;//Set destination type of tile to the moved tile's type
	grid[layer][x][y].typeoftile = tilelist[grid[layer][x][y].typeoftile].tilebelow;//Set the moved tile's type to the tile below it
}
//Adds a tile to the list of types of tiles
unsigned int tileset::AddTile(sprite *image,unsigned int tileunder,TileFunction callbackfunction) {
	tilelist.push_back(tiletype(image,tileunder,callbackfunction));//Create a type of tile based on arguments and push to end
	return tilelist.size()-1;//Return index of new tile type
}
//Sets various class members
inline void tileset::SetDrawPos(unsigned int xpos,unsigned int ypos,unsigned int thedrawwidth,unsigned int thedrawheight) {
	m_x = xpos;
	m_y = ypos;
	m_drawwidth = thedrawwidth;
	m_drawheight= thedrawheight;
}
//Draws tile set
void tileset::DrawTileSet(unsigned int x,unsigned int y,unsigned int drawwidth,unsigned int drawheight) {
	unsigned int tilewidth = tilelist[grid[0][x][y].typeoftile].graphic->GetWidth();//Get width of tile
	unsigned int tileheight= tilelist[grid[0][x][y].typeoftile].graphic->GetHeight();//Get height of tile
	//Loop until end of grid is reached
	unsigned int tempy=y;
	unsigned int tempx=x;
	//Loop through tiles, drawing each one
	for (unsigned int temp=0;temp<layercount;temp++,x=tempx)//Loop through layers
		for (unsigned int temp2=0;temp2<width&&x<drawwidth;temp2++,x+=tilewidth,y=tempy)//Loop through columns
			for (unsigned int temp3=0;temp3<height&&y<drawheight;temp3++,y+=tileheight) {//Loop through rows
				drawing.SetModulation(modulationgrid[temp][temp2][temp3]);
				grid[temp][temp2][temp3].graphic.Draw(x,y);//Draw grid sprite at current location
			}
	drawing.SetModulation(0xFFFFFFFF);
}
//Sets all tiles in the grid to a certain type
void tileset::SetGrid(unsigned int type,unsigned int layer) {
	for (unsigned int temp=0;temp<width;temp++)//Loop through columns
		for (unsigned int temp2=0;temp2<height;temp2++)//Loop through rows
			SetTile(type,temp,temp2,layer);//Set column/row to the tile specified by the argument
}

//Gets the tile position under the position
extern inline bool tileset::GetTileUnderPosition(unsigned int x,unsigned int y,unsigned int *retx,unsigned int *rety) {
	//Get the x 'moved' so that the tileset's left position is 0, and get the tile
	*retx = (x-m_x)/(tilelist[grid[0][0][0].typeoftile].graphic->GetWidth());
	*rety = (y-m_y)/(tilelist[grid[0][0][0].typeoftile].graphic->GetHeight());//Same as above except with y
	//Check if the mouse's tile would be outside of the tileset
	if (*retx>=width || *rety>=height)
		return false;
	return true;
}

void tileset::ColorModulateCircle(unsigned int layer,POINT& circlestart,unsigned int radius,D3DCOLOR color) {
	//Start with a line length of 1, at 'radius' tiles above the start
	unsigned int linelength = 1;
	unsigned int xstart = circlestart.x;
	unsigned int ystart = circlestart.y - radius;
	//For every line until the line length reaches (radius*2)+1 (to compensate for the middle tile)
	unsigned int widest = (radius*2)+1;
	while (linelength<widest) {
		//Go to the starting point, and go in a line to the right, setting color modulation
		for (unsigned int temp=0;temp<linelength;temp++)
			SetTileModulation(color,xstart+temp,ystart,layer);
		//Go left and down one tile
		xstart--;
		ystart++;
		//Increment the line length by 2;
		linelength+=2;
	}
	linelength += 2;
	//The top 1/2 is done. Now do the bottom
	while (linelength>1) {
		//Decrement the line length by 2;
		linelength-=2;
		//Go to the starting point, and go in a line to the right, setting color modulation
		for (unsigned int temp=0;temp<linelength;temp++)
			SetTileModulation(color,xstart+temp,ystart,layer);
		//Go right and down one tile
		xstart++;
		ystart++;
		
	}
}

void tileset::SearchTiles(unsigned int type,SearchFunction callback,void *extra) {
	for (unsigned int temp=0;temp<layercount;temp++)
		for (unsigned int temp2=0;temp2<width;temp2++)
			for (unsigned int temp3=0;temp3<height;temp3++)
				if (grid[temp][temp2][temp3].typeoftile==type)
					callback(temp,temp2,temp3,type,extra);
}

void tileset::SetColorModulation(D3DCOLOR color) {
	for (unsigned int temp=0;temp<layercount;temp++)
		for (unsigned int temp2=0;temp2<width;temp2++)
			for (unsigned int temp3=0;temp3<height;temp3++)
				modulationgrid[temp][temp2][temp3] = color;
}

bool tileset::TileNearTile(unsigned int type,unsigned int radius,POINT& a) {
	//Search the grid
	for (unsigned int temp=0;temp<layercount;temp++)
		for (unsigned int temp2=0;temp2<width;temp2++)
			for (unsigned int temp3=0;temp3<height;temp3++) {
				if (grid[temp][temp2][temp3].typeoftile == type) {
					POINT circlestart = {temp2,temp3};
					if (algorithms::TileInCircle(circlestart,radius,a))
						return true;
				}
			}
	return false;
}