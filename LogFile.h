/*************************************************************************************************
 * LOGFILE.H                                                   Copyright (c) 2003 Eamonn Doherty *
 *                                                                                               *
 * Logging function prototypes                                                                   *
 *************************************************************************************************/

/*
LOGFILE SYNTAX DESCRIPTION:

The log file outputs logging information to an RTF file. C-style file I/O is used because it is
slightly faster than iostream. Text passed to the writer may use the following formatting tags:

[<colour>]              -   Text after this tag will be coloured specified colour.
<colour> can be: BLACK, WHITE, GREY, RED, GREEN, BLUE, CYAN, YELLOW,
MAGENTA, DARK_RED, DARK_GREEN, DARK_BLUE, LIGHT_RED, LIGHT_GREEN,
LIGHT_BLUE, ORANGE.

[PLAIN]                 -   Text after this tag will be unformatted

[B], [/B]               -   Text in between these two tags will be bolded

[I], [/I]               -   Text in between these two tags will be italicized

[U], [/U]               -   Text in between these two tags will be underlined

[S], [/S]               -   Text in between these two tags will have a line through it

[SUB], [/SUB]           -   Text in between these two tags will be a subscript

[SUPER], [/SUPER]       -   Text in between these two tags will be a superscript

[LEFT]                  -   Text will be left-justified (this is the default justification)

[RIGHT]                 -   Text will be right-justified

[CENTER]                -   Text will be center-justified

[FULL]                  -   Text will be full-justified

[TITLE], [/TITLE]       -   Text in between these two tags will be size 20

[H], [/H]               -   Text in between these two tags will be size 12

*/

#pragma once

#include <Windows.h>

//Constant for disabling logging
const int LOGGING_DISABLED = 0x00330099;

//Logging definitions
#ifdef ENABLE_LOGGING
    #define openLog(fileName) CLogFile::Init(fileName)
    #define closeLog CLogFile::Close()
    #define writeLog(s) CLogFile::LogString(s)

    #define msgLog(message, type) CLogFile::LogMessage(__FUNCTION__, message, type)
    #define infoLog(message) CLogFile::LogMessage(__FUNCTION__, message, CLogFile::MSG_INFO)
    #define successLog(message) CLogFile::LogMessage(__FUNCTION__, message, CLogFile::MSG_SUCCESS)
    #define warnLog(message) CLogFile::LogMessage(__FUNCTION__, message, CLogFile::MSG_WARN)
    #define errorLog(message) CLogFile::LogMessage(__FUNCTION__, message, CLogFile::MSG_ERROR)

#else
    #define openLog(fileName) LOGGING_DISABLED
    #define closeLog LOGGING_DISABLED
    #define writeLog(s)
    #define msgLog(message, type)
    #define infoLog(message)
    #define successLog(message)
    #define warnLog(message)
    #define errorLog(message)
#endif

//********************************
//* Prototypes                   *
//********************************

//-----------------------------------------------------
//The following should be public in your logging class
//-----------------------------------------------------
class logger {
public:
	//Log message types
	enum MESSAGETYPE
	{
		MSG_SUCCESS = 1,	
			MSG_INFO = 2,
			MSG_WARN = 3,
			MSG_ERROR = 4,
	};
	
	//Open file for logging
	int Init (const string& sFile);
	
	//Close logging file
	int Close (void);
	
	//Returns current state of logging class
	int GetState(void);
	
	//Format and write a string to the log file
	int LogString (string sText);
	
	//Write a specially formatted message to the logfile
	int LogMessage (const string &sSource, const string& sMessage, MESSAGETYPE messageType);
	
	
	//-----------------------------------------------------
	//The following should be private in your logging class
	//-----------------------------------------------------
private:
	//RTF file's colour table
	enum COLOURTABLE {
			BLACK = 0,
			WHITE = 1,
			GREY = 2,
			RED = 3,
			GREEN = 4,
			BLUE = 5,
			CYAN = 6,
			YELLOW = 7,
			MAGENTA = 8,
			DARK_RED = 9,
			DARK_GREEN = 10,
			DARK_BLUE = 11,
			LIGHT_RED = 12,
			LIGHT_GREEN = 13,
			LIGHT_BLUE = 14,
			ORANGE = 15,
	};
	
	//Add RTF formatting to a string using formatting tags
	void DoFormatting (string* sText);
	
	//Replace all instances of a tag in a string with equivalent RTF formatting
	void ReplaceTag (string* sText, const string& sTag, const string& sReplacement);
	
	//Writes a string directly to the logfile
	void WriteString (const string& sText);
	
	//File loaded flag
	BOOL bLoaded;
	
	//Handle to the file
	HANDLE hFile;
	
	//Filename
	string sFilename;
};