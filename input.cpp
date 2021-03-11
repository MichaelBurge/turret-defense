// input.cpp: implementation of the input class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"
#include "misc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern draw drawing;
extern logger debugger;
input::input() {
	mousex=320;
	mousey=240;
	leftclicked=false;
	rightclicked=false;
}
void input::initialize(HWND hWnd,int x2,int y2,int screenwidth2,int screenheight2)
{
	x=x2;
	y=y2;
	screenwidth=screenwidth2;
	screenheight=screenheight2;
	hwnd = hWnd;
	if (FAILED(DirectInput8Create(GetModuleHandle(0),DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)&directinput,NULL))) 
		debugger.LogMessage("input::initialize","Couldn't create a DirectInput object.",logger::MSG_ERROR);
	else {
		if (FAILED(directinput->CreateDevice(GUID_SysKeyboard,&keyboard,NULL))) 
			debugger.LogMessage("input::initialize","Couldn't create a keyboard device.",logger::MSG_ERROR);
		else if (FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard)))
			debugger.LogMessage("input::initialize","Couldn't set keyboard data format.",logger::MSG_ERROR);
		else if (FAILED(keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE )))
			debugger.LogMessage("input::initialize","Couldn't set keyboard cooperative level.",logger::MSG_ERROR);
		else keyboard->Acquire();
		if (FAILED(directinput->CreateDevice(GUID_SysMouse,&mouse,NULL)))
			debugger.LogMessage("input::initialize","Couldn't create a mouse device.",logger::MSG_ERROR);
		else if (FAILED(mouse->SetDataFormat(&c_dfDIMouse2)))
			debugger.LogMessage("input::initialize","Couldn't set mouse data format.",logger::MSG_ERROR);
		else if (FAILED(mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND| DISCL_EXCLUSIVE)))
			debugger.LogMessage("input::initialize","Couldn't set mouse cooperative level.",logger::MSG_ERROR);
		else mouse->Acquire();

	}
}

input::~input()
{
	if (keyboard) {
		keyboard->Unacquire();
		keyboard->Release();
		keyboard=NULL;
	}
	if (mouse) {
		mouse->Unacquire();
		mouse->Release();
		mouse=NULL;
	}
	if (directinput) {
		directinput->Release();
		directinput=NULL;
	}
}
bool input::Update(void) {
	HRESULT h;
	if (keyboard) {
		memcpy(previousbuffer,keyboardbuffer,sizeof(keyboardbuffer));
		h=keyboard->GetDeviceState(sizeof(keyboardbuffer),(void*)&keyboardbuffer);
	if (h==DIERR_INPUTLOST) {
		keyboard->Acquire();
		h=keyboard->GetDeviceState(sizeof(keyboardbuffer),(void*)&keyboardbuffer);
	}
	if (FAILED(h)) {
		string error = "Couldn't get keyboard state";
		error += ErrorCodeRet(h);
		debugger.LogMessage("input::Update()",error,logger::MSG_ERROR);
		return false;
	}

		for (int temp=0;temp<256;temp++)//Loop through, calling the callback function if the key is pressed
			if (callback&&keyboardbuffer[temp]&0x80)
				callback(temp);
	}
	if (mouse) {
		DIMOUSESTATE2	mousebuffer;
		h=mouse->GetDeviceState(sizeof(mousebuffer),&mousebuffer);
		if (h==DIERR_INPUTLOST) {
			mouse->Acquire();
			h=mouse->GetDeviceState(sizeof(mousebuffer),&mousebuffer);
		}
		if (FAILED(h)) return false;
		mousex+=mousebuffer.lX;
		mousey+=mousebuffer.lY;
		xdiff=  mousebuffer.lX;
		ydiff=  mousebuffer.lY;
		if (mousex<x) mousex=x;
		if (mousex > screenwidth) mousex=screenwidth;
		if (mousey<y) mousey=y;
		if (mousey > screenheight) mousey=screenheight;
		leftclicked=mousebuffer.rgbButtons[0]&0x80;
		rightclicked=mousebuffer.rgbButtons[1]&0x80;
		
	}
	return true;
}

int input::KeyPressed(int key) {
	return keyboardbuffer[key]&0x80;
}

void input::SetCallback(void (*target)(char)) {
	callback=target;
}

void input::SetKey(int key,int value) {
	keyboardbuffer[key]=value;
}