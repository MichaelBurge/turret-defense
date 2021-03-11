// engine.cpp: implementation of the engine class.
//
//////////////////////////////////////////////////////////////////////
#include "main.h"
#include "tileset.h"
#include "misc.h"
#include "algorithms.h"
#include <fstream>
#include <string>
#include "tank.h"
using std::fstream;
using std::string;
using algorithms::TileInCircle;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern input gameinput;
extern draw drawing;
extern bool display_fps;
extern int fps;
SIZE tempsize = {32,32};
sprite engine::explosionsprite(tempsize);

enum Colors { 
	Red = D3DCOLOR_ARGB(255,255,0,0),
	Green = D3DCOLOR_ARGB(255,0,255,0),
	Blue = D3DCOLOR_ARGB(255,0,0,255),
	Yellow = D3DCOLOR_ARGB(255,255,255,0)
};

const int GridWidth = 16,GridHeight = 16;
const int TileWidth = 32,TileHeight = 32;
const unsigned int PowerplantCost = 3;
const unsigned int TurretCost = 5;
const unsigned int TankCost = 4;
const unsigned int powerradius = 3;
const unsigned int tankradius = 3;
const unsigned int powercolor = Red;
const D3DCOLOR powerrangecolor = Blue;
const unsigned int spritecount = 12;
const double laserspeed = 16.0;
const unsigned int TankRange = 3;
const unsigned int TurretRange = GridWidth+GridHeight;


engine::engine()
{
	screenx=0;
	screeny=0;
	placingturret=false;
	turn = Player1;
	turretplacementdirection = Down;
	playergrid=NULL;
	explosionobject = NULL;
	turretplaced = false;
	tankplaced = false;
	powerplantplaced = false;
}

engine::~engine()
{
	while (unitlist.size()) {
		delete unitlist[0];
		unitlist.pop_front();
	}
	if (playergrid)
		for (int temp=0;temp<GridWidth;temp++)
			delete [] playergrid[temp];
	delete [] playergrid;
}

void engine::Initialize(void) {
	Load();
	endturn.QuickSet(718,567,drawing.LoadSurface("pictures/endturnup.bmp"),
		drawing.LoadSurface("pictures/endturndown.bmp"),EndTurnCallback,this,true);
	placeturret.QuickSet(718,500,drawing.LoadSurface("pictures/placeturretup.bmp"),
		drawing.LoadSurface("pictures/placeturretdown.bmp"),PlaceTurretCallback,this,true);
	placepowerplant.QuickSet(718,400,drawing.LoadTexture("pictures/powerplantup.bmp"),
		drawing.LoadTexture("pictures/powerplantdown.bmp"),PlacePowerPlantCallback,this,true);
	placetank.QuickSet(718,300,drawing.LoadTexture("pictures/tankbuttonup.bmp"),
		drawing.LoadTexture("pictures/tankbuttondown.bmp"),PlaceTankCallback,this,true);
	playergrid = new Player*[GridWidth];
	for (int temp=0;temp<GridWidth;temp++) {
		playergrid[temp] = new Player[GridHeight];
		memset(playergrid[temp],0,sizeof(Player)*GridHeight);
	}


	Reset();
}

void engine::Load(void) {
	SIZE size = {32,32};
	for (unsigned int temp=0;temp<=spritecount;temp++)
		spritelist.push_back(sprite(size));
	player1 = drawing.LoadTexture("pictures/player1.bmp");
	player2 = drawing.LoadTexture("pictures/player2.bmp");
	turretlaser = drawing.LoadTexture("pictures/verticallaser.bmp");
	horizontalturretlaser = drawing.LoadTexture("pictures/horizontallaser.bmp");
	//Load sprites for the tile graphics
	LoadSprites();
	//The index of sprite pictures is needed for various reasons in the engine, so obtain it.
	turretdown	= spritelist[2].GetPictureIndex(0);
	turretup	= spritelist[3].GetPictureIndex(0);
	turretleft	= spritelist[4].GetPictureIndex(0);
	turretright	= spritelist[5].GetPictureIndex(0);
	capital		= spritelist[6].GetPictureIndex(0);
	powerplant1	= spritelist[7].GetPictureIndex(0);
	tankdown	= spritelist[8].GetPictureIndex(0);
	tankup		= spritelist[9].GetPictureIndex(0);
	tankleft	= spritelist[10].GetPictureIndex(0);
	tankright	= spritelist[11].GetPictureIndex(0);
	enemytile	= spritelist[12].GetPictureIndex(0);
	gametiles.Resize(3,GridWidth,GridHeight);//Create grid with 3 layers
	gametiles.SetGrid(0,1);//First layer = grass
	gametiles.SetGrid(1,0);//Second layer = transparent
	gametiles.SetGrid(2,0);//Third layer = transparent
	gui = drawing.LoadSurface("pictures/gui.bmp");
}

