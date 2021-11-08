#ifndef _FRAME_BASE_H
#define _FRAME_BASE_H

#include <Arduino.h>

#include "../gui/gui_button.h"

class Frame_Base
{
	public:
		Frame_Base(bool _has_title = true);
		virtual ~Frame_Base();
		void exitbtn();
		virtual int run();
		virtual void exit();
		virtual int init(gui_args_vector_t &args) = 0;
		String GetFrameName() { return _frame_name; }
		int isRun() { return _is_run; }
		void SetFrameID(uint32_t id) { _frame_id = id; }
		uint32_t GetFrameID() { return _frame_id; }

	protected:
		static void exit_cb(gui_args_vector_t &args);
		String _frame_name;
		int _is_run = 1;
		GUI_Button *_key_exit = NULL;
		uint32_t _frame_id;

	private:
};

#endif
