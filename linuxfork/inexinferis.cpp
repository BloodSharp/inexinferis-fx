#include "inexinferis.h"

//WNDPROC pHLWndProc=NULL;HANDLE hThread=NULL;HWND hdHalfLife;

#ifdef __cplusplus
#define HOOKED_EXPORT extern "C" __attribute__ ((visibility ("default")))
#else
#define HOOKED_EXPORT __attribute__ ((visibility ("default")))
#endif // __cplusplus

__typeof__(SDL_GL_GetProcAddress)*pOrig_SDL_GL_GetProcAddress=0;

SDL_Point pt;model_s model;player_s player;
char config[]="./inexinferis/inexinferis.ini";
GLint iView[4];GLdouble dModel[16],dProy[16];unsigned int coil=0;
GLboolean bOnSpeed=0,bKeyAimbot=0,bShoot=0,bHasTarget=0,bSmoke=0,bSky=0,bFlash=0,
bTex=0,bDrawn=0,modelviewport=0,bLeftButtonDown=0;
cheat_cfg cfg={0,0,0, 0,0,0, 0,0,0, 0,0,0,
0,0,0, 0,0,0, -5,0,0};

/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
//pQueryPerformanceCounter;

void LoadConfig()
{
    FILE*fi=fopen(config,"r");
    if(fi)
    {
        fread(&cfg,sizeof(cfg),1,fi);
        fclose(fi);
    }
}

void SaveConfig()
{
    FILE *fi=fopen(config,"w");
    if(fi)
    {
        fwrite(&cfg,sizeof(cfg),1,fi);
        fclose(fi);
    }
}


GLfloat GetDist(GLfloat firstX,GLfloat firstY,GLfloat firstZ,GLfloat secX,GLfloat secY,GLfloat secZ)
{
    return (GLfloat)sqrt((((secX-firstX)*(secX-firstX))+((secY-firstY)*(secY-firstY))+((secZ-firstZ)*(secZ-firstZ))));
}

typeof(glBegin)*pglBegin=0;
void GLAPIENTRY glBegin(GLenum mode)
{
    if(!pglBegin)pglBegin=(typeof(glBegin)*)dlsym(RTLD_NEXT,"glBegin");

    float col[4];
    if(!cfg.bDisabled)
    {
        switch(cfg.bWall)
        {
            case 1:
                if(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN)
                    pglDisable(GL_DEPTH_TEST);
                else if(mode!=GL_QUADS&&mode!=GL_LINES)
                    pglEnable(GL_DEPTH_TEST);
                break;
            case 2:
                if(!(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
                {
                    glGetFloatv(GL_CURRENT_COLOR, col);
                    pglDisable(GL_DEPTH_TEST);
                    pglEnable(GL_BLEND);
                    glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA_SATURATE);
                    glColor4f(col[0], col[1], col[2],.667f);
                }
                else if(mode==GL_TRIANGLES || mode==GL_TRIANGLE_STRIP || mode==GL_TRIANGLE_FAN)
                {
                    pglEnable(GL_DEPTH_TEST);
                    pglDisable(GL_BLEND);
                }
                break;
            default:
                break;
        }
        if(!(mode==GL_TRIANGLES||mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN||mode==GL_QUADS))
        {
            if(cfg.bNightmode)
                glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);
            if(cfg.bWhiteWalls)
                glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
            if(cfg.bFullbright&&bTex)
                pglDisable(GL_TEXTURE_2D);
        }
        else if(cfg.bNiggermodels)
            glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_BLEND);

        if(mode==GL_QUADS)
        {
            if(cfg.bNoFlash||cfg.bNoSmoke)
            {
                glGetFloatv(GL_CURRENT_COLOR, col);
                bSmoke=(col[0]==col[1]&&col[0]==col[2]&&col[0]!=0.0&&col[0]!=1.0);
                bFlash=(col[0]==1.0&&col[1]==1.0&&col[2]==1.0&&col[3]>0.2);
            }
            bSky=1;
        }
        else
            bSky=0;

        if(cfg.bWireframe)
        {
            if(mode==GL_POLYGON)
            {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glLineWidth(1.0f);
                glColor3f(1.0f,1.0f,1.0f);
            }
            else
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }
        if(cfg.bWiremodels)
        {
            if(mode==GL_TRIANGLE_STRIP||mode==GL_TRIANGLE_FAN)
            {
                glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                glLineWidth(1.0f);
            }
            else if(!cfg.bWireframe)
                glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }
    }
    pglBegin(mode);
}