void engine::Process(void) {
	GameRender();
	GameProcess();
}

void engine::GameProcess(void) {
	ProcessInput();
	if (turretfiring) {

		if (gameinput.KeyPressed(DIK_ESCAPE)) {
			turretfiring = false;
			while (gameinput.KeyPressed(DIK_ESCAPE))
				gameinput.Update();
			return;
		}
		unsigned int tilex,tiley;
		unsigned int destx,desty;
		if (!gametiles.GetTileUnderPosition(turretlaserpoint.x,turretlaserpoint.y,&tilex,&tiley)||
			!gametiles.GetTileUnderPosition(turretlaserdestination.x,turretlaserdestination.y,&destx,&desty)) {
			turretfiring = false;
			return;
		}
		//If time for laser is still > 0
		if (maxlaserstep-->0) {
			switch (turretlaserdirection) {
			case Down:
				turretlaserpoint.y += laserspeed;
				break;
			case Up:
				turretlaserpoint.y -= laserspeed;
				break;
			case Right:
				turretlaserpoint.x += laserspeed;
				break;
			case Left:
				turretlaserpoint.x -= laserspeed;
				break;
			}
			//Laser has hit
		} else {
			POINT destpoint = {destx,desty};
			KillUnitsAt(destpoint);
			gametiles.SetTile(0,destx,desty,1);
			playergrid[destx][desty] = None;
			turretfiring = false;
			explosionobject = new object(explosionsprite,WIDTH,HEIGHT,ExplosionCallback,this);
		}
		return;
	}
	capitalokay = false;
	gametiles.SearchTiles(6,CapitalSearch,this);
	if (!capitalokay) {
		state=0;
		Reset();
	}
	if (!missileactive) {
		missileactive = true;
		POINT pos = {0,0},target = {800,600};
		turretmissile.Activate(pos,target,1.0,600);
	}
	if (missileactive)
		turretmissile.Process();
	if (gameinput.KeyPressed(DIK_DOWN)) {
		turretplacementdirection = Down;
		tankplacementdirection = Down;
	}

	else if (gameinput.KeyPressed(DIK_UP)) {
		turretplacementdirection = Up;
		tankplacementdirection = Up;
	}

	else if (gameinput.KeyPressed(DIK_LEFT)) {
		turretplacementdirection = Left;
		tankplacementdirection = Left;
	}

	else if (gameinput.KeyPressed(DIK_RIGHT)) {
		turretplacementdirection = Right;
		tankplacementdirection = Right;
	}
	if (gameinput.rightclicked) {
		placingturret = false;
		placingpowerplant = false;
		placingtank = false;
	}
	if (gameinput.leftclicked) {
		if (!turretplaced&&placingturret) {
			unsigned int x,y;
			if (gametiles.GetTileUnderPosition(gameinput.mousex,gameinput.mousey,&x,&y)) {
				if (gametiles.GetTile(x,y,1)==0) {//If nothing is in 2nd layer...
					turretlocationvalid = false;//Set variable to false
					turretlocationcheck.x = x;
					turretlocationcheck.y = y;
					Player tempplayer = playergrid[x][y];
					playergrid[x][y] = turn;
					gametiles.SearchTiles(7,ValidTurretLocationSearch,this);//Check if location for turret is within power plant
					if (turretlocationvalid) {
						switch (turretplacementdirection) {
						case Down:
							gametiles.SetTile(2,x,y,1);
							break;
						case Up:
							gametiles.SetTile(3,x,y,1);
							break;
						case Left:
							gametiles.SetTile(4,x,y,1);
							break;
						case Right:
							gametiles.SetTile(5,x,y,1);
							break;
						}
						turretplaced=true;
						placingturret = false;
					} else {
						playergrid[x][y] = tempplayer;
					}
				}

			}
		}
		if (!powerplantplaced&&placingpowerplant) {
			unsigned int x,y;
			if (gametiles.GetTileUnderPosition(gameinput.mousex,gameinput.mousey,&x,&y)) {
				if (gametiles.GetTile(x,y,1)==0) {//If nothing is in 2nd layer...
					Player tempplayer = playergrid[x][y];
					playergrid[x][y] = turn;
					if (PowerPlantLocationValid(x,y)) {
						gametiles.SetTile(7,x,y,1);
						playergrid[x][y] = turn;
						powerplantplaced = true;
						placingpowerplant = false;
					} else {
						playergrid[x][y] = tempplayer;
					}
				}
			}
		}
	} 
	if (explosionobject)
		explosionobject->Process();
	for (unsigned int temp=0;temp<unitlist.size();temp++)
		unitlist[temp]->Process();
	TankWeaponSearch();
	gametiles.SearchTiles(2,DownTurretCallback,this);
	gametiles.SearchTiles(3,UpTurretCallback,this);
	gametiles.SearchTiles(4,LeftTurretCallback,this);
	gametiles.SearchTiles(5,RightTurretCallback,this);
}


