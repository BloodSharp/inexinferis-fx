#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include "inexinferis.h"

typedef struct _ColorEntry{
	int r, g, b, a;
}ColorEntry,*pColorEntry;

//void DbgPrint(LPSTR form, ...);

void FillRGBA(GLfloat x,GLfloat y,GLfloat w,GLfloat h,int r,int g,int b,int a);

void DrawCross(int type);
void DrawRectangle(GLfloat x, GLfloat y, GLfloat w, GLfloat h, int r, int g, int b, int a);
void DrawFilledRectangle(GLfloat x,GLfloat y,GLfloat w, GLfloat h,int br, int bg, int bb, int ba,int cr, int cg, int cb, int ca);
void DrawString(GLboolean bCenter,GLfloat x,GLfloat y, int r, int g, int b,int a, const char *fmt, ...);

void DrawBox(GLfloat x, GLfloat y,GLfloat dist);

extern ColorEntry Colors[];

#endif //GRAPHICS_H_INCLUDED
