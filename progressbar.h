// progressbar.h: interface for the progressbar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROGRESSBAR_H__7989B13A_5D7E_4EDE_AFE5_0C25D9FD2BBC__INCLUDED_)
#define AFX_PROGRESSBAR_H__7989B13A_5D7E_4EDE_AFE5_0C25D9FD2BBC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "control.h"
#include <d3d9.h>
enum {
	CPROGRESSBAR_SETMAX,
	CPROGRESSBAR_STEP,
	CPROGRESSBAR_SETINCREMENT,
	CPROGRESSBAR_RESET,
	CPROGRESSBAR_GETFINISHED,
	CPROGRESSBAR_GETSTEP,
	CPROGRESSBAR_GETINCREMENT,
	CPROGRESSBAR_GETMAX,
	CPROGRESSBAR_SETBACKCOLOR,
	CPROGRESSBAR_GETBACKCOLOR,
	CPROGRESSBAR_SETFRONTCOLOR,
	CPROGRESSBAR_GETFRONTCOLOR,
	CPROGRESSBAR_SETVERTICAL
};
class progressbar : public control {
	int max;
	int step;
	int increment;
	D3DCOLOR backcolor,frontcolor;
	bool vertical;
public:
	StandardId IsStandard(void) { return StandardId_ProgressBar;}
	void HandleMessages(void);
	void Process(void);
	progressbar();
	~progressbar();

};

#endif // !defined(AFX_PROGRESSBAR_H__7989B13A_5D7E_4EDE_AFE5_0C25D9FD2BBC__INCLUDED_)
