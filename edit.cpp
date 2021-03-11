// edit.cpp: implementation of the edit class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"
#include "misc.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern input gameinput;
extern draw drawing;
extern logger debugger;
edit::edit()
{
	caretx=0;
	carety=0;
	callback=NULL;
	active = true;
}

edit::~edit()
{

}

void edit::HandleMessages(void) {
	int size = messages.size();
	while (size--) {
		switch (messages[0].message) {
		case CONTROL_SETPOS:
			if	(messages[0].parameter1>0&&messages[0].parameter1<drawing.width&&(!parent||
				(messages[0].parameter1>parent->xpos()&&messages[0].parameter1<parent->xpos()+parent->GetWidth())))
				x = messages[0].parameter1;
			if	(messages[0].parameter2>0&&messages[0].parameter2<drawing.height&&(!parent||
				(messages[0].parameter2>parent->ypos()&&messages[0].parameter2<parent->ypos()+parent->GetHeight())))
				y = messages[0].parameter2;
			break;
		case CONTROL_SETSIZE:
			width=messages[0].parameter1;
			height=messages[0].parameter2;
			break;
		case CONTROL_ACTIVATE:
			active = messages[0].parameter1>0;
			break;
		case CEDIT_SETBGCOLOR:
			bgcolor=messages[0].parameter1;
			break;
		case CEDIT_SETTEXTCOLOR:
			textcolor=messages[0].parameter1;
			break;
		case CEDIT_SETTOPLEFTCOLOR:
			topleftcolor=messages[0].parameter1;
			break;
		case CEDIT_SETBOTTOMRIGHTCOLOR:
			bottomrightcolor=messages[0].parameter1;
			break;
		case CEDIT_SETCARETCOLOR:
			caretcolor=messages[0].parameter1;
			break;
		case CEDIT_GETTEXT:
			*(char**)messages[0].parameter1= text.text;
			break;
		case CEDIT_SETCALLBACK:
			callback = (void (*)(char*))messages[0].parameter1;
			break;
		default:
			break;//Do nothing for default
		}
		messages.pop_front();
	}
}

void edit::Process(void) {
	bool newbuf[256];
	HandleMessages();
	RECT rect = {0,0,width,height};
	
	RECT box = {xpos(),ypos(),xpos()+width,ypos()+height};
	if (active) {
		if (gameinput.leftclicked)
		focus = (gameinput.mousex>=xpos()&&gameinput.mousex<=(xpos()+width)&&
			gameinput.mousey>=ypos()&&gameinput.mousey<=(ypos()+height));
		
		drawing.DrawColor(xpos(),ypos(),xpos()+width,ypos()+height,bgcolor);
		drawing.DrawLine(xpos(),ypos(),xpos()+width,ypos(),topleftcolor);
		drawing.DrawLine(xpos(),ypos(),xpos(),ypos()+height,topleftcolor);
		drawing.DrawLine(xpos()+width,ypos()+height,xpos(),ypos()+height,bottomrightcolor);
		drawing.DrawLine(xpos()+width,ypos()+height,xpos()+width,ypos(),bottomrightcolor);

		if (focus) {
			bool found=false;
			unsigned char temp  =0;
			unsigned char result=0;
			while (1) {
				newbuf[temp]=gameinput.KeyPressed(temp);
				if (gameinput.KeyPressed(temp)) found=true;
				if (temp++==255) break;
			}
			if (found) 
			for (int temp=0;temp<256;temp++)
				if (!oldbuf[temp]&&newbuf[temp]) {
					if ((dxtoascii(temp,gameinput.KeyPressed(DIK_RSHIFT)||gameinput.KeyPressed(DIK_LSHIFT),&result))) {
						switch (result) {
						case '\b':
							text-=1;
							break;
						case '\n':
							break;
						case 0:
							break;
						default:
							text+=result;
							break;
						}
					}
					if (temp==DIK_RETURN&&callback) {
						callback(text.text);//If enter key has been pressed, call the callback function
						gameinput.SetKey(DIK_RETURN,0);
					}
				}
			memcpy(oldbuf,newbuf,sizeof(newbuf));
		}
		HDC hdc = drawing.Lock();
		SetTextColor(hdc,textcolor);
		SetBkMode(hdc,TRANSPARENT);
		DrawText(hdc,text.text,-1,&box,DT_EDITCONTROL|DT_WORDBREAK);
		drawing.Unlock();
		if (width&&text.text) {
			caretx = strlen(text.text)%width;
			carety = (strlen(text.text)/width)*12;
			if (caretx<xpos()&&caretx>xpos()+width&&carety>ypos()&&carety<ypos()+width)
				drawing.DrawColor(xpos()+caretx,ypos()+carety,4,12,RGB(192,192,192));
		}
	}
}