#include "inexinferis.h"

#define iMenuSize 19
GLboolean bDrawMenu=0;
unsigned int iSelected=0;

struct Menu
{
    const char *cVarName;
    int*iValue,iMin,iMax;
}Menues[iMenuSize]=
{
    {"WallHack",&cfg.bWall,0,2},
    {"NoSmoke",&cfg.bNoSmoke,0,1},
    {"NoFlash",&cfg.bNoFlash,0,1},
    {"NoSky",&cfg.bNoSky,0,1},
    {"Crosshair",&cfg.bCrosshair,0,6},
    {"Lambert",&cfg.bLambert,0,1},
    {"ColorLambert",&cfg.bColorLambert,0,1},
    {"WhiteWalls",&cfg.bWhiteWalls,0,1},
    {"Fullbright",&cfg.bFullbright,0,1},
    {"Nightmode",&cfg.bNightmode,0,1},
    {"Niggermodels",&cfg.bNiggermodels,0,1},
    {"Wireframe",&cfg.bWireframe,0,1},
    {"Wiremodels",&cfg.bWiremodels,0,1},
    {"EspBox",&cfg.bEspBox,0,1},
    {"AimTeam",&cfg.bAimTeam,0,3},
    {"BunnyHop",&cfg.bBunnyHop,0,1},
    {"NoRecoil",&cfg.iNoRecoil,0,5},
    {"Speed",&cfg.iSpeed,-9,9},
    {"Zoom",&cfg.bZoom,0,1}
};

void DrawTitleBox(GLfloat x,GLfloat y,GLfloat w,GLfloat h,const char* title)
{
    FillRGBA(x,y,w,15,Colors[0].r,Colors[0].g,Colors[0].b,Colors[0].a);
    DrawRectangle(x+1,y+1,w-1,h,0,0,0,255);
    DrawRectangle(x+4,y+5,8,8,0,0,0,255);
    DrawString(0,x+18,y+12,Colors[6].r,Colors[6].g,Colors[6].b,Colors[6].a,title);
}

void DrawContentBox(GLfloat x,GLfloat y,GLfloat w,GLfloat h,GLboolean bg)
{
    if(bg)
        FillRGBA(x,y,w,h,Colors[1].r,Colors[1].g,Colors[1].b,Colors[1].a);
    DrawRectangle(x+1,y+1,w-1,h,0,0,0,255);
}

void DrawWindow(GLfloat x,GLfloat y,GLfloat w,GLfloat h,GLboolean bg,const char* title)
{
    DrawTitleBox(x-2,y-17,w,15,title);
    DrawContentBox(x-2,y-2,w,h,bg);
}

void DrawMenu(int x,int y)
{
    unsigned int i=0,w=140,h=(iMenuSize+1)*14;
	DrawWindow(x,y,w,h,1,"Inexinferis FX Series");
	for(;i<iMenuSize;i++)
	{
        pColorEntry clr=&Colors[(i!=iSelected)?5:6];
        DrawString(0,x+5,y+(i+1)*14,clr->r,clr->g,clr->b,clr->a,Menues[i].cVarName);
        DrawString(0,x+w-15,y+(i+1)*14,clr->r,clr->g,clr->b,clr->a,"%d",*Menues[i].iValue);
	}
}

//handle drawing functions
void GuiHandleDraw()
{
    if(cfg.bCrosshair)
        DrawCross(cfg.bCrosshair);
    if(bDrawMenu)
        DrawMenu(100,100);
}

//handle input functions
typeof(SDL_PollEvent)*pSDL_PollEvent;
DECLSPEC int SDLCALL SDL_PollEvent(SDL_Event*event)
{
    if(!pSDL_PollEvent)pSDL_PollEvent=(typeof(SDL_PollEvent)*)dlsym(RTLD_NEXT,"SDL_PollEvent");

    int returnvalue=pSDL_PollEvent(event);
    if(event->type==SDL_KEYDOWN)
    {
        switch(event->key.keysym.sym)
        {
            case SDLK_F3:
                cfg.bDisabled=!cfg.bDisabled;
                break;
            case SDLK_F4:
                if(!cfg.bDisabled)
                {
                    bDrawMenu=!bDrawMenu;
                    SaveConfig();
                }
                break;
            case SDLK_UP:
                if(!cfg.bDisabled&&bDrawMenu)
                {
                    if(iSelected>0)
                        iSelected--;
                    else
                        iSelected=iMenuSize-1;
                }
                break;
            case SDLK_DOWN:
                if(!cfg.bDisabled&&bDrawMenu)
                {
                    if(iSelected<iMenuSize-1)
                        iSelected++;
                    else
                        iSelected=0;
                }
                break;
            case SDLK_LEFT:
                if(!cfg.bDisabled&&bDrawMenu)
                {
                    if(Menues[iSelected].iValue)
                    {
                        --(*Menues[iSelected].iValue);
                        if(*Menues[iSelected].iValue<Menues[iSelected].iMin)
                            *Menues[iSelected].iValue=Menues[iSelected].iMax;
                    }
                }
                break;
            case SDLK_RIGHT:
                if(!cfg.bDisabled&&bDrawMenu)
                {
                    if(Menues[iSelected].iValue)
                    {
                        ++(*Menues[iSelected].iValue);
                        if(*Menues[iSelected].iValue>Menues[iSelected].iMax)
                            *Menues[iSelected].iValue=Menues[iSelected].iMin;
                    }
                }
                break;
            case SDLK_SPACE:
                if(!cfg.bDisabled&&cfg.bBunnyHop)
                {
                    //keybd_event(VK_SPACE,MapVirtualKey(VK_SPACE,0),KEYEVENTF_KEYUP,0);
                    Uint8*state=(Uint8*)SDL_GetKeyboardState(0);
                    state[SDL_SCANCODE_SPACE]=1;
                    SDL_PumpEvents();
                }
                break;
        }
    }
    else if(event->type==SDL_MOUSEWHEEL)
    {
        switch(event->wheel.direction)
        {
            case SDL_MOUSEWHEEL_NORMAL:
                break;
            case SDL_MOUSEWHEEL_FLIPPED:
                break;
        }
    }
    else if(event->type==SDL_MOUSEBUTTONDOWN)
    {
        if(event->button.button==SDL_BUTTON_LEFT)
        {
            if(!cfg.bDisabled)
            {
                if(bDrawMenu)
                {
                    if(Menues[iSelected].iValue)
                    {
                        ++(*Menues[iSelected].iValue);
                        if(*Menues[iSelected].iValue>Menues[iSelected].iMax)
                            *Menues[iSelected].iValue=Menues[iSelected].iMin;
                    }
                    return 1;
                }
                else
                {
                    bKeyAimbot=1;
                    if(bHasTarget&&!bShoot)
                        return 1;
                }
            }
        }
        else if(event->button.button==SDL_BUTTON_RIGHT)
        {
            if(!cfg.bDisabled&&bDrawMenu)
            {
                if(Menues[iSelected].iValue)
                {
                    --(*Menues[iSelected].iValue);
                    if(*Menues[iSelected].iValue<Menues[iSelected].iMin)
                        *Menues[iSelected].iValue=Menues[iSelected].iMax;
                }
                return 1;
            }
        }
    }
    else if(event->type==SDL_MOUSEBUTTONUP && event->button.button==SDL_BUTTON_LEFT)
        bKeyAimbot=0;
    return returnvalue;
}
