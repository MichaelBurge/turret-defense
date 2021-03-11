// draw.cpp: implementation of the draw class.
//
//////////////////////////////////////////////////////////////////////
#include "main.h"
#include "misc.h"
extern logger debugger;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
bool fullscreen = true;
draw::draw() {
	clearcolor=RGB(256,256,256);
	width=WIDTH;
	height=HEIGHT;
	background=0;
	target=false;
	folder="";
	currentfont=-1;
	modulationcolor = 0xFFFFFFFF;
}

draw::~draw()
{
	kill();
}

void draw::initialize(HWND hwnd) {
	int error =  0;
	
	bool running=true;
	char *message = NULL;
	if (NULL == (direct3d = Direct3DCreate9(D3D_SDK_VERSION))) {//Creates D3D object
		MessageBox(NULL,"Couldn't create Direct3D object.","Error",0);
		debugger.LogMessage("draw::init","Couldn't create D3D.",logger::MSG_ERROR);
		running = false;//Program can't work if D3D isn't working
	}
	if (running) {

		if (FAILED(error = direct3d->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&displaymode))) {
			MessageBox(NULL,"Couldn't get info on your display mode adapter.",NULL,0);
			debugger.LogMessage("draw::init","Couldn't get info on display adapter",logger::MSG_ERROR);
			running = false;
		}
		D3DPRESENT_PARAMETERS d3dpp; //Get parameters working
		ZeroMemory( &d3dpp, sizeof(d3dpp) );
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.BackBufferFormat = displaymode.Format;
		d3dpp.BackBufferWidth = WIDTH;
		d3dpp.BackBufferHeight = HEIGHT;
		d3dpp.BackBufferCount = 3;
		d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
		d3dpp.MultiSampleQuality = 0;
		d3dpp.hDeviceWindow = hwnd;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.Flags = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL|D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;
		d3dpp.FullScreen_RefreshRateInHz = 0;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;


		if (running) {
			//Test if we can make a HAL device
			if (SUCCEEDED(error = direct3d->CheckDeviceType(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,displaymode.Format,displaymode.Format,TRUE))) {
				if( FAILED( error = direct3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,//Make device for D3D
					D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &device ) ) ) //Try again if failed
					//D3D sets backbuffercount value in paramaters to what is possible, and fails. Try again with new count...
					if( FAILED( error = direct3d->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,//Make device for D3D
						D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp, &device ) ) ) {
					message = ErrorCodeRet(error);//If device created failed, display error
					debugger.LogMessage("draw::init","D3D device initialization failed.",logger::MSG_ERROR);
					debugger.LogMessage("draw::init",message,logger::MSG_ERROR);
					running = false;
				}
			} else {
				message = ErrorCodeRet(error);//If we can't check make a HAL device, log an error.
				debugger.LogMessage("draw::init","Hardware Abstraction Layer device can't be made on your comp.",logger::MSG_ERROR);
				debugger.LogMessage("draw::init",message,logger::MSG_ERROR);
				running = false;
			}
		}
	}

	//Check if all is currently okay. If not, exit.
	if (running==false)
		goto end;
	//Set various settings
	error = device->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );//Set culling to 'None'
	error= device->SetRenderState( D3DRS_LIGHTING, FALSE );//No lighting
	if (FAILED(error))
		running=false;

	//Check if all is currently okay. If not, exit.
	if (running==false)
		goto end;

	//Create the ID3DXSprite object so we can draw pictures with it
	if (FAILED(error=D3DXCreateSprite(device,&sprite))) {
		message = ErrorCodeRet(error);
		debugger.LogMessage("draw::init","Couldn't create sprite object.",logger::MSG_ERROR);
		debugger.LogMessage("draw::init",message,logger::MSG_ERROR);
		running = false;
	}
	//Create line drawer object
	if (FAILED(error=D3DXCreateLine(device,&line))) {
		string error2 = "Couldn't create line: ";
		error2 += ErrorCodeRet(error);
		debugger.LogMessage("draw::init",error2,logger::MSG_ERROR);
		running=false;
	}