void engine::GameRender(void) {
	drawing.StartDraw();
	HRESULT h=0;

	gametiles.DrawTileSet(0,0,700,600);
	RECT rect = {0,0,32,32};
	RECT laserrect = {0,0,32,64};
	RECT hlaserrect = {0,0,64,32};
	if (turretfiring) {
		if (turretlaserdirection == Right || turretlaserdirection == Left)
			drawing.DrawPicture(turretlaserpoint.x,turretlaserpoint.y,horizontalturretlaser,&hlaserrect);
		else
			drawing.DrawPicture(turretlaserpoint.x,turretlaserpoint.y,turretlaser,&laserrect);
		
	}	
	gametiles.SetColorModulation(0xFFFFFFFF);//Clear color modulation. If need be, it'll be modulated on its own.
	for (int temp=0;temp<objectlist.size();temp++)//Loop through all objects
		objectlist[temp].Draw(screenx,screeny);//Draw the current object
	//Draw all units
	for (temp=0;temp<unitlist.size();temp++)
		unitlist[temp]->Render();
	if (explosionobject)
		explosionobject->Draw(screenx,screeny);
	RECT guirect = {0,0,99,599},playerrect = {0,0,drawing.GetTextureWidth(player1),drawing.GetTextureHeight(player2)};
	drawing.DrawPicture(700,0,gui,&guirect);
	if (turn==Player1) {
		drawing.DrawPicture(687,50,player1,&playerrect);
	} else if (turn==Player2) {
		drawing.DrawPicture(687,50,player2,&playerrect);
	}
	placepowerplant.Process();
	placeturret.Process();
	//placetank.Process();
	endturn.Process();
	
	turretmissile.Render();
	if (placingturret) {
		gametiles.SearchTiles(7,ColorModulationSearch,this);
		switch (turretplacementdirection) {
		case Down:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,turretdown,&rect);
			break;
		case Up:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,turretup,&rect);
			break;
		case Left:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,turretleft,&rect);
			break;
		case Right:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,turretright,&rect);
			break;
		}
	} else if (placingpowerplant) {
		//The player is placing a power plant. Color yellow what the power plant will be powering.
		POINT tilepos = {gameinput.mousex/TileWidth,gameinput.mousey/TileHeight};
		gametiles.ColorModulateCircle(0,tilepos,powerradius,powerrangecolor);
		ColorModulatePowerPlants();
		drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,powerplant1,&rect);
		
	} else if (placingtank) {
		gametiles.SearchTiles(Capital,ColorModulationSearch,this);
		switch (tankplacementdirection) {
		case Down:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,tankdown,&rect);
			break;
		case Up:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,tankup,&rect);
			break;
		case Left:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,tankleft,&rect);
			break;
		case Right:
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,tankright,&rect);
			break;
		}
	}
	else drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,target,&rect);
	if (display_fps)  
		drawing.DrawNumber(fps,WIDTH-100,0,D3DCOLOR_ARGB(255,0,0,255));

	drawing.EndDraw();
}

void engine::EndTurnCallback(bool pressed,void *extra) {
	if (pressed) {
		//The function is static, because we need it for a callback, so we can't use member variables.
		//The 'extra' argument is a pointer to the class, so cast it to the class, and access the variable.
		//Set turn to the other player
		if (((engine*)extra)->turn==Player1)
			((engine*)extra)->turn=Player2;
		else
			((engine*)extra)->turn=Player1;
		((engine*)extra)->turretplaced = false;//Clear whether a turret was placed
		((engine*)extra)->powerplantplaced = false;//Clear whether a power plant was placed
		((engine*)extra)->tankplaced = false;//Clear whether a tank was placed
		//If displaying markers on enemy tiles...
		if (((engine*)extra)->showenemytiles)
			//Display the markers
			((engine*)extra)->AddEnemyMarkers();
		for (unsigned int temp=0;temp<((engine*)extra)->unitlist.size();temp++)
			((engine*)extra)->unitlist[temp]->Reset();
	}
}

void engine::PlaceTurretCallback(bool pressed,void *extra) {
	if (pressed) {
		((engine*)extra)->placingturret = true;
		((engine*)extra)->placingpowerplant = false;
		((engine*)extra)->placingtank = false;
	}
}

void engine::PlacePowerPlantCallback(bool pressed,void *extra) {
	if (pressed) {
		((engine*)extra)->placingpowerplant = true;
		((engine*)extra)->placingturret = false;
		((engine*)extra)->placingtank = false;
	}
}

