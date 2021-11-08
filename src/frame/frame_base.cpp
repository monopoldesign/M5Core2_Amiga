#include "frame_base.h"
#include "../gui/gui.h"

Frame_Base::Frame_Base(bool _has_title)
{
	if (_has_title)
	{

	}
	_frame_id = 0;
	_frame_name = "Frame_Base";
	GUI_UpdateGlobalLastActiveTime();
}

Frame_Base::~Frame_Base()
{
	if (_key_exit != NULL)
		delete _key_exit;
}

void Frame_Base::exitbtn(void)
{
	_key_exit = new GUI_Button("close", 0, 24, 24, 25, CloseWindowUp, CloseWindowDown);
}

int Frame_Base::run(void)
{
	return _is_run;
}

void Frame_Base::exit(void)
{

}

void Frame_Base::exit_cb(gui_args_vector_t &args)
{
	GUI_PopFrame();
	*((int *)(args[0])) = 0;
}
