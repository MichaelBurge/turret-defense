// music.h: interface for the music class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MUSIC_H__15E4614F_D99D_4F92_AC6D_B4A90C09C82D__INCLUDED_)
#define AFX_MUSIC_H__15E4614F_D99D_4F92_AC6D_B4A90C09C82D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class music  
{
	IDirectMusicPerformance8 *directmusic;
	IDirectMusicLoader8		 *loader;
	vector<IDirectMusicSegment8*> themusic;
	int current;
public:
	int LoadMusic(unsigned short *file,int repeat,_GUID type);
	void PlayMusic(int music);
	void StopMusic(void);
	music();
	~music();

};

#endif // !defined(AFX_MUSIC_H__15E4614F_D99D_4F92_AC6D_B4A90C09C82D__INCLUDED_)
