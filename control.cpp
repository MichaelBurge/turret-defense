// control.cpp: implementation of the control class.
//
//////////////////////////////////////////////////////////////////////

#include "main.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


void control::GiveMessage(int message,int parameter1,int parameter2) {
	Message themessage;
	themessage.message=message;
	themessage.parameter1=parameter1;
	themessage.parameter2=parameter2;
	messages.push_back(themessage);
}

int control::AddChild(control *target) {
	children.push_back(target);//Add to list of child controls
	target->SetParent(this);//Set child's parent to self
	return children.size();//The added child is on the end, so return that number
}

void control::SetChild(int child,control *target) {
	children[child]->SetParent(NULL);//Clear old child's parent
	children[child]=target;//Set old child to target
	target->SetParent(this);//Set new child's parent to self
}

void control::RemoveChild(int child) {
	children.erase(children.begin()+child);
}