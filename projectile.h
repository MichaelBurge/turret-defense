// projectile.h: interface for the projectile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROJECTILE_H__0BF18425_14CF_4BBA_8BEB_27A1BF061CA8__INCLUDED_)
#define AFX_PROJECTILE_H__0BF18425_14CF_4BBA_8BEB_27A1BF061CA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <deque>
#include <windows.h>
using std::deque;
#include "sprite.h"
#include "math.h"

class projectile  {
public:
	enum ProjectileMessage {TargetReached,Exploded};
private:
	sprite picture,explosion;
	double speed;
	int x,y;
	int targetx,targety;
	void *extra;
	int time;
	bool active;
	bool exploding;
	typedef void (*ProjectileCallback)(ProjectileMessage message,projectile& theprojectile,void *extra);
	ProjectileCallback callback;
public:
	
	void Create(sprite& thepicture,sprite& theexplosion,ProjectileCallback thecallback,void *theextra) {
		picture = thepicture;
		explosion = theexplosion;
		x=0;
		y=0;
		targetx=0;
		targety=0;
		callback = thecallback;
	}
	void Activate(POINT position,POINT target,double thespeed,int explodetime) {
		x = position.x;
		y = position.y;
		targetx=target.x;
		targety=target.y;
		speed = thespeed;
		time = explodetime;
		active = true;
	}
	void Process(void);
	void Render(void);
	projectile();
	~projectile();

};

#endif // !defined(AFX_PROJECTILE_H__0BF18425_14CF_4BBA_8BEB_27A1BF061CA8__INCLUDED_)