void engine::ColorModulationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type, void *extra) {
	POINT circlestart = {x,y};
	//If the player who owns that tile is having is turn right now...
	if (((engine*)extra)->playergrid[x][y]==((engine*)extra)->turn)
		//Modulate the color on the tile so it's more visible as to where to place his turret
		((engine*)extra)->gametiles.ColorModulateCircle(0,circlestart,powerradius,powercolor);
}

void engine::ValidTurretLocationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	POINT circlestart = {x,y};
	//If turret is within range of the power plant...
	if (((engine*)extra)->playergrid[x][y] == ((engine*)extra)->playergrid[
		((engine*)extra)->turretlocationcheck.x][((engine*)extra)->turretlocationcheck.y]&&//If power plant is player's...
		//And the turret lies within range of the power plant...
		TileInCircle(circlestart,powerradius,((engine*)extra)->turretlocationcheck))
		//Set the validity of the location to true
		((engine*)extra)->turretlocationvalid = true;
}

void engine::TurretMissileCallback(projectile::ProjectileMessage message,projectile& theprojectile,void *extra) {
	if (message==projectile::Exploded)
		((engine*)extra)->missileactive = false;
}

void engine::UpTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	tileset *tiles = &(((engine*)extra)->gametiles);
	engine *theengine = (engine*)extra;
	POINT point = {x,y};
	theengine->turretlocationcheck = point;
	theengine->turretlocationvalid = false;
	tiles->SearchTiles(7,ValidTurretLocationSearch,extra);
	if (theengine->turretlocationvalid) {
		for (unsigned int temp=y-1,temp2=0;temp<GridHeight&&temp2<TurretRange;temp--,temp2++) {
			POINT currentpoint = {x,temp};
			//If there is a unit at the position, fire the laser
			if (theengine->IsUnitAt(currentpoint)&&theengine->UnitAt(currentpoint)->GetPlayer()!=theengine->playergrid[x][y])
				goto firelaser;
			switch (tiles->GetTile(x,temp,layer)) {
			case 2://Down
			case 3://Up
			case 4://Left
			case 5://Right
			case 6://Capital tile
			case 7://Power plant
				if (theengine->playergrid[x][temp] != theengine->playergrid[x][y]) {
firelaser:
					theengine->turretfiring = true;
					theengine->turretlaserpoint.x = x*TileWidth;
					theengine->turretlaserpoint.y = y*TileHeight;
					theengine->turretlaserdestination.x = x*TileWidth;
					theengine->turretlaserdestination.y = temp*TileHeight;
					theengine->turretlaserdirection = Up;
					//Maximum laser frames = distance to destination/speed
					theengine->maxlaserstep = ((theengine->turretlaserpoint.y)-(theengine->turretlaserdestination.y))/laserspeed;
				}
				goto end;
			}
		}
	} else {
		tiles->SetTileModulation(D3DCOLOR_ARGB(255,255,0,0),x,y,layer);
	}
end:
	;
}

void engine::DownTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	tileset *tiles = &(((engine*)extra)->gametiles);
	engine *theengine = (engine*)extra;
	POINT point = {x,y};
	theengine->turretlocationcheck = point;
	theengine->turretlocationvalid = false;
	tiles->SearchTiles(7,ValidTurretLocationSearch,extra);
	if (theengine->turretlocationvalid) {
		for (unsigned int temp=y+1,temp2=0;temp<GridHeight&&temp2<TurretRange;temp++,temp2++) {
			POINT currentpoint = {x,temp};
			//If there is a unit at the position, fire the laser
			if (theengine->IsUnitAt(currentpoint)&&theengine->UnitAt(currentpoint)->GetPlayer()!=theengine->playergrid[x][y])
				goto firelaser;

			switch (tiles->GetTile(x,temp,layer)) {
			case 2://Down
			case 3://Up
			case 4://Left
			case 5://Right
			case 6://Capital tile
			case 7://Power plant
				if (theengine->playergrid[x][temp] != theengine->playergrid[x][y]) {
firelaser:
					theengine->turretfiring = true;
					theengine->turretlaserpoint.x = x*TileWidth;
					theengine->turretlaserpoint.y = y*TileHeight;
					theengine->turretlaserdestination.x = x*TileWidth;
					theengine->turretlaserdestination.y = temp*TileHeight;
					theengine->turretlaserdirection = Down;
					//Maximum laser frames = distance to destination/speed
					theengine->maxlaserstep = ((theengine->turretlaserdestination.y)-(theengine->turretlaserpoint.y))/laserspeed;
					
				}
				goto end;
			}
		}
	} else {
		tiles->SetTileModulation(D3DCOLOR_ARGB(255,255,0,0),x,y,layer);
	}
