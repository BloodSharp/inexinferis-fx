#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

void DrawWindow(GLfloat x, GLfloat y, GLfloat w, GLfloat h, GLboolean bg,const char* title);
void DrawMenu(int x,int y);

void GuiHandleDraw();
//GLboolean GuiHandleMsg(unsigned int uMsg,WPARAM wParam, LPARAM lParam);

#endif //GUI_H_INCLUDED
