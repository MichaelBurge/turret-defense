#include <windows.h>
#include <math.h>//C standard library math
#include "math.h"//Local math header
int GetDirection(int x,int y,int x2,int y2) {
	return 360-(radtodeg(atan2((double)(y-y2),(double)(x-x2)))+180);
}

double DistanceToPoint(int x,int y,int x2,int y2) {
	double result=sqrt((double)(x-x2)*(x-x2)+(y-y2)*(y-y2));
	if (result<0) result*=-1;
	return result;
}
double radtodeg(double rad) {
	return (rad*180)/3.1415926535897932384626433832795;
}
double degtorad(double deg) {
	return (deg*3.1415926535897932384626433832795)/180;
}

POINT MoveInDirection(int x,int y,double direction,double speed) {
	x+=speed*cos(degtorad(direction));
	y-=speed*sin(degtorad(direction));
	POINT ret = {x,y};
	return ret;
}