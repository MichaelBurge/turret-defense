#include <windows.h>
namespace algorithms {
	bool TileInCircle(POINT& circlestart,unsigned int radius,POINT& tilepoint) {
		//Start with a line length of 1, at 'radius' tiles above the start
		unsigned int linelength = 1;
		unsigned int xstart = circlestart.x;
		unsigned int ystart = circlestart.y - radius;
		//For every line until the line length reaches (radius*2)+1 (to compensate for the middle tile)
		unsigned int widest = (radius*2)+1;
		while (linelength<widest) {
			//Check tiles in a line going right
			for (unsigned int temp=0;temp<linelength;temp++)
				//If found, return true
				if (xstart+temp == (unsigned int)tilepoint.x && ystart == (unsigned int)tilepoint.y)
					return true;
				//Increment the line length by 2
				linelength+=2;
				//Go down and left 1 tile
				xstart--;
				ystart++;
		}
		//Adds one more iteration to next
		linelength+=2;
		//If we reach radius*2, and haven't found it
		//For every line until the length length reaches 1
		while (linelength>1) {
			//Decrement the line length by 2
			linelength-=2;
			//Check tiles in a line going right
			for (unsigned int temp=0;temp<linelength;temp++)
				//If found, return true
				if (xstart+temp == (unsigned int)tilepoint.x && ystart == (unsigned int)tilepoint.y)
					return true;
				
				//Go down and right 1 tile
				xstart++;
				ystart++;
		}
		
		//If we've searched the entire area, and haven't found it, return false
		return false;
	}
}