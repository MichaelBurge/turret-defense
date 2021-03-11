// engine.h: interface for the engine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINE_H__1806EBB5_ED1F_4A33_A3A8_3FBCB238AACA__INCLUDED_)
#define AFX_ENGINE_H__1806EBB5_ED1F_4A33_A3A8_3FBCB238AACA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "tileset.h"
#include "control.h"
#include "button.h"
#include "projectile.h"
#include "unit.h"
#include "tank.h"

class engine {
	enum Player{None,Player1,Player2};
	enum Direction {Left,Right,Up,Down};
	enum Tiles {
		Transparent,
		Grass,
		TurretDown,
		TurretUp,
		TurretLeft,
		TurretRight,
		Capital,
		PowerPlant,
		TankDown,
		TankUp,
		TankLeft,
		TankRight,
		EnemyTile
	};
	deque<sprite> spritelist;
	deque<object> objectlist;
	deque<unit*> unitlist;
	Player **playergrid;//Grid for information about who owns which square
	unsigned int width,height;
	int screenx,screeny;
	int grass;
	tileset gametiles;
	
	Player turn;
	unsigned int gui;
	button endturn;
	button placeturret;
	button placepowerplant;
	button placetank;
	bool placingtank;
	bool placingturret;
	bool placingpowerplant;
	bool turretplaced;
	bool powerplantplaced;
	bool tankplaced;
	bool capitalokay;
	bool showenemytiles;
	bool turretfiring;
	bool turretlocationvalid;//The callback function which checks turret location sets this as it's output
	bool powerplantlocationvalid;//See above, except with the power plant
	int powerplant1;
	bool missileactive;
	projectile turretmissile;
	int turretdown,turretup,turretleft,turretright;//Image indexes for turret pictures
	int tankdown,tankup,tankleft,tankright;//Image indexes for tank pictures
	int enemytile;//Image index for picture displayed on enemy tiles
	int turretlaser;//Image index for turret laser
	int horizontalturretlaser;//Image index for horizontal turret laser
	int maxlaserstep;//Maximum number of frames to wait for laser
	POINT turretlaserpoint;//Where the turret's laser is
	POINT turretlaserdestination;//Where the turret's destination is
	POINT turretlocationcheck;//For use in the callback function which checks if a turret's location is valid
	POINT powerplantlocationcheck;//For use in the power-plant placement validation function
	int player1,player2;//Image indexes for player pictures
	Direction tankplacementdirection;//Current direction of tank being placed
	Direction turretplacementdirection;//Current direction of turret being placed
	Direction turretlaserdirection;//Direction of turret laser
	int capital;//Image index for the capital tile
	object *explosionobject;
	static sprite explosionsprite;
	//Options variables
	bool turretrangelimited;



	void Kill(POINT& target);//Kills all units at the target tile
	void GameProcess(void);
	void GameRender(void);
	void ProcessInput(void);
	void Load(void);
	void LoadSprites(void);
	bool PowerPlantLocationValid(unsigned int x,unsigned int y);
	void ColorModulatePowerPlants(void);//Modulates the color around power plants
	void Reset(void);//Resets the engine
	void AddEnemyMarkers(void);//Marks tiles as enemy tiles
	void TankWeaponSearch(void);//Searches all tanks to see if their weapons can be fired.
	void KillUnitsAt(POINT& pos);//Kills all units at a position
	bool IsUnitAt(POINT& pos);//Returns whether a unit is at a position
	unit *UnitAt(POINT& pos);//Returns the unit at a position
	static void EndTurnCallback(bool pressed,void *extra);//Callback function for when end turn button is pressed
	static void PlaceTurretCallback(bool pressed,void *extra);//Callback function for when place turret button is pressed
	static void PlacePowerPlantCallback(bool pressed,void *extra);//Callback function for placing power plant
	static void PlaceTankCallback(bool pressed,void *extra);//Callback function for placing tank
	//To search for tiles to color-modulate
	static void ColorModulationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	//To search if the location was valid for a turret
	static void ValidTurretLocationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	//To search if the location was valid for a power plant
	static void ValidPowerPlantLocationSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	//Callback for turret missile
	static void TurretMissileCallback(projectile::ProjectileMessage,projectile& theprojectile,void *extra);
	//Callbacks for all 4 directions of turret
	static void UpTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	static void DownTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	static void LeftTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	static void RightTurretCallback(unsigned layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	//Callback which searches for player's capital
	static void CapitalSearch(unsigned int layer,unsigned int x,unsigned int y,unsigned int type,void *extra);
	//Callback which handles the callback for the explosion object
	static SIZE ExplosionCallback(object* theobject,object::SentMessage message,void *extra);
	void GridResize(unsigned int newwidth,unsigned int newheight);//Resizes the grid
	friend tank;
public:
	void Options(void);//Shows the options screen
	void Process(void);
	void Initialize(void);
	engine();
	~engine();

};

#endif // !defined(AFX_ENGINE_H__1806EBB5_ED1F_4A33_A3A8_3FBCB238AACA__INCLUDED_)
