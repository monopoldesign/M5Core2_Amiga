/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Keyboard-Input-Application
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "frame_input.h"

/******************************************************************************
* Global Variables
*******************************************************************************/

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void keyb_finished(gui_args_vector_t &args)
{
	// send String to Source-Frame
	GUI_Keyboard *_kb = (GUI_Keyboard *)(args[1]);
	String *_str = new String(_kb->getData());
	GUI_AddFrameArg(*(String *)(args[2]), 0, _str);

	// end Input-Frame
	GUI_PopFrame();
	*((int *)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Input::Frame_Input(void)
{
	_frame_name = "Frame_Input";

	_keyb = new GUI_Keyboard();
	_keyb->AddArgs(EVENT_FINISHED, 0, (void *)(&_is_run));
	_keyb->AddArgs(EVENT_FINISHED, 1, _keyb);
	_keyb->AddArgs(EVENT_FINISHED, 2, (String *)(&_sourceFrame));
	_keyb->Bind(EVENT_FINISHED, &keyb_finished);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &exit_cb);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Input::~Frame_Input(void)
{
	delete _key_exit;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Input::init(gui_args_vector_t &args)
{
	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	GUI_AddObject(_keyb);
	_keyb->init();

	if (args.size() > 0)
	{
		// Receive String
		_keyb->setData(*(String *)(args[0]));

		// Receive Source-Frame-Name
		_sourceFrame = *(String *)(args[1]);

		args.pop_back();
	}

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Input::run()
{
	Frame_Base::run();
	return 1;
}
