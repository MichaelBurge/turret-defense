// edit.h: interface for the edit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EDIT_H__32465898_6F38_40E2_8ABB_79A13A9F6ED3__INCLUDED_)
#define AFX_EDIT_H__32465898_6F38_40E2_8ABB_79A13A9F6ED3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int CEDIT_SETBGCOLOR			=0;
const int CEDIT_SETTEXTCOLOR		=1;
const int CEDIT_SETTOPLEFTCOLOR		=2;
const int CEDIT_SETBOTTOMRIGHTCOLOR	=3;
const int CEDIT_SETCARETCOLOR		=4;
const int CEDIT_GETTEXT				=5;
const int CEDIT_SETCALLBACK			=6;

class edit:public control {
	strings text;
	char buffer[256];
	bool oldbuf[256];
	COLORREF bgcolor;
	COLORREF textcolor;
	COLORREF topleftcolor;
	COLORREF bottomrightcolor;
	COLORREF caretcolor;
	void (*callback)(char *);
	bool focus;
	int caretx,carety;
public:
	void HandleMessages(void);
	StandardId IsStandard(void) {return StandardId_Edit;};
	void Process(void);
	edit();
	~edit();

};

#endif // !defined(AFX_EDIT_H__32465898_6F38_40E2_8ABB_79A13A9F6ED3__INCLUDED_)


