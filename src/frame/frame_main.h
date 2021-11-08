#ifndef _FRAME_MAIN_H
#define _FRAME_MAIN_H

#include "frame_base.h"

#define MAX_APPS	12

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
		GUI_Button *_key[MAX_APPS];
		uint32_t _next_update_time;
		uint32_t _time;
};

#endif
