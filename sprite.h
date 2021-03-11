// sprite.h: interface for the sprite class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITE_H__8DD966AB_A93F_43C1_BCD1_73356CD67819__INCLUDED_)
#define AFX_SPRITE_H__8DD966AB_A93F_43C1_BCD1_73356CD67819__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class sprite {
	unsigned short current;
	unsigned int width,height;
	deque<int> pictures;
public:
	unsigned int GetWidth(void) {
		return width;
	}
	unsigned int GetHeight(void) {
		return height;
	}
	void AddPicture(const char *file);
	void AddPicture(int picture);
	void AddPictureRange(char *basefile,unsigned short amount);
	unsigned int GetPictureCount(void) {
		return pictures.size();
	}
	int GetPictureIndex(int index) {
		return pictures[index];
	}
	HRESULT Draw(unsigned int x,unsigned int y);
	void SetCurrent(unsigned short newindex) {
		current=newindex;
	}
	unsigned short GetCurrent(void) {
		return current;
	}
	sprite(SIZE& size) {
		current=0;
		width=size.cx;
		height=size.cy;
	}
	sprite(void);
	~sprite();

};

#endif // !defined(AFX_SPRITE_H__8DD966AB_A93F_43C1_BCD1_73356CD67819__INCLUDED_)
