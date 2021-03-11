// strings.h: interface for the strings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGS_H__DBB0EC83_0C7E_474B_B13C_9EEC52FB6C56__INCLUDED_)
#define AFX_STRINGS_H__DBB0EC83_0C7E_474B_B13C_9EEC52FB6C56__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class strings  
{
public:
	strings &operator=(char *string);
	strings operator+(char *string);
	strings &operator+=(char *string);
	strings operator+(int number);
	strings operator+(char character);
	strings &operator+=(char character);
	strings &operator-=(int number);
	char *text;
	strings();
	strings(char *text);
	~strings();

};

#endif // !defined(AFX_STRINGS_H__DBB0EC83_0C7E_474B_B13C_9EEC52FB6C56__INCLUDED_)
