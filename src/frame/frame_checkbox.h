#ifndef _FRAME_CHECKBOX_H_
#define _FRAME_CHECKBOX_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Checkbox-Test
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Class Definition
*******************************************************************************/

class Frame_Checkbox : public Frame_Base
{
	public:
		Frame_Checkbox();
		~Frame_Checkbox();
		int init(gui_args_vector_t &args);
		int run();

	private:
		GUI_Button *_but[2];
		GUI_Checkbox *_cb[8 * 8];
};

#endif