typeof(glClear)*pglClear=0;
void GLAPIENTRY glClear(GLbitfield mask)
{
    if(!pglClear)pglClear=(typeof(glClear)*)dlsym(RTLD_NEXT,"glClear");

    if(!cfg.bDisabled&&mask==GL_DEPTH_BUFFER_BIT)
    {
        mask+=GL_COLOR_BUFFER_BIT;
        glClearColor(0.0f,0.0f,0.0f,0.0f);
    }
    pglClear(mask);
}

typeof(glVertex2f)*pglVertex2f=0;
void GLAPIENTRY glVertex2f(GLfloat x,GLfloat y)
{
    if(!pglVertex2f)pglVertex2f=(typeof(glVertex2f)*)dlsym(RTLD_NEXT,"glVertex2f");

    if(!cfg.bDisabled&&cfg.bNoFlash&&bFlash&&x==0.0&&y==0.0)
        glColor4f(1.0f,1.0f,1.0f,0.2f);
    pglVertex2f(x,y);
}

typeof(glVertex3fv)*pglVertex3fv=0;
void GLAPIENTRY glVertex3fv(const GLfloat *v)
{
    if(!pglVertex3fv)pglVertex3fv=(typeof(glVertex3fv)*)dlsym(RTLD_NEXT,"glVertex3fv");

    if(!cfg.bDisabled)
    {
        if(cfg.bNoSmoke&&bSmoke)
            return;
        if((cfg.bNoSky||cfg.bWall==2)&&bSky)
            return;
        modelviewport=1;
    }
    pglVertex3fv(v);
}

typeof(glVertex3f)*pglVertex3f=0;
void GLAPIENTRY glVertex3f(GLfloat x,GLfloat y,GLfloat z)
{
    if(!pglVertex3f)pglVertex3f=(typeof(glVertex3f)*)dlsym(RTLD_NEXT,"glVertex3f");

    if(!cfg.bDisabled)
    {
        if(player.vertex==8)
        {
            player.vTexOneX=x;
            player.vTexOneY=y;
            player.vTexOneZ=z;
        }
        if(player.vertex==9)
        {
            player.vTexTwoX=x;
            player.vTexTwoY=y;
            player.vTexTwoZ=z;
        }
        if(player.vertex==10)
        {
            player.vTexDist=GetDist(player.vTexOneX,player.vTexOneY,player.vTexOneZ,player.vTexTwoX,player.vTexTwoY,player.vTexTwoZ);
            //CT'S
            if(
                ((player.vTexDist>3.63f)&&(player.vTexDist<3.65f))||
                ((player.vTexDist>10.6f)&&(player.vTexDist<10.8f))||
                ((player.vTexDist>0.34f)&&(player.vTexDist<0.36f))||
                ((player.vTexDist>2.85f)&&(player.vTexDist<2.87f))||
                ((player.vTexDist>3.31f)&&(player.vTexDist<3.33f)))
                player.team=1;
            //T'S
            else if(
                ((player.vTexDist>2.31f)&&(player.vTexDist<2.33f))||
                ((player.vTexDist>3.69f)&&(player.vTexDist<3.71f))||
                ((player.vTexDist>2.22f)&&(player.vTexDist<2.24f))||
                ((player.vTexDist>2.08f)&&(player.vTexDist<2.10f)))
                player.team=2;
            //colors
            if(player.team==1)
            {
                player.fr=Colors[3].r/255;
                player.fg=Colors[3].g/255;
                player.fb=Colors[3].b/255;
            }
            else if(player.team==2)
            {
                player.fr=Colors[2].r/255;
                player.fg=Colors[2].g/255;
                player.fb=Colors[2].b/255;
            }
            else
            {
                player.fr=Colors[4].r/255;
                player.fg=Colors[4].g/255;
                player.fb=Colors[4].b/255;
            }
        }
        if(model.get)
        {
            model.vertex++;
            if((z>model.highest_z)||(model.highest_z==-99999))
            {
                model.highest_x=x;
                model.highest_y=y;
                model.highest_z=z;
            }
            if((z<model.lowest_z)||(model.lowest_z==-99999))
            {
                model.lowest_x=x;
                model.lowest_y=y;
                model.lowest_z=z;
            }
        }
        player.vertex++;
        if(cfg.bLambert)
            glColor3f(1.0f,1.0f,1.0f);
        if(cfg.bColorLambert&&player.team)
            glColor3f(player.fr,player.fg,player.fb);
    }
    pglVertex3f(x,y,z);
}

