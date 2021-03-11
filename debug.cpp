// debug.cpp: implementation of the debug class.
//
//////////////////////////////////////////////////////////////////////

#include "debug.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

debug::debug()
{
	file=NULL;
	nameoffile=NULL;
	
}

void debug::SetFile(char *target) {
	char *temp = new char[strlen(target)+1];
	strcpy(temp,target);
	if (nameoffile) 
		delete [] nameoffile;
	nameoffile=temp;
	if (file) fclose(file);
	file = fopen(nameoffile,"w");
	fclose(file);
}
debug::~debug()
{
	if (nameoffile) delete [] nameoffile;
	if (file) fclose(file);
}
void debug::logdata(int data) {
	file = fopen(nameoffile,"a");
	fprintf(file,"%d.%d",errors++,data);
	fclose(file);
}
void debug::logdata(char *data) {
	file = fopen(nameoffile,"a");
	fprintf(file,"%d.%s",errors++,data);
	fclose(file);
}