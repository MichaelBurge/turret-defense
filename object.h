// object.h: interface for the object class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJECT_H__87A80041_6965_4C95_91D4_1BCE8C8A1BD4__INCLUDED_)
#define AFX_OBJECT_H__87A80041_6965_4C95_91D4_1BCE8C8A1BD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class object {
public:
	enum SentMessage {Message_Position,Message_EndOfSprite};
private:
	int screenwidth,screenheight;
	sprite objectsprite;
	SIZE (*callback)(object*,SentMessage,void*);
	double direction;
	double speed;
	void *extra;
public:

	int x,y;
	void Process(void);
	void Draw(unsigned int screenx,unsigned int screeny);
	object(sprite thesprite,int thescreenwidth,int thescreenheight,SIZE (*thecallback)(object*,SentMessage,void*)=NULL,void *theextra=NULL) {
		objectsprite = thesprite;
		screenwidth=thescreenwidth;
		screenheight=thescreenheight;
		callback=thecallback;
		speed = 1.0;
		direction=0.0;
		extra = theextra;
	}
	~object();

};

#endif // !defined(AFX_OBJECT_H__87A80041_6965_4C95_91D4_1BCE8C8A1BD4__INCLUDED_)