typeof(glShadeModel)*pglShadeModel=0;
void GLAPIENTRY glShadeModel(GLenum mode)
{
    if(!pglShadeModel)pglShadeModel=(typeof(glShadeModel)*)dlsym(RTLD_NEXT,"glShadeModel");

    GLdouble wx,wz,wy;
    if(!cfg.bDisabled)
    {
        if((mode==GL_SMOOTH)&&(!model.get))
        {
            model.get=1;
            model.highest_z=-99999;
            model.lowest_z=-99999;
            model.vertex=0;
        }
        else
        {
            if(model.get&&!player.fg)
            {
                if(model.vertex>550&&(model.highest_z-model.lowest_z)>30.0f)
                {
                    glGetDoublev(GL_MODELVIEW_MATRIX,dModel);
                    glGetDoublev(GL_PROJECTION_MATRIX,dProy);
                    glGetIntegerv(GL_VIEWPORT,iView);
                    gluProject(model.highest_x,model.highest_y,model.highest_z,dModel,dProy,iView,&wx,&wy,&wz);
                    if(cfg.bEspBox)
                        DrawBox((wx/(iView[2]/2))-1.0f,(wy/(iView[3]/2))-1.0f,wz-1.0f);
                    if(cfg.bAimTeam)
                    {
                        bHasTarget=((abs((int)(iView[2]/2-wx))<(iView[2]/10))&&(abs((int)(iView[3]/2-wy))<(iView[3]/10)));
                        if(bKeyAimbot&&bHasTarget&&(cfg.bAimTeam==3||cfg.bAimTeam==player.team))
                        {
                            pt.x=(long)wx;pt.y=(long)(iView[3]-(wy-.01/(wz-1)));//(long)(iView[3]-wy);
                            /*ClientToScreen(hdHalfLife,&pt);
                            SetCursorPos(pt.x,pt.y);*/
                            bShoot=((iView[2]/2)==wx&&(iView[3]/2)==wy);
                        }
                        if(!bHasTarget)
                            bShoot=0;
                    }
                }
            }
        }
    }
    pglShadeModel(mode);
}

typeof(glPushMatrix)*pglPushMatrix=0;
void GLAPIENTRY glPushMatrix()
{
    if(!pglPushMatrix)pglPushMatrix=(typeof(glPushMatrix)*)dlsym(RTLD_NEXT,"glPushMatrix");

    if(!cfg.bDisabled)
    {
        player.vertex=0;
        player.team=0;
    }
    pglPushMatrix();
}

typeof(glPopMatrix)*pglPopMatrix=0;
void GLAPIENTRY glPopMatrix()
{
    if(!pglPopMatrix)pglPopMatrix=(typeof(glPopMatrix)*)dlsym(RTLD_NEXT,"glPopMatrix");

    if(!cfg.bDisabled&&model.get)
        model.get=0;
    pglPopMatrix();
}

typeof(glEnable)*pglEnable=0;
void GLAPIENTRY glEnable(GLenum mode)
{
    if(!pglEnable)pglEnable=(typeof(glEnable)*)dlsym(RTLD_NEXT,"glEnable");

    if(!cfg.bDisabled)
    {
        if(mode==GL_TEXTURE_2D)
            bTex=1;
        //GUI
        if(bDrawn)
        {
            GuiHandleDraw();
            bDrawn=0;
        }
    }
    pglEnable(mode);
}

typeof(glDisable)*pglDisable=0;
void GLAPIENTRY glDisable(GLenum mode)
{
    if(!pglDisable)pglDisable=(typeof(glDisable)*)dlsym(RTLD_NEXT,"glDisable");

    if(!cfg.bDisabled&&mode==GL_TEXTURE_2D)
        bTex=0;
    pglDisable(mode);
}

typeof(glViewport)*pglViewport=0;
void GLAPIENTRY glViewport(GLint x,GLint y,GLsizei width,GLsizei height)
{
    if(!pglViewport)pglViewport=(typeof(glViewport)*)dlsym(RTLD_NEXT,"glViewport"),LoadConfig();

    bDrawn=1;//GUI
    if(!cfg.bDisabled)
    {
        if(modelviewport)
        {
            model.highest_z=-99999;
            model.lowest_z=-99999;
            modelviewport=0;
        }
        if(cfg.iNoRecoil)
        {
            coil++;
            if(coil>=6)
            {
                coil=0;
                if(bLeftButtonDown)
                {
                    //GetCursorPos(&pt);
                    SDL_GetMouseState(&pt.x,&pt.y);
                    //SetCursorPos(pt.x,pt.y+cfg.iNoRecoil);
                }
            }
        }
    }
    pglViewport(x,y,width,height);
}

