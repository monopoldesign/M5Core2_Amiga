#ifndef _FRAME_WIFIMAN_H_
#define _FRAME_WIFIMAN_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Wifi-Manager
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_WifiMan : public Frame_Base
{
	public:
		Frame_WifiMan();
		~Frame_WifiMan();
		int init(gui_args_vector_t &args);

	private:
		GUI_List *_list;
		GUI_Button *_but[2];
};

#endif
