// debug.h: interface for the debug class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEBUG_H__94EF589A_DF7E_49B3_8D50_68DE9A6A3A14__INCLUDED_)
#define AFX_DEBUG_H__94EF589A_DF7E_49B3_8D50_68DE9A6A3A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <stdio.h>

#define CRITICAL(statement) if (debugger.priority>=debugger.Critical) {statement;}
#define IMPORTANT(statement) if (debugger.priority>=debugger.Important) {statement;}
#define MEDIUM(statement) if (debugger.priority>=debugger.Medium) {statement;}
#define TRIVIAL(statement) if (debugger.priority>=debugger.Trivial) {statement;}

class debug  
{
	char *nameoffile;
	FILE *file;
	unsigned int errors;
public:
	enum {Critical,Important,Medium,Trivial};
	int priority;
	void SetFile(char *target);
	void logdata(char *);
	void logdata(int);
	debug();
	virtual ~debug();

};
#include "main.h"
#endif // !defined(AFX_DEBUG_H__94EF589A_DF7E_49B3_8D50_68DE9A6A3A14__INCLUDED_)