end:
	;
}

void engine::LeftTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	tileset *tiles = &(((engine*)extra)->gametiles);
	engine *theengine = (engine*)extra;
	POINT point = {x,y};
	theengine->turretlocationcheck = point;
	theengine->turretlocationvalid = false;
	tiles->SearchTiles(7,ValidTurretLocationSearch,extra);
	if (theengine->turretlocationvalid) {
		for (unsigned int temp=x-1,temp2=0;temp<GridWidth&&temp2<TurretRange;temp--,temp2++) {
			POINT currentpoint = {temp,y};
			//If there is a unit at the position, fire the laser
			if (theengine->IsUnitAt(currentpoint)&&theengine->UnitAt(currentpoint)->GetPlayer()!=theengine->playergrid[x][y])
				goto firelaser;
			switch (tiles->GetTile(temp,y,layer)) {
			case 2://Down
			case 3://Up
			case 4://Left
			case 5://Right
			case 6://Capital tile
			case 7://Power plant
				
				if (theengine->playergrid[temp][y] != theengine->playergrid[x][y]) {
firelaser:
					theengine->turretfiring = true;
					theengine->turretlaserpoint.x = x*TileWidth;
					theengine->turretlaserpoint.y = y*TileHeight;
					theengine->turretlaserdestination.x = temp*TileWidth;
					theengine->turretlaserdestination.y = y*TileHeight;
					theengine->turretlaserdirection = Left;
					//Maximum laser frames = distance to destination/speed
					theengine->maxlaserstep = ((theengine->turretlaserpoint.x)-(theengine->turretlaserdestination.x))/laserspeed;
				}
				goto end;
			}
		}
	} else {
		tiles->SetTileModulation(D3DCOLOR_ARGB(255,255,0,0),x,y,layer);
	}
end:
	;
}

void engine::RightTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	tileset *tiles = &(((engine*)extra)->gametiles);
	engine *theengine = (engine*)extra;
	POINT point = {x,y};
	theengine->turretlocationcheck = point;
	theengine->turretlocationvalid = false;
	tiles->SearchTiles(7,ValidTurretLocationSearch,extra);
	if (theengine->turretlocationvalid) {
		for (unsigned int temp=x+1,temp2=0;temp<GridWidth&&temp2<TurretRange;temp++,temp2++) {
			POINT currentpoint = {temp,y};
			//If there is a unit at the position, fire the laser
			if (theengine->IsUnitAt(currentpoint)&&theengine->UnitAt(currentpoint)->GetPlayer()!=theengine->playergrid[x][y])
				goto firelaser;
			switch (tiles->GetTile(temp,y,layer)) {
			case 2://Down
			case 3://Up
			case 4://Left
			case 5://Right
			case 6://Capital tile
			case 7://Power plant
				if (theengine->playergrid[temp][y] != theengine->playergrid[x][y]) {
firelaser:
					theengine->turretfiring = true;
					theengine->turretlaserpoint.x = x*TileWidth;
					theengine->turretlaserpoint.y = y*TileHeight;
					theengine->turretlaserdestination.x = temp*TileWidth;
					theengine->turretlaserdestination.y = y*TileHeight;
					theengine->turretlaserdirection = Right;
					//Maximum laser frames = distance to destination/speed
					theengine->maxlaserstep = ((theengine->turretlaserdestination.x)-(theengine->turretlaserpoint.x))/laserspeed;
				}
				goto end;
			}
		}
	} else {
		tiles->SetTileModulation(D3DCOLOR_ARGB(255,255,0,0),x,y,layer);
	}
end:
	;
}

void engine::ValidPowerPlantLocationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	POINT circlestart = {x,y};
	if (((engine*)extra)->playergrid[x][y] == ((engine*)extra)->playergrid[
		((engine*)extra)->powerplantlocationcheck.x][((engine*)extra)->powerplantlocationcheck.y]&&//If power plant is player's...
		//And the plant lies within range of the power plant...
		algorithms::TileInCircle(circlestart,powerradius,((engine*)extra)->powerplantlocationcheck))
		//Set the validity of the location to true
		((engine*)extra)->powerplantlocationvalid = true;
}

bool engine::PowerPlantLocationValid(unsigned int x,unsigned int y) {
	powerplantlocationcheck.x = x;
	powerplantlocationcheck.y = y;
	powerplantlocationvalid = false;
	gametiles.SearchTiles(7,ValidPowerPlantLocationSearch,this);//Search for power plants
	gametiles.SearchTiles(6,ValidPowerPlantLocationSearch,this);//Search for capitals
	return powerplantlocationvalid;
}