typeof(glFrustum)*pglFrustum=0;
void GLAPIENTRY glFrustum(GLdouble left,GLdouble right,GLdouble bottom,GLdouble top,GLdouble zNear,GLdouble zFar)
{
    if(!pglFrustum)pglFrustum=(typeof(glFrustum)*)dlsym(RTLD_NEXT,"glFrustum");

    if(!cfg.bDisabled&&cfg.bZoom)
    {
        top=zNear*tan((50-cfg.iZoom*5)*M_PI/360);
        bottom=-top;
        left=bottom*4/3;
        right=top*4/3;
    }
    pglFrustum(left,right,bottom,top,zNear,zFar);
}

HOOKED_EXPORT void*SDL_GL_GetProcAddress(const char*proc)
{
    while(!pOrig_SDL_GL_GetProcAddress)
    {
        CH4::Utils::DbgPrint("[B#] SDL_GL_GetProcAddress!");
        pOrig_SDL_GL_GetProcAddress=(__typeof__(SDL_GL_GetProcAddress)*)dlsym(dlopen(szLibSDL2,RTLD_NOW),"SDL_GL_GetProcAddress");
    }

    // Hooked GL!
    if(!strcmp(proc,"glBegin"))
    {
        pglBegin=(__typeof__(pglBegin))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglBegin;
    }
    else if(!strcmp(proc,"glClear"))
    {
        pglClear=(__typeof__(pglClear))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglClear;
    }
    else if(!strcmp(proc,"glVertex2f"))
    {
        pglVertex2f=(__typeof__(pglVertex2f))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglVertex2f;
    }
    else if(!strcmp(proc,"glVertex3fv"))
    {
        pglVertex3fv=(__typeof__(pglVertex3fv))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglVertex3fv;
    }
    else if(!strcmp(proc,"glVertex3f"))
    {
        pglVertex3f=(__typeof__(pglVertex3f))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglVertex3f;
    }
    else if(!strcmp(proc,"glShadeModel"))
    {
        pglShadeModel=(__typeof__(pglShadeModel))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglShadeModel;
    }
    else if(!strcmp(proc,"glPushMatrix"))
    {
        pglPushMatrix=(__typeof__(pglPushMatrix))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglPushMatrix;
    }
    else if(!strcmp(proc,"glPopMatrix"))
    {
        pglPopMatrix=(__typeof__(pglPopMatrix))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglPopMatrix;
    }
    else if(!strcmp(proc,"glEnable"))
    {
        pglEnable=(__typeof__(pglEnable))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglEnable;
    }
    else if(!strcmp(proc,"glDisable"))
    {
        pglDisable=(__typeof__(pglDisable))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglDisable;
    }
    else if(!strcmp(proc,"glViewport"))
    {
        pglViewport=(__typeof__(pglViewport))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglViewport;
    }
    else if(!strcmp(proc,"glFrustum"))
    {
        pglFrustum=(__typeof__(pglFrustum))pOrig_SDL_GL_GetProcAddress(proc);
        return(void*)&hglFrustum;
    }
    return pOrig_SDL_GL_GetProcAddress(proc);
}

/*#ifdef __cplusplus
}
#endif*/


/*GLboolean WINAPI hQueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount)
{
    LONGLONG newvalue;GLboolean ret;
    static LONGLONG oldfakevalue=0;
    static LONGLONG oldrealvalue=0;
    double factor=1.0;
    if(oldfakevalue==0||oldrealvalue==0)
    {
        oldfakevalue=lpPerformanceCount->QuadPart;
        oldrealvalue=lpPerformanceCount->QuadPart;
    }
    ret=pQueryPerformanceCounter(lpPerformanceCount);
    if(bKeyAimbot&&cfg.iSpeed)
    {
        if(cfg.iSpeed<0)
        {
            if(cfg.iSpeed<-9)
                cfg.iSpeed=-9;
            factor=1.0+(float)cfg.iSpeed/10;
        }
        else
            factor=cfg.iSpeed*3;
    }
    newvalue=lpPerformanceCount->QuadPart;
    newvalue=oldfakevalue+(LONGLONG)((newvalue-oldrealvalue)*factor);
    oldrealvalue=lpPerformanceCount->QuadPart;
    oldfakevalue=newvalue;
    lpPerformanceCount->QuadPart=newvalue;
    return ret;
}*/

