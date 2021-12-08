/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Functions
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <Arduino.h>
#include <stack>
#include <map>
#include <list>

#include "gui.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
std::list<GUI_Base *> gui_object_list;
std::list<GUI_Base *> gui_objectM_list;
uint32_t obj_id = 1;
uint32_t objM_id = 1;
Frame_Base* wait_for_delete = NULL;
std::stack<Frame_Base *> frame_stack;
std::map<String, frame_struct_t> frame_map;

uint32_t g_last_active_time_millis = 0;

boolean pressed;
TouchPoint_t pos;
Frame_Base *curFrame;

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_AddObject(GUI_Base *object)
{
	object->SetID(obj_id);
	obj_id++;
	gui_object_list.push_back(object);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_AddObject_Move(GUI_Base *object)
{
	object->SetID(objM_id);
	objM_id++;
	gui_objectM_list.push_back(object);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Draw()
{
	for (std::list<GUI_Base *>::iterator p = gui_object_list.begin(); p != gui_object_list.end(); p++)
		(*p)->Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Process()
{
	for (std::list<GUI_Base *>::iterator p = gui_object_list.begin(); p != gui_object_list.end(); p++)
		(*p)->UpdateState(pos);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Process_Move()
{
	for (std::list<GUI_Base *>::iterator p = gui_objectM_list.begin(); p != gui_objectM_list.end(); p++)
		(*p)->UpdatePosition(pos);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Clear(void)
{
	gui_object_list.clear();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Run(Frame_Base* frame)
{
	if (frame->isRun() == 0)
	{
		frame->exit();
		if (wait_for_delete != NULL)
		{
			delete wait_for_delete;
			wait_for_delete = NULL;
		}
		return;
	}

	GUI_Draw();

	while (1)
	{
		if ((frame->isRun() == 0) || (frame->run() == 0))
		{
			frame->exit();
			if (wait_for_delete != NULL)
			{
				delete wait_for_delete;
				wait_for_delete = NULL;
			}
			return;
		}

		pos = M5.Touch.getPressPoint();

		if ((pos.x != -1) && (pos.y != -1) && (pressed == false))
		{
			pressed = true;
			GUI_Process();
		}
		else if ((pos.x == -1) && (pos.y == -1) && (pressed == true))
		{
			pressed = false;
			GUI_Process();
		}
		else if ((pos.y != -1) && (pressed == true))
		{
			GUI_Process_Move();
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_MainLoop(void)
{
	if ((!frame_stack.empty()) && (frame_stack.top() != NULL))
	{
		curFrame = frame_stack.top();

		log_d("Run %s", curFrame->GetFrameName().c_str());

		GUI_Clear();
		curFrame->init(frame_map[curFrame->GetFrameName()].args);
		GUI_Run(curFrame);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_AddFrame(String name, Frame_Base *frame)
{
	frame_struct_t f;
	f.frame = frame;
	frame_map.insert(std::pair<String, frame_struct_t>(name, f));
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_AddFrameArg(String name, int n, void* arg)
{
	if (frame_map.count(name) == 0)
		return;

	if (frame_map[name].args.size() > n)
		frame_map[name].args[n] = arg;
	else
		frame_map[name].args.push_back(arg);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Base* GUI_GetFrame(String name)
{
	if (frame_map.count(name) > 0)
		return frame_map[name].frame;

	return NULL;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_PushFrame(Frame_Base* frame)
{
	frame_stack.push(frame);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_PopFrame(bool isDelete)
{
	if (isDelete)
		wait_for_delete = frame_stack.top();

	frame_stack.pop();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_UpdateGlobalLastActiveTime()
{
	g_last_active_time_millis = millis();
}