void engine::ColorModulatePowerPlants(void) {
	gametiles.SearchTiles(7,ColorModulationSearch,this);//Make color around power plants
	gametiles.SearchTiles(6,ColorModulationSearch,this);//And capital tiles
}

void engine::CapitalSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra) {
	if (((engine*)extra)->playergrid[x][y] == ((engine*)extra)->turn)
		((engine*)extra)->capitalokay = true;
}

void engine::Reset() {
	unitlist.clear();
	gametiles.Resize(3,GridWidth,GridHeight);//Resize the grid
	gametiles.SetGrid(Grass,0);//Set first layer to grass
	gametiles.SetGrid(Transparent,1);//Set 2nd layer to transparent
	gametiles.SetGrid(Transparent,2);//Set third layer to transparent
	gametiles.SetTile(6,0,0,1);
	gametiles.SetTile(6,GridWidth-1,GridHeight-1,1);
	for (unsigned int temp=0;temp<GridWidth;temp++)
		for (unsigned int temp2=0;temp2<GridHeight;temp2++)
			playergrid[temp][temp2] = None;
	playergrid[0][0] = Player1;
	playergrid[GridWidth-1][GridHeight-1] = Player2;
	turn = Player1;
}

void engine::AddEnemyMarkers(void) {
	//Clear grid before setting markers
	gametiles.SetGrid(0,2);
	//Loop through grid
	for (unsigned int tempw=0;tempw<GridWidth;tempw++)
		for (unsigned int tempy=0;tempy<GridHeight;tempy++)
			//Check if tile is the other player. If so...
			if (playergrid[tempw][tempy]!=None&&playergrid[tempw][tempy] != turn)
				//Set tile in layer above tile to marker
				gametiles.SetTile(EnemyTile,tempw,tempy,2);
}

void engine::ProcessInput(void) {
	if (gameinput.WasKeyPressed(DIK_E)) {
		if (showenemytiles) {
			showenemytiles = false;
			gametiles.SetGrid(0,2);
		} else {
			showenemytiles = true;
			AddEnemyMarkers();
		}
	}
	if (gameinput.KeyPressed(DIK_ESCAPE)) {
		state=0;
		while (gameinput.KeyPressed(DIK_ESCAPE))
			gameinput.Update();
		Reset();
		return;
	}
	//If any arrow keys were pressed...
	if (gameinput.WasKeyPressed(DIK_UP)||gameinput.WasKeyPressed(DIK_DOWN)||
		gameinput.WasKeyPressed(DIK_LEFT)||gameinput.WasKeyPressed(DIK_RIGHT)) {
		int xchange=0,ychange=0;
		unsigned short dir = 0;
		if (gameinput.WasKeyPressed(DIK_UP)) {
			dir=1;
			ychange = -32;
		} else if (gameinput.WasKeyPressed(DIK_DOWN)) {
			dir=0;
			ychange = 32;
		} else if (gameinput.WasKeyPressed(DIK_LEFT)) {
			dir=2;
			xchange = -32;
		} else if (gameinput.WasKeyPressed(DIK_RIGHT)) {
			dir=3;
			xchange = 32;
		}
		//Loop through the units
		for (unsigned int temp=0;temp<unitlist.size();temp++) {
			//If it's selected, and the tank is the current player's...
			if (unitlist[temp]->IsSelected()&&unitlist[temp]->GetPlayer()==turn) {
				//New position equals the current position + the change in X,Y.
				POINT newpos = {unitlist[temp]->GetX()+xchange,unitlist[temp]->GetY()+ychange};
				if (newpos.x > (GridWidth*TileWidth)-32)
					newpos.x = (GridWidth*TileWidth)-32;
				if (newpos.y > (GridHeight*TileHeight)-32)
					newpos.y = (GridHeight*TileHeight)-32;
				if (newpos.x < 0)
					newpos.x = 0;
				if (newpos.y < 0)
					newpos.y = 0;

				//Set the facing direction to the pressed key
				unitlist[temp]->SetFaceDirection(dir);
				//Get the unit moving towards it's destination
				unitlist[temp]->Move(newpos);
			}
		}
	}
	if (gameinput.leftclicked) {
		if (placingtank&&!tankplaced) {
			unsigned int x,y;
			bool validtile = gametiles.GetTileUnderPosition(gameinput.mousex,gameinput.mousey,&x,&y);
			POINT placementpoint = {x,y};
			if (validtile&&!IsUnitAt(placementpoint)&&gametiles.TileNearTile(Capital,tankradius,placementpoint)) {
				POINT tankpos = {x*32,y*32};
				sprite tankimage[4] = {spritelist[TankDown],spritelist[TankUp],spritelist[TankLeft],spritelist[TankRight]};
				unitlist.push_back(new tank(tankpos,tankimage,turn));
				unitlist[unitlist.size()-1]->SetFaceDirection(0);
				placingtank = false;
				tankplaced = true;
			}
		}
		//Select a unit 32 pixels away from the mouse position, if there is one. Deselect all others
		for (unsigned int temp = 0;temp<unitlist.size();temp++) {
			//Deselect unit. If unit is valid, it will be selected below.
			unitlist[temp]->Select(false);
			//If the current unit's middle is 32 pixels away from the mouse position, select it.
			if (DistanceToPoint(unitlist[temp]->GetX()+16,unitlist[temp]->GetY()+16,gameinput.mousex,gameinput.mousey)<32.0) {
				unitlist[temp]->Select(true);
			}
		}
	}
}

