#include "main.h"
#include "misc.h"
bool running=true;
HWND hwnd;
int selectedkey,target,targetted;
TCHAR szWindowClass[] = "Window";
LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long _stdcall WndProc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam);
void InitFonts(void);
void cheese(bool pressed,void *extra);

draw drawing;
input gameinput;
engine gameengine;
music gamemusic;
logger debugger;
int screenx=0,screeny=0;
int x=0,y=0;
int state=3;
int win,lose;
int title=0,startpic=0,exitpic=0;
int WIDTH=800,HEIGHT=600;
int arrowlocation=0;
int fps = 0;
int tempfps = 0;
int fps_time = 0;
bool keyokay=true;
bool initialized=false;
bool display_fps = true;
/*Control declarations*/
button start;
button exitbutton;
progressbar loadbar;
/*End of Control declarations*/

void ProgressBar(void) {
	drawing.Clear();
	drawing.StartDraw();
	loadbar.GiveMessage(CPROGRESSBAR_STEP,0,0);
	loadbar.Process();
	drawing.DrawText("Loading...",0,0,200,100,D3DCOLOR_ARGB(255,0,0,255));
	drawing.EndDraw();
	drawing.Flip();
}

void Run(void) {
	RECT rect = {0,0,32,32},titlerect;
	if (state!=3) {
		titlerect.top = 0;
		titlerect.left = 0;
		titlerect.right = drawing.GetTextureWidth(title);
		titlerect.bottom = drawing.GetTextureHeight(title);
	}
	switch(state) {
	case 0:
		{
			if (gameinput.KeyPressed(DIK_ESCAPE)) {running=false;return;}
			int startdown=0;
			drawing.StartDraw();
			drawing.DrawPicture(0,0,WIDTH,HEIGHT,title,&titlerect,false);
			drawing.SetFont(0);
			drawing.DrawText("Turret Defense v1.0",0,0,200,100,D3DCOLOR_ARGB(255,0,0,255));
			//drawing.DrawLine(0,y,WIDTH,y,D3DCOLOR_ARGB(255,0,128,0));
			//drawing.DrawLine(x,0,x,HEIGHT,D3DCOLOR_ARGB(255,0,128,0));
			start.Process();
			//start.GiveMessage(CBUTTON_GETDOWN,(int)&startdown,0);
			//start.Process();
			//if (startdown)
			//	state=1;
			//exitbutton.GiveMessage(CBUTTON_GETDOWN,(int)&startdown,0);
			//exitbutton.Process();
			//if (startdown)
			//	running=false;
			drawing.DrawPicture(gameinput.mousex-16,gameinput.mousey-16,32,32,target,&rect,true);
			if (display_fps)  
				drawing.DrawNumber(fps,WIDTH-100,0,D3DCOLOR_ARGB(255,0,0,255));
			drawing.EndDraw();
		}
		break;
	case 1:

		gameengine.Process();
		break;
	case 2:
		return;
		//Loading
	case 3:
		loadbar.GiveMessage(CPROGRESSBAR_SETMAX,8,0);
		loadbar.GiveMessage(CPROGRESSBAR_SETINCREMENT,1,0);
		loadbar.GiveMessage(CPROGRESSBAR_RESET,0,0);
		loadbar.GiveMessage(CPROGRESSBAR_SETBACKCOLOR,D3DCOLOR_ARGB(255,255,0,0),0);
		loadbar.GiveMessage(CPROGRESSBAR_SETFRONTCOLOR,D3DCOLOR_ARGB(255,0,255,0),0);
		loadbar.GiveMessage(CONTROL_SETPOS,0,HEIGHT/2);
		loadbar.GiveMessage(CONTROL_SETSIZE,WIDTH,25);
		loadbar.GiveMessage(CONTROL_ACTIVATE,1,0);

		//Set clear color to black
		drawing.SetClearColor(D3DCOLOR_XRGB(0,0,0));//Black
		//From here, use loading bar
		//Initialize fonts
		InitFonts();
		drawing.SetFont(0);
		ProgressBar();
		//Load music
		gamemusic.LoadMusic(L"music.mid",DMUS_SEG_REPEAT_INFINITE,GUID_StandardMIDIFile);
		ProgressBar();
		//Player music
		gamemusic.PlayMusic(0);
		ProgressBar();
		//Initialize game input
		gameinput.initialize(hwnd,0,0,WIDTH,HEIGHT);
		ProgressBar();
		//Load target.bmp
		target=drawing.LoadSurface("pictures/target.bmp");
		ProgressBar();
		//Load title.bmp
		title=drawing.LoadSurface("pictures/title.bmp");
		ProgressBar();
		//Initialize engine
		gameengine.Initialize();
		ProgressBar();
		//Set textures for start button
		start.QuickSet(0,400,drawing.LoadTexture("pictures/startup.png"),drawing.LoadTexture("pictures/startdown.png"),
			cheese,NULL,false);
		ProgressBar();
		state = 0;
		break;
	}
}
void Process(void) {
	try {
	HRESULT h;
	RECT rect = {0,0,32,32};
	if (state!=3) {

		if (!gameinput.Update())//If there's an error getting input...
			debugger.LogMessage("Process","Error obtaining input",logger::MSG_ERROR);
		//If F2 was pressed, toggle "display_fps".
		if (gameinput.WasKeyPressed(DIK_F2))
			display_fps ^= 1;
		x = gameinput.mousex;
		y = gameinput.mousey;
		if (x < 16) x=16;
		if (x >= drawing.width-16) x=drawing.width-17;
		if (y < 16) y=16;
		if (y >= drawing.height-16) y=drawing.height-17;
	}
	//  SetCursorPos(x,y);
	int temptime = timeGetTime();
	drawing.Clear();

	
	Run();
	if (!running)
		return;


	if (FAILED(h=drawing.Flip())) {
		string error = "Error flipping buffers: ";
		error += ErrorCodeRet(h); 
		debugger.LogMessage("Process()",error,logger::MSG_ERROR);
	}
	int temptime2 = timeGetTime();
	fps_time += temptime2-temptime;
	tempfps++;
	if (fps_time >= 1000) {
		fps = tempfps;
		tempfps = 0;
		fps_time = 0;
	}
	}
	catch (char *message) {
		running = false;
		debugger.LogMessage("Process()",message,logger::MSG_ERROR);
		return;
	}
	///catch (...) {
	///	running = false;
	///	debugger.LogMessage("Process()","An exception has occurred",logger::MSG_ERROR);
	///	return;
	//}
}
void InitFonts(void) {
	HRESULT h = drawing.AddFont(12,8,FW_REGULAR,false,"");
	if (FAILED(h)) {
		string error = "Error creating font: ";
		error += ErrorCodeRet(h); 
		debugger.LogMessage("InitFonts()",error,logger::MSG_ERROR);
	}
	drawing.SetFont(0);
}
ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEX wcex;
	
	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= NULL;
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= NULL;

	return RegisterClassEx(&wcex);
}


