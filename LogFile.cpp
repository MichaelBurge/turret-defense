/*************************************************************************************************
 * LOGFILE.CPP                                                 Copyright (c) 2003 Eamonn Doherty *
 *                                                                                               *
 * Logging function implementations                                                              *
 *************************************************************************************************/
#include <string>
using namespace std;
#include "LogFile.h"

//********************************
//* Functions                    *
//********************************

//Open log file
int logger::Init (const string& sFile) {
    //Make sure log file is not already open
    if (bLoaded)
        return FALSE;

    //Open file
    hFile = CreateFile (sFile.c_str (), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,
        0);
    if (hFile == INVALID_HANDLE_VALUE)
        return FALSE;

    //Set file loaded flag
    bLoaded = TRUE;

    //Set filename
    sFilename = sFile;

    //Write RTF header
    WriteString ("{\\rtf1\\ansi\\deff0{\\fonttbl{\\f0 Courier New;}}\\fs20\n");

    //Write colour table
    WriteString ("{\\colortbl;\n");                     //Black
    WriteString ("\\red255\\green255\\blue255;\n");     //White
    WriteString ("\\red128\\green128\\blue128;\n");     //Grey
    WriteString ("\\red255\\green0\\blue0;\n");         //Red
    WriteString ("\\red0\\green255\\blue0;\n");         //Green
    WriteString ("\\red0\\green0\\blue255;\n");         //Blue
    WriteString ("\\red0\\green255\\blue255;\n");       //Cyan
    WriteString ("\\red255\\green255\\blue0;\n");       //Yellow
    WriteString ("\\red255\\green0\\blue255;\n");       //Magenta
    WriteString ("\\red128\\green0\\blue0;\n");         //Dark Red
    WriteString ("\\red0\\green128\\blue0;\n");         //Dark Green
    WriteString ("\\red0\\green0\\blue128;\n");         //Dark Blue
    WriteString ("\\red255\\green128\\blue128;\n");     //Light Red
    WriteString ("\\red128\\green255\\blue128;\n");     //Light Green
    WriteString ("\\red128\\green128\\blue255;\n");     //Light Blue
    WriteString ("\\red255\\green128\\blue0;\n");       //Orange
    WriteString ("}\n");

    //Write log header
    LogString ("*** Logging Started");
	LogString ("");

    //Success
    return TRUE;
}

//Close log file
int logger::Close (void) {
    //Make sure log file is already opened
    if (!bLoaded)
        return FALSE;

    //Write closing line
    LogString ("");
    LogString ("*** Logging Ended");

    //Write closing brace
    WriteString ("}");

    //Close file
    CloseHandle (hFile);

    //Clear file loaded flag
    bLoaded = FALSE;

    //Success
    return TRUE;
}


//Get log file state
int logger::GetState (void) {
    if (bLoaded)
        return TRUE;
    else
        return FALSE;
}


//Format and log a string
int logger::LogString (string sText) {
    //Make sure log file is already opened
    if (!bLoaded)
        return FALSE;

    //Format string
    DoFormatting (&sText);

    //Write string to file
    WriteString (sText);

    //Success
    return TRUE;
}


//Write a specially formatted message to the log file
int logger::LogMessage (const string &sSource, const string& sMessage, MESSAGETYPE messageType) {
    string sString;

    switch (messageType)
    {
    case MSG_SUCCESS:
        sString = "[BLUE]";
        break;

    case MSG_INFO:
        sString = "[GREY]";
        break;

    case MSG_WARN:
        sString = "[ORANGE]";
        break;

    case MSG_ERROR:
        sString = "[RED]";
        break;
    }

    sString += "[B]<";
    sString += sSource;
    sString += ">[/B] ";
    sString += sMessage;

    //Format the string and write it to the log file
    return LogString (sString);
}


//Replace formatting tags in a string with RTF formatting strings
void logger::DoFormatting (string* sText) {
    //Fix special symbols {, }, and backslash
    ReplaceTag (sText, "\\", "\\\\");
    ReplaceTag (sText, "{", "\\{");
    ReplaceTag (sText, "}", "\\}");

    //Colours
    ReplaceTag (sText, "[BLACK]", "\\cf0 ");
    ReplaceTag (sText, "[WHITE]", "\\cf1 ");
    ReplaceTag (sText, "[GREY]", "\\cf2 ");
    ReplaceTag (sText, "[RED]", "\\cf3 ");
    ReplaceTag (sText, "[GREEN]", "\\cf4 ");
    ReplaceTag (sText, "[BLUE]", "\\cf5 ");
    ReplaceTag (sText, "[CYAN]", "\\cf6 ");
    ReplaceTag (sText, "[YELLOW]", "\\cf7 ");
    ReplaceTag (sText, "[MAGENTA]", "\\cf8 ");
    ReplaceTag (sText, "[DARK_RED]", "\\cf9 ");
    ReplaceTag (sText, "[DARK_GREEN]", "\\cf10 ");
    ReplaceTag (sText, "[DARK_BLUE]", "\\cf11 ");
    ReplaceTag (sText, "[LIGHT_RED]", "\\cf12 ");
    ReplaceTag (sText, "[LIGHT_GREEN]", "\\cf13 ");
    ReplaceTag (sText, "[LIGHT_BLUE]", "\\cf14 ");
    ReplaceTag (sText, "[ORANGE]", "\\cf15 ");

    //Text style
    ReplaceTag (sText, "[PLAIN]", "\\plain ");
    ReplaceTag (sText, "[B]", "\\b ");
    ReplaceTag (sText, "[/B]", "\\b0 ");
    ReplaceTag (sText, "[I]", "\\i ");
    ReplaceTag (sText, "[/I]", "\\i0 ");
    ReplaceTag (sText, "[U]", "\\ul ");
    ReplaceTag (sText, "[/U]", "\\ul0 ");
    ReplaceTag (sText, "[S]", "\\strike ");
    ReplaceTag (sText, "[/S]", "\\strike0 ");
    ReplaceTag (sText, "[SUB]", "\\sub ");
    ReplaceTag (sText, "[/SUB]", "\\sub0 ");
    ReplaceTag (sText, "[SUPER]", "\\super ");
    ReplaceTag (sText, "[/SUPER]", "\\super0 ");
    ReplaceTag (sText, "[LEFT]", "\\ql ");
    ReplaceTag (sText, "[RIGHT]", "\\qr ");
    ReplaceTag (sText, "[CENTER]", "\\qc ");
    ReplaceTag (sText, "[FULL]", "\\qj ");
    ReplaceTag (sText, "[TITLE]", "\\fs40 ");
    ReplaceTag (sText, "[/TITLE]", "\\fs20 ");
    ReplaceTag (sText, "[H]", "\\fs24 ");
    ReplaceTag (sText, "[/H]", "\\fs20 ");

    //Add brackets and line formatting tags
    sText->insert (0, "{\\pard ");
    sText->insert (sText->length (), "\\par}\n");
}


//Replace all instances of a formatting tag with the RTF equivalent
void logger::ReplaceTag (string* sText, const string& sTag, const string& sReplacement) {
    int start = 0;

    while (sText->find(sTag, start) != string::npos)
    {
        start = (int) sText->find(sTag, start);
        sText->replace (start, sTag.length (), sReplacement);
        start += (int) sTag.length () + 1;
    }
}

//Write a string directly to the log file
void logger::WriteString (const string& sText) {
    DWORD bytesWritten;

    WriteFile (hFile, sText.c_str (), (int) sText.length (), &bytesWritten, NULL);
}