SIZE engine::ExplosionCallback(object *theobject,object::SentMessage message,void *extra) {
	SIZE ret = {0,0};
	switch (message) {
	case object::Message_EndOfSprite:
		delete theobject;
		((engine*)extra)->explosionobject = NULL;
		break;
	case object::Message_Position:
		break;
	}
	return ret;
}
/*
Usage:
Opens a data file and reads tile information in.
Parameters:
None
Returns:
Nothing

Format of file is:

Number of tiles
First tile's number of pictures
Location of first tile's first picture
Location of first tile's second picture
Second tile's number of pictures
Location of second tile's first picture
Location of second tile's second picture
etc.

Algorithm is:
Open file "tiles.dat"
Read number of tiles into x
Repeat x times {
 Push sprite z into the back of list w
 Read tile's number of pictures into y
 Repeat y times {
  Read tile's location into string a
  Add picture onto sprite z
 }
 Add sprite z to list of game tiles
}
Close file "tiles.dat"
*/
void engine::LoadSprites(void) {
	//Open file "tiles.dat"
	fstream tiledata("data/tiles.dat");
	//Read number of tiles into x
	unsigned int NumberOfTiles;
	tiledata >> NumberOfTiles;
	//Repeat x times {
	for (unsigned int temp=0;temp<NumberOfTiles;temp++) {
		//Push sprite z into the back of list w
		spritelist.push_back(sprite());
		//Read tile's number of pictures into y
		unsigned int TileNumberOfPictures;
		tiledata >> TileNumberOfPictures;
		//Repeat y times {
		for (unsigned int temp2=0;temp2<TileNumberOfPictures;temp2++) {
			//Read tile's location into string a
			string PictureLocation;
			tiledata >> PictureLocation;
			//Add picture onto sprite z
			spritelist[temp].AddPicture(PictureLocation.c_str());
		//}
		}
		//Add sprite z to list of game tiles
		gametiles.AddTile(&spritelist[temp],0,NULL);
	//}
	}
	//Close file "tiles.dat"
	tiledata.close();
}

void engine::PlaceTankCallback(bool pressed,void *extra) {
	if (pressed) {
		((engine*)extra)->placingturret = false;
		((engine*)extra)->placingpowerplant = false;
		((engine*)extra)->placingtank = true;
	}
}

