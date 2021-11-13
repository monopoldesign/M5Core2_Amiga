#ifndef _FRAME_MAIN_H
#define _FRAME_MAIN_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Main
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"

#include "M5Settings.h"

/******************************************************************************
* Definitions
*******************************************************************************/
enum {kKeySettings = 0};

#define KEY_W	70
#define KEY_H	38

#define MAX_APPS	12

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_Main : public Frame_Base
{
	public:
		Frame_Main();
		~Frame_Main();
		int run();
		int init(gui_args_vector_t &args);
		void StatusBar();
		void AppName();

	private:
		GUI_ImgButton *_key[MAX_APPS];
		uint32_t _next_update_time;
		uint32_t _time;
};

#endif
