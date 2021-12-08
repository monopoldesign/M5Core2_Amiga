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
#include "frame_keyb.h"
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
void string_input(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_Input");

	if (frame == NULL)
	{
		frame = new Frame_Input();
		GUI_AddFrame("Frame_Input", frame);
	}

	// send current String to Input-Frame
	GUI_String *_str = (GUI_String *)(args[0]);
	String *text = new String(_str->getValue());
	GUI_AddFrameArg("Frame_Input", 0, text);

	// send Frame-Name to Input-Frame
	String *fr = new String("Frame_Keyb");
	GUI_AddFrameArg("Frame_Input", 1, fr);

	// end application
	GUI_PushFrame(frame);
	*((int*)(args[1])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Keyb::Frame_Keyb(void)
{
	_frame_name = "Frame_Keyb";

	_string = new GUI_String("\0", "abc", WINLEFT + 8, WINTOP + 8 + 24, 320 - (2 * WINLEFT) - (2 * 8), 24);

	_string->AddArgs(EVENT_RELEASED, 0, _string);
	_string->AddArgs(EVENT_RELEASED, 1, (void *)(&_is_run));
	_string->Bind(EVENT_RELEASED, &string_input);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &exit_cb);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Keyb::~Frame_Keyb(void)
{
	delete _key_exit;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Keyb::init(gui_args_vector_t &args)
{
	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	GUI_AddObject(_string);
	_string->init();

	if (args.size() > 0)
	{
		// set new Text from Input-Frame
		_string->setValue(*(String *)(args[0]));
		args.pop_back();
	}

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Keyb::run()
{
	Frame_Base::run();
	return 1;
}