void engine::TankWeaponSearch(void) {
	for (unsigned int temp=0;temp<unitlist.size();temp++) {
		//Dynamically cast the current unit into a tank.
		tank *currenttank = dynamic_cast<tank*>(unitlist[temp]);
		//If it's > 0, it must be valid, and thus the unit is a tank.
		if (currenttank) {
			POINT pos = currenttank->GetPosition();
			unsigned int x,y;
			bool valid = gametiles.GetTileUnderPosition(pos.x,pos.y,&x,&y);
			if (!valid)
				return;
			unsigned short dir = currenttank->GetFaceDirection();
			switch (dir) {
			case 0://Down
				{
				for (unsigned int temp2=y;temp2<y+TankRange&&temp2<GridHeight;temp2++) {
					POINT currentpoint = {x,temp2};
					//Get the player at that grid location, or if it's out of bounds, the farthest part of the grid.
					Player gridplayer = playergrid[x][temp2];
					if (IsUnitAt(currentpoint))
						gridplayer = static_cast<Player>(UnitAt(currentpoint)->GetPlayer());
					//If it's an enemy player, shoot
					if (gridplayer != None && gridplayer != currenttank->GetPlayer()) {
						turretfiring = true;
						turretlaserpoint.x = pos.x;
						turretlaserpoint.y = pos.y;
						turretlaserdestination.x = pos.x;
						turretlaserdestination.y = temp2*TileHeight;
						turretlaserdirection = Down;
						//Maximum laser frames = distance to destination/speed
						maxlaserstep = ((turretlaserdestination.y)-(turretlaserpoint.y))/laserspeed;
						break;
					}
				}
				}
				break;
			case 1://Up
				{
				for (unsigned int temp2=y;temp2>y-TankRange;temp2--) {
					POINT currentpoint = {x,temp2};
					//Get the player at that grid location, or if it's out of bounds, the farthest part of the grid.
					Player gridplayer = playergrid[x][temp2];
					//If there is a unit at the position, set the gridplayer variable to the owner of that unit
					if (IsUnitAt(currentpoint))
						gridplayer = static_cast<Player>(UnitAt(currentpoint)->GetPlayer());
					//If it's an enemy player, shoot
					if (gridplayer != None && gridplayer != currenttank->GetPlayer()) {
						turretfiring = true;
						turretlaserpoint.x = pos.x;
						turretlaserpoint.y = pos.y;
						turretlaserdestination.x = pos.x;
						turretlaserdestination.y = temp2*TileHeight;
						turretlaserdirection = Up;
						//Maximum laser frames = distance to destination/speed
						maxlaserstep = ((turretlaserpoint.y)-(turretlaserdestination.y))/laserspeed;
						break;
					}
					if (temp2==0)
						break;
				}
				}
				break;
			case 2://Left
				{
				for (unsigned int temp2=x;temp2>x-TankRange;temp2--) {
					POINT currentpoint = {temp2,y};
					//Get the player at that grid location, or if it's out of bounds, the farthest part of the grid.
					Player gridplayer = playergrid[temp2][y];
					//If there is a unit at the position, set the gridplayer variable to the owner of that unit
					if (IsUnitAt(currentpoint))
						gridplayer = static_cast<Player>(UnitAt(currentpoint)->GetPlayer());
					//If it's an enemy player, shoot
					if (gridplayer != None && gridplayer != currenttank->GetPlayer()) {
						turretfiring = true;
						turretlaserpoint.x = pos.x;
						turretlaserpoint.y = pos.y;
						turretlaserdestination.x = temp2*TileWidth;
						turretlaserdestination.y = pos.y;
						turretlaserdirection = Left;
						//Maximum laser frames = distance to destination/speed
						maxlaserstep = ((turretlaserpoint.x)-(turretlaserdestination.x))/laserspeed;
						break;
					}
					if (temp2==0)
						break;
				}
				}
				break;
			case 3://Right
				{
				for (unsigned int temp2=x;temp2<x+TankRange&&temp2<GridWidth;temp2++) {
					POINT currentpoint = {temp2,y};
					//Get the player at that grid location, or if it's out of bounds, the farthest part of the grid.
					Player gridplayer = playergrid[temp2][y];
					if (IsUnitAt(currentpoint))
						gridplayer = static_cast<Player>(UnitAt(currentpoint)->GetPlayer());
					//If it's an enemy player, shoot
					if (gridplayer != None && gridplayer != currenttank->GetPlayer()) {
						turretfiring = true;
						turretlaserpoint.x = pos.x;
						turretlaserpoint.y = pos.y;
						turretlaserdestination.x = temp2*TileHeight;
						turretlaserdestination.y = pos.y;
						turretlaserdirection = Right;
						//Maximum laser frames = distance to destination/speed
						maxlaserstep = ((turretlaserdestination.x)-(turretlaserpoint.x))/laserspeed;
						break;
					}
				}
				}
				break;

			}
		}
	}
}

void engine::KillUnitsAt(POINT& pos) {
	//Loop through all units
	for (unsigned int temp=0;temp<unitlist.size();temp++) {
		//Get the position, and divide by tile width/height
		POINT unitpos = unitlist[temp]->GetPosition();
		unitpos.x/=TileWidth;
		unitpos.y/=TileHeight;
		//Kill the unit
		if (unitpos.x==pos.x&&unitpos.y==pos.y)
			unitlist.erase(unitlist.begin()+temp);
	}
}

bool engine::IsUnitAt(POINT& pos) {
	//Loop through all units
	for (unsigned int temp=0;temp<unitlist.size();temp++) {
		//Get the position, and divide by tile width/height
		POINT unitpos = unitlist[temp]->GetPosition();
		unitpos.x/=TileWidth;
		unitpos.y/=TileHeight;
		//Kill the unit
		if (unitpos.x==pos.x&&unitpos.y==pos.y)
			return true;
	}
	return false;
}

unit *engine::UnitAt(POINT& pos) {
	//Loop through all units
	for (unsigned int temp=0;temp<unitlist.size();temp++) {
		//Get the position, and divide by tile width/height
		POINT unitpos = unitlist[temp]->GetPosition();
		unitpos.x/=TileWidth;
		unitpos.y/=TileHeight;
		//Kill the unit
		if (unitpos.x==pos.x&&unitpos.y==pos.y)
			return unitlist[temp];
	}
	return NULL;
}
