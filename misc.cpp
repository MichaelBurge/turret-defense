#include "main.h"
bool dxtoascii(char character,bool shift,unsigned char *dest) {
	bool ret=true;
	switch (character) {
	case DIK_0:
		character=shift?')':'0';
		break;
	case DIK_1:
		character=shift?'!':'1';
		break;
	case DIK_2:
		character=shift?'@':'2';
		break;
	case DIK_3:
		character=shift?'#':'3';
		break;
	case DIK_4:
		character=shift?'$':'4';
		break;
	case DIK_5:
		character=shift?'%':'5';
		break;
	case DIK_6:
		character=shift?'^':'6';
		break;
	case DIK_7:
		character=shift?'&':'7';
		break;
	case DIK_8:
		character=shift?'*':'8';
		break;
	case DIK_9:
		character=shift?'(':'9';
		break;
	case DIK_A:
		character=shift?'A':'a';
		break;
	case DIK_B:
		character=shift?'B':'b';
		break;
	case DIK_C:
		character=shift?'C':'c';
		break;
	case DIK_D:
		character=shift?'D':'d';
		break;
	case DIK_E:
		character=shift?'E':'e';
		break;
	case DIK_F:
		character=shift?'F':'f';
		break;
	case DIK_G:
		character=shift?'G':'g';
		break;
	case DIK_H:
		character=shift?'H':'h';
		break;
	case DIK_I:
		character=shift?'I':'i';
		break;
	case DIK_J:
		character=shift?'J':'j';
		break;
		
	case DIK_K:
		character=shift?'K':'k';
		break;
		
	case DIK_L:
		character=shift?'L':'l';
		break;
		
	case DIK_M:
		character=shift?'M':'m';
		break;
	case DIK_N:
		character=shift?'N':'n';
		break;
	case DIK_O:
		character=shift?'O':'o';
		break;
	case DIK_P:
		character=shift?'P':'p';
		break;
	case DIK_Q:
		character=shift?'Q':'q';
		break;
	case DIK_R:
		character=shift?'R':'r';
		break;
	case DIK_S:
		character=shift?'S':'s';
		break;
	case DIK_T:
		character=shift?'T':'t';
		break;
	case DIK_U:
		character=shift?'U':'u';
		break;
	case DIK_V:
		character=shift?'V':'v';
		break;
	case DIK_W:
		character=shift?'W':'w';
		break;
	case DIK_X:
		character=shift?'X':'x';
		break;
	case DIK_Y:
		character=shift?'Y':'y';
		break;
	case DIK_Z:
		character=shift?'Z':'z';
		break;
	case DIK_APOSTROPHE:
		character=shift?'\"':'\'';
		break;
	case DIK_AT:
		character='@';
		break;
	case DIK_BACK:
		character='\b';
		break;
	case DIK_ADD:
		character='+';
		break;
	case DIK_BACKSLASH:
		character=shift?'|':'\\';
		break;
	case DIK_COLON:
		character=':';
		break;
	case DIK_COMMA:
		character=shift?'<':',';
		break;
	case DIK_DIVIDE:
		character='/';
		break;
	case DIK_EQUALS:
		character=shift?'+':'=';
		break;
	case DIK_GRAVE:
		character=shift?'~':'`';
		break;
	case DIK_LBRACKET:
		character=shift?'{':'[';
		break;
	case DIK_MINUS:
		character=shift?'_':'-';
		break;
	case DIK_MULTIPLY:
		character='*';
		break;
	case DIK_NUMPAD0:
		character='0';
		break;
	case DIK_NUMPAD1:
		character='1';
		break;
	case DIK_NUMPAD2:
		character='2';
		break;
	case DIK_NUMPAD3:
		character='3';
		break;
	case DIK_NUMPAD4:
		character='4';
		break;
	case DIK_NUMPAD5:
		character='5';
		break;
	case DIK_NUMPAD6:
		character='6';
		break;
	case DIK_NUMPAD7:
		character='7';
		break;
	case DIK_NUMPAD8:
		character='8';
		break;
	case DIK_NUMPAD9:
		character='9';
		break;
	case DIK_NUMPADCOMMA:
		character=',';
		break;
	case DIK_NUMPADEQUALS:
		character='=';
		break;
	case DIK_PERIOD:
		character=shift?'>':'.';
		break;
	case DIK_RBRACKET:
		character=shift?'}':']';
		break;
	case DIK_SEMICOLON:
		character=shift?':':';';
		break;
	case DIK_SLASH:
		character=shift?'?':'/';
		break;
	case DIK_SPACE:
		character=' ';
		break;
	case DIK_SUBTRACT:
		character='-';
		break;
	default:
		ret=false;
		break;
	}
	*dest=character;
	return ret;			
}

char *ErrorCodeRet(int error) {
	switch (error) {
		/*
		case DD_OK:
			return "For some reason, error is DD_OK. No problem here...";
			break;
		case DDERR_INVALIDOBJECT:
			return "Invalid object passed";
			break;
		case DDERR_INVALIDPARAMS:
			return "Invalid parameters passed";
			break;
		case DDERR_OUTOFVIDEOMEMORY:
			return "Out of video memory";
			break;
		case DDERR_NOCOOPERATIVELEVELSET:
			return "No cooperative level set";
			break;
		case DDERR_INVALIDCAPS:
			return "One or more of the caps bits passed to the callback are incorrect.";
			break;
		case D3DERR_INVALIDCALL:
			return "Invalid call";
			break;
		*/
		case D3DXERR_INVALIDDATA:
			return "Invalid data";
			break;
		case D3DERR_DEVICELOST:
			return "Device Lost";
			break;
		case D3DERR_DRIVERINTERNALERROR :
			return "Driver Error";
			break;
		case DIERR_INPUTLOST:
			return "Input lost";
			break;
		case DIERR_NOTACQUIRED:
			return "Device not acquired";
			break;
		case DIERR_NOTINITIALIZED:
			return "Not initialized";
			break;
		case E_PENDING:
			return "Data is not yet available.";
			break;
		default:
			return "Unknown problem";
	}
}