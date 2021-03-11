// strings.cpp: implementation of the strings class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern logger debugger;
strings::strings()
{
	text=NULL;
}
strings::strings(char *initialize) {
	text=NULL;
	*this=initialize;
}
strings::~strings()
{
	delete [] text;
}
strings &strings::operator=(char *string) {
	if (text) 
		delete [] text;
	char *temp=new char[strlen(string)+1];
	strcpy(temp,string);
	text = temp;//Copy the string to spot, and point to spot. Doesn't affect normal string if this happens
	return *this;
}
strings strings::operator+(char *string) {
	char *buffer = new char[strlen(text)+strlen(string)+1];
	strcpy(buffer,text);
	strcpy(buffer+strlen(text),string);
	buffer[strlen(text)+strlen(string)]=0;
	return strings::strings(buffer);
}
strings &strings::operator+=(char *string) {
	return *this=*this+string;
}
strings strings::operator+(int number) {
	int temp = 0;
	while (pow((double)temp,(double)2)<number) temp++;
	char *buffer = new char[temp*3+1];
	itoa(number,buffer,10);
	return *this+buffer;
}

strings strings::operator+(char character) {
	char *temp;
	if (text) {
	temp = new char[strlen(text)+2];//1 for NULL, and 1 for character
	strcpy(temp,text);
	temp[strlen(temp)-1]=character;
	temp[strlen(temp)]=0;
	} else {
		temp = new char[2];
		temp[0]=character;
		temp[1]=0;
	}
	return strings(temp);
}

strings &strings::operator-=(int number) {
	int length = strlen(text);
	if (text&&!(length-number+1)) return *this;
	char *temp = new char[length-number+1];
	for (int temp2=0;temp2<length&&temp2<length-number;temp2++)
		temp[temp2]=text[temp2];
	temp[length-number]=0;
	if (text)
		delete [] text;
	text = new char[strlen(temp)+1];
	strcpy(text,temp);
	return *this;
}

strings &strings::operator+=(char character) {
	int length=1;
	if (text) 
		length=strlen(text)+1;
	char *temp = new char[length+1];
	if (text) 
		strcpy(temp,text);
	temp[length-1]=character;
	temp[length]=0;
	delete [] text;
	text = temp;
	return *this;
}