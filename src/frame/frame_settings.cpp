/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Settings
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "frame_settings.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
GUI_String *_but = NULL;

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_changeVal(gui_args_vector_t &args)
{
	_but = (GUI_String *)(args[0]);
	_but->setValue("Hallo");

	_but = (GUI_String *)(args[1]);
	_but->setValue(random(9999));
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_exit(gui_args_vector_t &args)
{
	GUI_PopFrame();
	*((int *)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Settings::Frame_Settings(void)
{
	_frame_name = "Frame_Settings";

	// create String-Field and Number-Field
	_string[0] = new GUI_String("Text:", "Text", 320 - 200 - 16, (1 * (32 + 8)) + 48 + 16, 200, 24);
	_string[1] = new GUI_String("Number:", 1234, 320 - 200 - 16, (2 * (32 + 8)) + 48 + 16, 200, 24);

	// create Button
	_button[0] = new GUI_Button("Change Values", 16, (0 * (32 + 8)) + 48 + 16, 320 - (2 * 16), 32);
	_button[0]->AddArgs(GUI_Button::EVENT_RELEASED, 0, _string[0]);
	_button[0]->AddArgs(GUI_Button::EVENT_RELEASED, 1, _string[1]);
	_button[0]->Bind(GUI_Button::EVENT_RELEASED, &but_changeVal);

	_button[1] = new GUI_Button("Exit", 16, (3 * (32 + 8)) + 48 + 16, 320 - (2 * 16), 32);
	_button[1]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_button[1]->Bind(GUI_Button::EVENT_RELEASED, &but_exit);

	exitbtn();
	_key_exit->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(GUI_Button::EVENT_RELEASED, &Frame_Base::exit_cb);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Settings::~Frame_Settings(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Settings::init(gui_args_vector_t &args)
{
	_is_run = 1;

	M5.Lcd.drawBitmap(0, 24, 320, 216, Window);

	M5.Lcd.setCursor(24 + 8, 41);
	M5.Lcd.println(_frame_name);

	// Bevelbox
	bevel(8, 56, 320 - (2 * 8), 240 - (2 * 24) - (2 * 8), BT_RECESSED);

	for (uint8_t i = 0; i < 2; i++)
	{
		GUI_AddObject(_string[i]);
		_string[i]->init();

		GUI_AddObject(_button[i]);
		_button[i]->init();
	}

	GUI_AddObject(_key_exit);
	_key_exit->init();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Settings::setNewValues(void)
{
	_string[0]->setValue("Hallo");
	_string[1]->setValue(5678);
}
