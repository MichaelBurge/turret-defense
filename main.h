#ifndef main
#define DIRECTINPUT_VERSION 0x0800
#include <vector>
#include <deque>
//#include <ddraw.h>
#include <dinput.h>
#include <DMUSICC.H>
#include <DMUSICI.H>
#include <stdlib.h>
#include <windows.h>
#include <math.h>//Include C math header
#include <d3d9.h>
#include <d3dx9math.h>
//#include <stdlib.h>
using namespace std;
//Include low-level headers
//#include "ddutil.h"
#include "input.h"
#include "draw.h"
#include "sprite.h"
#include "object.h"
#include "strings.h"
#include "LogFile.h"//Include RTF logger
//Include high-level headers
#include "sprite.h"
#include "object.h"
#include "tileset.h"

#include "engine.h"
#include "music.h"
#include "math.h"//Include my math header


//Include GUI system
#include "controls.h"
extern HWND hwnd;
extern int selectedkey,target,targetted;
extern int playerside;
extern int firstplanet;
extern int win,lose;
extern int state;
extern int redx;
extern int playercolors[];
extern int bottomgui;
unsigned long WINAPI drawthread(LPVOID parameter);

extern int WIDTH,HEIGHT;
#define main
#endif