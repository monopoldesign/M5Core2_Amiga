#ifndef _FRAME_SYSINFO_H_
#define _FRAME_SYSINFO_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: SysInfo
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_SysInfo : public Frame_Base
{
	public:
		Frame_SysInfo();
		~Frame_SysInfo();
		int init(gui_args_vector_t &args);

	private:
		GUI_String *_string[6];
};

#endif
