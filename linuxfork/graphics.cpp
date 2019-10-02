#include "graphics.h"

ColorEntry Colors[]=
{
	{250,100,0,150},	  // Menu Title - 0
	{128,128,128,180},  // Menu Content - 1
	{255,0,0,255},		  // Terrorist - 2
	{0,0,255,255},  		// Counter-Terrorist - 3
	{0,255,0,255},		  // Others - 4
	{48,48,48,255},  	  // Menu - Deselected - 5
	{255,255,255,255},	// Menu - Selected - 6
	{255,255,255,200},  // Crosshair - 7
	{255,0,0,200}       // Crosshair - 8
};

void FillRGBA(GLfloat x,GLfloat y,GLfloat w,GLfloat h,int r,int g,int b,int a)
{
    glPushMatrix();
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glColor4f((float)r/255.0f,(float)g/255.0f,(float)b/255.0f,(float)a/255.0f);
    glBegin(GL_QUADS);
    glVertex2f(x,y);
    glVertex2f(x+w,y);
    glVertex2f(x+w,y+h);
    glVertex2f(x,y+h);
    glEnd();
    glDisable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);

    glPopMatrix();
}

void DrawRectangle(GLfloat x,GLfloat y,GLfloat w,GLfloat h,int r,int g,int b,int a)
{
    FillRGBA(x-1,y-1,w+2,1,r,g,b,a);//top
    FillRGBA(x-1,y,1,h-1,r,g,b,a);//left
    FillRGBA(x-1,y+h-1,w+2,1,r,g,b,a);//bottom
    FillRGBA(x+w,y,1,h-1,r,g,b,a);//right
}

void DrawFilledRectangle(GLfloat x,GLfloat y,GLfloat w,GLfloat h,int br,int bg,int bb,int ba,int cr,int cg,int cb,int ca)
{
    FillRGBA(x,y,w,h,cr,cg,cb,ca);
    FillRGBA(x-1,y-1,w+2,1,br,bg,bb,ba);//top
    FillRGBA(x-1,y,1,h-1,br,bg,bb,ba);//left
    FillRGBA(x-1,y+h-1,w+2,1,br,bg,bb,ba);//bottom
    FillRGBA(x+w,y,1,h-1,br,bg,bb,ba);//right
}

void DrawCross(int type)
{
    GLint iDim[4];
    glGetIntegerv(GL_VIEWPORT,iDim);
    int xcross=iDim[2]/2;
    int ycross=iDim[3]/2;

    int r=Colors[7].r,g=Colors[7].g,b=Colors[7].b,a=Colors[7].a;
    int R=Colors[8].r,G=Colors[8].g,B=Colors[8].b,A=Colors[8].a;

    switch(type)
    {
        case 1:
            FillRGBA(xcross-14,ycross,9,1,r,g,b,a);
            FillRGBA(xcross+5,ycross,9,1,r,g,b,a);
            FillRGBA(xcross,ycross-14,1,9,r,g,b,a);
            FillRGBA(xcross,ycross+5,1,9,r,g,b,a);
            FillRGBA(xcross,ycross,1,1,R,G,B,A);//center
            break;
        case 2:
            FillRGBA(xcross-25,ycross,50,1,r,g,b,a);
            FillRGBA(xcross,ycross-25,1,50,r,g,b,a);
            FillRGBA(xcross - 5,ycross,10,1,R,G,B,A);
            FillRGBA(xcross,ycross - 5, 1,10,R,G,B,A);
            break;
        case 3:
            FillRGBA(xcross-25,ycross,50,2,r,g,b,a);
            FillRGBA(xcross,ycross-25,2,50,r,g,b,a);
            FillRGBA(xcross - 5,ycross,10,2,R,G,B,A);
            FillRGBA(xcross,ycross - 5, 2,10,R,G,B,A);
            break;
        case 4:
            FillRGBA(xcross-25,ycross-25,50,1,255,255,255,180);
            FillRGBA(xcross-25,ycross+25,50,1,255,255,255,180);
            FillRGBA(xcross-25,ycross-25+1,1,50-1,255,255,255,180);
            FillRGBA(xcross+25,ycross-25,1,50+1,255,255,255,180);
            FillRGBA(xcross,ycross-25+1,1,50-1,0,160,0,180);
            FillRGBA(xcross-25+1,ycross,50-1,1,0,160,0,180);
            break;
        case 5:
            FillRGBA(0,ycross,2*xcross,1,r,g,b,a);
            FillRGBA(xcross,0,1,2*ycross,r,g,b,a);
            FillRGBA(xcross - 5,ycross,10,1,R,G,B,A);
            FillRGBA(xcross,ycross - 5, 1,10,R,G,B,A);
            break;
        default:
            FillRGBA(xcross-1,ycross,3,1,R,G,B,A);
            FillRGBA(xcross,ycross-1,1,3,R,G,B,A);
            break;
    }
}