void cheese(bool pressed,void *extra) {
	if (pressed)
		state=1;
}

int _stdcall WinMain(HINSTANCE hinstance,HINSTANCE hprevinstance,LPSTR lpcmdline,int nshowcmd) {
	MSG msg;
	MyRegisterClass(hinstance);
	hwnd=CreateWindowEx(WS_EX_APPWINDOW|WS_EX_TOPMOST	,
		szWindowClass, "Turret Defense", WS_POPUP,
		0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
		NULL, NULL, hinstance, NULL);
	if (hwnd) {
		ShowWindow(hwnd,1);
		drawing.SetBackground(0xFF000000);
		debugger.Init("log.rtf");

		ShowCursor(NULL);

		drawing.initialize(hwnd);



		while (running) {
			if (PeekMessage(&msg,0,0,0,PM_NOREMOVE)) {
				if (!GetMessage(&msg,0,0,0)) running = false;
				else DispatchMessage(&msg);
			} else Process();
		}
	} else MessageBox(NULL,"Couldn't create window.",NULL,NULL);
	debugger.Close();
	return 0;
}

long _stdcall WndProc(HWND hwnd, unsigned int message, WPARAM wparam, LPARAM lparam) {
	switch (message) {
	case WM_DESTROY:
		DestroyWindow(hwnd);
		running=false;
		break;
	case WM_CLOSE:
		DestroyWindow(hwnd);
		running=false;
		break;
	//case WM_PAINT:
	//	ValidateRect( hwnd, NULL );
	//	break;
	default:
		return DefWindowProc(hwnd,message,wparam,lparam);
	
	}
	return TRUE;
}



