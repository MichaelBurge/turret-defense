// text.h: interface for the text class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXT_H__BF4C6C2A_2BFE_4960_BBC5_80BCDF197BAF__INCLUDED_)
#define AFX_TEXT_H__BF4C6C2A_2BFE_4960_BBC5_80BCDF197BAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
const int CTEXT_SETTEXT =0;
const int CTEXT_SETCOLOR=1;
class text:public control
{
	char *string;
	COLORREF textcolor;
public:
	void HandleMessages(void);
	StandardId IsStandard(void) {return StandardId_Text;}
	text();
	void Process(void);
	~text();

};

#endif // !defined(AFX_TEXT_H__BF4C6C2A_2BFE_4960_BBC5_80BCDF197BAF__INCLUDED_)