void DrawString(GLboolean bCenter,GLfloat x,GLfloat y,int r,int g,int b,int a,const char *fmt,...)
{
    /*static unsigned int g_FontListID=0,i;
  static short g_FontHeight,g_FontWidth[255];
  if(!g_FontListID){
    g_FontListID=pglGenLists(256);
    HFONT hFont=CreateFont(13,0,0,0,FW_BOLD,0,0,0,ANSI_charSET,
    OUT_TT_ONLY_PRECIS,CLIP_DEFAULT_PRECIS,PROOF_QUALITY,DEFAULT_PITCH|FF_DONTCARE,"TAHOMA");
    HDC hDC=wglGetCurrentDC();
    HFONT hOldFont=(HFONT)SelectObject(hDC,hFont);
    while(!wglUseFontBitmaps(hDC,0,255,g_FontListID))
      Sleep(10);
    for(i=0;i<255;i++){
      SIZE Size={0};
      char szLine[2]={(char)i,0};
      GetTextExtentPoint(hDC,szLine,1,&Size);
      g_FontWidth[i]=(short)Size.cx;
      g_FontHeight=(short)Size.cy;
    }
    SelectObject(hDC,hOldFont);
    DeleteObject(hFont);
  }

  va_list va_alist;char buf[1024];
  va_start(va_alist,fmt);
  vsnprintf(buf,sizeof(buf),fmt,va_alist);
  va_end(va_alist);

  if(bCenter){
    char *p;int iLength=0,iHeight=g_FontHeight;
    for(p=buf;*p;p++)
			iLength+=g_FontWidth[(int)*p];
    x-=iLength/2;y-=iHeight/2;
  }

  pglDisable(GL_TEXTURE_2D);
	pglColor4f((float)r/255,(float)g/255,(float)b/255,(float)a/255);
	pglRasterPos2f(x,y);
	pglListBase(g_FontListID);
	pglCallLists(strlen(buf),GL_UNSIGNED_BYTE,buf);
	pglEnable(GL_TEXTURE_2D);*/
}

void DrawBox(GLfloat x,GLfloat y,GLfloat dist)
{
    GLint mMode;
    glGetIntegerv(GL_MATRIX_MODE,&mMode);
    glEnable(GL_POINT_SMOOTH);
    glDisable(GL_TEXTURE_2D);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);

    glLineWidth(4.0f);
    glColor4f(player.fr,player.fg,player.fb,0.6f);
    glBegin(GL_CURRENT_BIT|GL_POINT_BIT);
    glVertex2f(x-dist*2.5,y+dist*3);
    glVertex2f(x-dist*2.5,y+dist*10);
    glVertex2f(x+dist*2.5,y+dist*10);
    glVertex2f(x+dist*2.5,y+dist*3);
    glVertex2f(x-dist*2.5,y+dist*3);
    glEnd();

    glDisable(GL_BLEND);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glMatrixMode(mMode);

    glEnable(GL_TEXTURE_2D);
    glDisable(GL_POINT_SMOOTH);
}

