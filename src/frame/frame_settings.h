#ifndef _FRAME_SETTINGS_H_
#define _FRAME_SETTINGS_H_

#include "frame_base.h"
#include "../gui/gui.h"

class Frame_Settings : public Frame_Base
{
	public:
		Frame_Settings();
		~Frame_Settings();
		int init(gui_args_vector_t &args);
};

#endif