end:
	//Log a successful message if everything went okay
	if (running)
		debugger.LogMessage("draw::init","Finished initialization.",logger::MSG_SUCCESS);
	else 
		debugger.LogMessage("draw::init","Error initializing.",logger::MSG_ERROR);
}
void draw::kill(void) {
	for (int temp=fonts.size();temp;temp--) {
		fonts[temp-1]->Release();
		fonts.pop_back();
	}
	for (temp=textures.size();temp;temp--) {
		textures[temp-1]->Release();
		textures.pop_back();
	}
	if (sprite)
		sprite->Release();
	if (line)
		line->Release();
	if (device)//If device exists...
		device->Release();//Release it
	device=NULL;
	if (direct3d)
		direct3d->Release();
	direct3d = NULL;
}
//Outdated function. Instead of loading to surface, now calls LoadTexture to load to texture.
int draw::LoadSurface(const char *file) {
	return LoadTexture(file);
}
//Function loads a picture into a texture. Argument is filename of picture.
int draw::LoadTexture(const char *file) {
	picturesource.push_back(file);//Push back filename of picture

	D3DXIMAGE_INFO imageinfo;//Declare structure for image information

	//Creates the texture
	IDirect3DTexture9 *texture;//Declare texture
	int check = D3DXCreateTextureFromFileEx(//Load texture
		device,//D3D device
		file,//Load file passed to LoadTexture() function
		D3DX_DEFAULT,//Read width from file
		D3DX_DEFAULT,//Read height from file
		D3DX_DEFAULT,//Set mipmap level to default
		NULL,//Other possibilities are D3DUSAGE_RENDERTARGET,or D3DUSAGE_DYNAMIC. We don't use those, so set to NULL.
		D3DFMT_UNKNOWN,//Take format from file
		D3DPOOL_MANAGED,//Set memory type to 'managed'
		D3DX_DEFAULT,//Set filter type to default
		D3DX_DEFAULT,//Set mipfilter to default
		background,//Set colorkey value to current background color
		&imageinfo,//Store image information for later
		NULL,//Don't use palette information
		&texture);//'texture' is the returned texture
	if (FAILED(check)) {
		string debugstring = "Couldn't load picture: ";
		debugstring+=file;
		debugger.LogMessage("draw::LoadTexture",debugstring,logger::MSG_ERROR);
	}
	textures.push_back(texture);//Push pointer to texture to back
	textureinfo.push_back(imageinfo);//Push image information 
	return textures.size()-1;//Return index to picture
}
//Draw the full picture at a spot
BOOL draw::DrawPicture(int x, int y, int picture, RECT *rect) {
	if (drawing==false) {
		StartDraw();
		drawing=false;
	}
	HRESULT h;
	//Calculate matrix
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix,NULL,NULL,NULL,NULL,NULL,NULL);
	//Set transformation matrix into sprite
	sprite->SetTransform(&matrix);
	//Draw picture
	h = sprite->Draw(textures[picture],rect,NULL,&D3DXVECTOR3(x,y,0),modulationcolor);
	//Draw picture.
	if (FAILED(h)) {
		string debugstring = "Couldn't draw picture: ";
		debugstring+=picturesource[picture];
		debugger.LogMessage("draw::DrawPicture",debugstring,logger::MSG_SUCCESS);
	}
	if (drawing==false)
		EndDraw();
	return h;
}
//Draw a scaled picture
BOOL draw::DrawPicture(int x, int y, int thewidth,int theheight,int picture, RECT *rect,bool alpha) {
	if (drawing==false) {
		StartDraw();
		drawing=false;
	}
	HRESULT h;
	//Calculate scaling vector
	D3DXVECTOR2 result(((float)thewidth)/(rect->right-rect->left),((float)theheight)/(rect->bottom-rect->top));
	//Calculate matrix
	D3DXMATRIX matrix;
	D3DXMatrixTransformation2D(&matrix,NULL,NULL,&result,NULL,NULL,0);
	//Set transformation matrix into sprite
	sprite->SetTransform(&matrix);
	//Draw picture
	if (!alpha) {
		sprite->End();
		sprite->Begin(0);
	}
	h = sprite->Draw(textures[picture],rect,NULL,&D3DXVECTOR3(x,y,0),modulationcolor);
	if (!alpha) {
		sprite->End();
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	if (FAILED(h)) {
		string debugstring = "Couldn't draw picture: ";
		debugstring+=picturesource[picture];
		debugger.LogMessage("draw::DrawPicture",debugstring,logger::MSG_SUCCESS);
	}
	if (drawing==false)
		EndDraw();
	return h;
}
BOOL draw::Clear(void) {
	return device->Clear(NULL,NULL,D3DCLEAR_TARGET,clearcolor,NULL,NULL);
}
BOOL draw::Flip(void) {
	HRESULT h = device->Present(NULL,NULL,NULL,NULL);
	if (FAILED(h)) {
		string err = "Couldn't flip buffer";
		err += ErrorCodeRet(h);
		debugger.LogMessage("draw::Flip",err,logger::MSG_ERROR);
	}
	return h;
}

void draw::SetClearColor(int color) {
	clearcolor=color;
}
int draw::DrawColor(int x,int y,int x2,int y2,D3DCOLOR color) {
	D3DRECT rect = {x,y,x2,y2};
	return device->Clear(1,&rect,D3DCLEAR_TARGET,color,NULL,NULL);
}



void draw::SetBackground(COLORREF color) {
	background=color;
}

void draw::DrawLine(int x,int y,int x2,int y2, D3DCOLOR color) {
	if (drawing) {
		D3DXVECTOR2 vertices[2] = {D3DXVECTOR2(x,y),D3DXVECTOR2(x2,y2)};
		line->Draw(vertices,2,color);
	}
}

IDirect3DTexture9 *draw::ReturnTexture(int picture) {
	return textures[picture];
}
int draw::DrawText(char *text,int x,int y,D3DCOLOR color) {
	//Check if font is valid
	if (currentfont<0||currentfont>fonts.size()) {
		debugger.LogMessage("draw::DrawText","Font outside of range of fonts",logger::MSG_ERROR);
		return 0;
	}
	//Retrieve font from the list
	ID3DXFont *font = fonts[currentfont];
	//Create default rectangle
	RECT rect = {x,y,x,y};
	//Calculate rectangle to draw in, since only x and y are given.
	//sprite->Begin(D3DXSPRITE_ALPHABLEND);
	HRESULT h = font->DrawText(sprite,text,-1,&rect,DT_CALCRECT|DT_LEFT|DT_SINGLELINE,color);
	
	if (FAILED(h)) 
		debugger.LogMessage("draw::DrawText(char *text,int x,int y,D3DCOLOR)","Error calculating rectangle",logger::MSG_ERROR);
	//Call primary text drawing function to draw the text itself.
	h = DrawText(text,x,y,rect.right,rect.bottom,color,DT_LEFT|DT_SINGLELINE);
	//sprite->End();
	return h;
}

/**************************************
This is the primary text drawing function. It is the one which actually does the drawing, unlike the other overloaded ones.
As it is the primary one, it requires all parameters. The other overloaded ones simply provide default parameters, or
calculate these parameters. In all cases, the string is NULL-terminated.
***************************************/
int draw::DrawText(char *text,int x,int y,int x2,int y2,D3DCOLOR color,int format) {
	//Retrieve font from the list
	if (currentfont>0&&currentfont<fonts.size()) {
		debugger.LogMessage("draw::DrawText","Font outside of range of fonts",logger::MSG_ERROR);
		return 0;
	}
	ID3DXFont *font = fonts[currentfont];
	//Declare rectangle for drawing in
	RECT rect;
	rect.top	=y>y2?y2:y;//Set top to lowest 'y' value
	rect.bottom	=y<y2?y2:y;//Set bottom to highest 'y' value
	rect.left	=x>x2?x2:x;//Set left to lowest 'x' value
	rect.right	=x<x2?x2:x;//Set right to highest 'x' value
	//Draw text with the font
	HRESULT h = font->DrawText(sprite,text,-1,&rect,format,color);
	//Check the return value
	if (FAILED(h))
		debugger.LogMessage("draw::DrawText","Couldn't draw text",logger::MSG_ERROR);
	return h;
}
void draw::DrawNumber(int number,int x,int y,COLORREF color) {
	char buffer[256];
	for (int temp=0;temp<256;temp++)
		buffer[temp]=0;
	itoa(number,buffer,10);
	DrawText(buffer,x,y,color);
}

void draw::DrawPictureRepeat(int picture,int picwidth,int picheight,int x,int y, int endx,int endy) {
	RECT rect = {0,0,picwidth,picheight};
	for (int temp=x;temp<endx;temp+=picwidth)
		for (int temp2=y;temp2<endy;temp2+=picheight) {
			//int tempx=rect.right;
			//int tempy=rect.bottom;
			//if (temp+picwidth>endx) rect.right-=(temp+picwidth)-endx;
			//if (temp2+picheight>endy)rect.bottom-=(temp2+picheight)-endy;
			
			DrawPicture(temp,temp2,picture,&rect);
			//rect.right=tempx;
			//rect.bottom=tempy;
		}
}
//Sets whether to draw on front-buffer or back-buffer. Unused with Direct3D. Here for compatibility reasons only.
void draw::SetTarget(bool thetarget) {
	target=thetarget;
}

HDC draw::Lock(void) {
	device->GetBackBuffer(0,0,D3DBACKBUFFER_TYPE_MONO,&backbuffer);
	backbuffer->GetDC(&lockdc);
	return lockdc;
}

void draw::Unlock(void) {
	backbuffer->ReleaseDC(lockdc);
	backbuffer->Release();
}

int draw::StartDraw(void) {
	HRESULT h;
	h = device->BeginScene();
	sprite->Begin(D3DXSPRITE_ALPHABLEND);
	line->Begin();
	drawing=true;
	return h;
}

int draw::EndDraw(void) {
	line->End();
	sprite->End();
	HRESULT h = device->EndScene();
	
	drawing=false;
	return h;
}

int draw::AddFont(int height,int width,long weight,bool italic,const char fontname[32]) {
	//Define a font
	D3DXFONT_DESC font;
	font.Height=height; 
	font.Width=width; 
	font.Weight=weight; 
	font.Italic=(italic?1:0); 
	font.CharSet=ANSI_CHARSET; 
	font.OutputPrecision=OUT_TT_ONLY_PRECIS; 
	font.Quality=DEFAULT_QUALITY; 
	font.PitchAndFamily=DEFAULT_PITCH|FF_DONTCARE; 
	strcpy(font.FaceName,fontname);//Copy font name into logfont structure

	//Create font
	ID3DXFont *tempfont;
	HRESULT h = D3DXCreateFontIndirect(device,&font,&tempfont);

	//Add font to end of list if successful
	if (SUCCEEDED(h))
		fonts.push_back(tempfont);
	return h;
}

int draw::GetTextureWidth(int picture) {
	unsigned int width = textureinfo[picture].Width;//Retrieve picture information structure
	D3DXCheckTextureRequirements(device,&width,NULL,NULL,NULL,NULL,D3DPOOL_MANAGED);
	return width;
}

int draw::GetTextureHeight(int picture) {
	unsigned int height = textureinfo[picture].Height;//Retrieve picture information structure
	D3DXCheckTextureRequirements(device,&height,NULL,NULL,NULL,NULL,D3DPOOL_MANAGED);
	return height;
}

int draw::DrawPicture(int x, int y, int thewidth,int theheight,int picture, RECT *rect,bool alpha,double rotation) {
	if (drawing==false) {
		StartDraw();
		drawing=false;
	}
	HRESULT h;
	//Calculate scaling vector
	D3DXVECTOR2 result(((float)thewidth)/(rect->right-rect->left),((float)theheight)/(rect->bottom-rect->top));
	//Calculate matrix
	D3DXMATRIX matrix;
	float rot = degtorad(rotation);
	D3DXMatrixTransformation2D(&matrix,NULL,NULL,&result,NULL,rot,0);
	//Set transformation matrix into sprite
	sprite->SetTransform(&matrix);
	//Draw picture
	if (!alpha) {
		sprite->End();
		sprite->Begin(0);
	}
	h = sprite->Draw(textures[picture],rect,NULL,&D3DXVECTOR3(x,y,0),modulationcolor);
	if (!alpha) {
		sprite->End();
		sprite->Begin(D3DXSPRITE_ALPHABLEND);
	}
	if (FAILED(h)) {
		string debugstring = "Couldn't draw picture: ";
		debugstring+=picturesource[picture];
		debugger.LogMessage("draw::DrawPicture",debugstring,logger::MSG_SUCCESS);
	}
	if (drawing==false)
		EndDraw();
	return h;
}