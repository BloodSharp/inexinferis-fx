#ifndef WALLHACK_H_INCLUDED
#define WALLHACK_H_INCLUDED

#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<GL/glu.h>
#include<cstdio>
#include<cmath>
#include<dlfcn.h>

typedef struct _cheat_cfg
{
    int bDisabled;
    int bWall;
    int bWhiteWalls;
    int bNoSmoke;
    int bNoFlash;
    int bNoSky;
    int bLambert;
    int bColorLambert;
    int bFullbright;
    int bNightmode;
    int bNiggermodels;
    int bWireframe;
    int bWiremodels;
    int bCrosshair;
    int bAimTeam;
    int bEspBox;
    int bBunnyHop;
    int bZoom;
    int iZoom;
    int iNoRecoil;
    int iSpeed;
}cheat_cfg,*pcheat_cfg;

typedef struct
{
    GLboolean get;
    GLfloat highest_x;
    GLfloat highest_y;
    GLfloat highest_z;
    GLfloat lowest_x;
    GLfloat lowest_y;
    GLfloat lowest_z;
    GLfloat vertex;
}model_s;

typedef struct
{
    GLint team;
    GLfloat vTexOneX;
    GLfloat vTexOneY;
    GLfloat vTexOneZ;
    GLfloat vTexTwoX;
    GLfloat vTexTwoY;
    GLfloat vTexTwoZ;
    GLfloat vTexDist;
    GLfloat fr,fg,fb;
    GLfloat vertex;
}player_s;

extern cheat_cfg cfg;
extern player_s player;
extern model_s model;

extern GLboolean bKeyAimbot;
extern GLboolean bHasTarget;
extern GLboolean bShoot;

/*#ifdef __cplusplus
extern "C"
{
#endif*/
extern typeof(glBegin)*pglBegin;
extern typeof(glClear)*pglClear;
extern typeof(glVertex2f)*pglVertex2f;
extern typeof(glVertex3fv)*pglVertex3fv;
extern typeof(glVertex3f)*pglVertex3f;
extern typeof(glShadeModel)*pglShadeModel;
extern typeof(glPushMatrix)*pglPushMatrix;
extern typeof(glPopMatrix)*pglPopMatrix;
extern typeof(glEnable)*pglEnable;
extern typeof(glDisable)*pglDisable;
extern typeof(glViewport)*pglViewport;
extern typeof(glFrustum)*pglFrustum;

extern typeof(SDL_PollEvent)*pSDL_PollEvent;

extern void SaveConfig();
/*#ifdef __cplusplus
}
#endif*/

#include"graphics.h"
#include"gui.h"

#endif//WALLHACK_H_INCLUDED
