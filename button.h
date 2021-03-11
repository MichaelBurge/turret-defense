// button.h: interface for the button class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BUTTON_H__945D8A57_01D4_47CA_BC00_C5AE4865DCE1__INCLUDED_)
#define AFX_BUTTON_H__945D8A57_01D4_47CA_BC00_C5AE4865DCE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int CBUTTON_SETUP =0;
const int CBUTTON_SETDOWN =1;
const int CBUTTON_SETCALLBACK=2;
const int CBUTTON_GETDOWN=3;
const int CBUTTON_SETEXTRA=4;
const int CBUTTON_SOLID=5;
extern draw drawing;
class button:public control {
	int uppicture,downpicture;
	void (*callback)(bool pressed,void *);
	void *extra;
	char *callbackname;
	bool pressed;
	bool solidity;
public:
	//Quickly sets information for the button control
	void QuickSet(int thex,int they,int theuppicture,int thedownpicture,void (*thecallback)(bool,void*),
		void *theextra,bool thesolidity) {
		x           = thex;
		y           = they;
		width       = drawing.GetTextureWidth (theuppicture);
		height      = drawing.GetTextureHeight(theuppicture);
		uppicture   = theuppicture;
		downpicture = thedownpicture;
		callback    = thecallback;
		extra       = theextra;
		solidity    = thesolidity;
	}
	void HandleMessages(void);//Handles control messages
	StandardId IsStandard(void) {return StandardId_Button;};//Returns that it is a standard control(button)
	button();
	char *Save(void);
	char *Load(void);
	void virtual Process();
	~button();

};

#endif // !defined(AFX_BUTTON_H__945D8A57_01D4_47CA_BC00_C5AE4865DCE1__INCLUDED_)
