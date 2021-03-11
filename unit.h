// unit.h: interface for the unit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UNIT_H__86540EF5_A75F_4677_88A1_1818D9F63663__INCLUDED_)
#define AFX_UNIT_H__86540EF5_A75F_4677_88A1_1818D9F63663__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class unit {
protected:
	sprite graphic;//The unit's sprite
	bool selected;//Whether the unit is selected or not
	POINT point;//Current location on the screen
	unsigned int steps;//Number of processings to go until destination reached
	unsigned int moves;//Number of moves left
	unsigned int maxmoves;//Number of moves at start
	double speed;//Speed of unit
	double direction;//Direction unit is travelling in
	unsigned int player;//The player owning the tank
	unsigned short facingdirection;
public:
	unsigned short GetFaceDirection(void) {
		return facingdirection;
	}
	void SetFaceDirection(unsigned short arg) {
		facingdirection = arg;
	}
	unsigned int GetPlayer(void) {
		return player;
	}
	void Reset(void) {
		moves = maxmoves;
	}
	void Select(bool value) {
		selected = value;
	}
	int GetX(void) {
		return point.x;
	}
	int GetY(void) {
		return point.y;
	}
	POINT GetPosition(void) {
		return point;
	}
	void SetX(int value) {
		point.x = value;
	}
	void SetY(int value) {
		point.y = value;
	}
	void SetPosition(POINT& value) {
		point = value;
	}
	bool IsSelected(void) {
		return selected;
	}
	virtual void Process(void);//Processes current state of unit
	virtual void Render(void);//Renders the unit
	virtual void Move(POINT& destination);//Sets the unit to move towards a location every processing
	unit();
	virtual ~unit();

};

#endif // !defined(AFX_UNIT_H__86540EF5_A75F_4677_88A1_1818D9F63663__INCLUDED_)
