#ifndef _FRAME_INPUT_H_
#define _FRAME_INPUT_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Keyboard-Input-Application
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <Arduino.h>

#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Definitions
*******************************************************************************/

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_Input : public Frame_Base
{
	public:
		Frame_Input();
		~Frame_Input();
		int init(gui_args_vector_t &args);
		int run();

	private:
		GUI_String *_string;
		GUI_Keyboard *_keyb;
		String _sourceFrame;
};

#endif
