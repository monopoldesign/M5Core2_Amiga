#ifndef __GUI_H
#define __GUI_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Functions
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "gui_base.h"
#include "gui_button.h"
#include "../frame/frame_base.h"

/******************************************************************************
* Definitions
*******************************************************************************/
typedef struct
{
	Frame_Base *frame;
	gui_args_vector_t args;
} frame_struct_t;

/******************************************************************************
* Prototypes
*******************************************************************************/
void GUI_AddObject(GUI_Base *object);
void GUI_AddObject_Move(GUI_Base *object);
void GUI_Draw();
void GUI_Process();
void GUI_Process_Move();
void GUI_Clear(void);
void GUI_Run(Frame_Base *frame);
void GUI_MainLoop(void);
void GUI_AddFrame(String name, Frame_Base *frame);
Frame_Base *GUI_GetFrame(String name);
void GUI_PushFrame(Frame_Base *frame);
void GUI_PopFrame(bool isDelete = false);
void GUI_UpdateGlobalLastActiveTime();

#endif
