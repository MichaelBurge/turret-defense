// tank.h: interface for the tank class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TANK_H__AEC2B4CC_1ABE_41FD_875A_3109FE4A3C71__INCLUDED_)
#define AFX_TANK_H__AEC2B4CC_1ABE_41FD_875A_3109FE4A3C71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class tank: public unit {
	sprite graphics[4];
public:
	void Process(void);
	void Render(void);
	tank(POINT& location,sprite* thegraphic,unsigned int theplayer) {
		point.x = location.x;
		point.y = location.y;
		//thegraphic consists of an array of 4 sprites
		for (unsigned char temp=0;temp<4;temp++)
			graphics[temp] = *thegraphic++;
		maxmoves = 3;
		moves = 3;
		speed = 4.0;
		player = theplayer;
	};
	~tank();
};
#endif // !defined(AFX_TANK_H__AEC2B4CC_1ABE_41FD_875A_3109FE4A3C71__INCLUDED_)
