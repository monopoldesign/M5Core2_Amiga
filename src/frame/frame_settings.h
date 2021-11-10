#ifndef _FRAME_SETTINGS_H_
#define _FRAME_SETTINGS_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Settings
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_Settings : public Frame_Base
{
	public:
		Frame_Settings();
		~Frame_Settings();
		int init(gui_args_vector_t &args);
		void setNewValues(void);

	private:
		GUI_Button *_button[2];
		GUI_String *_string[2];
};

#endif
