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
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_menu0(gui_args_vector_t &args)
{
	Serial.println("Menu1");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_menu1(gui_args_vector_t &args)
{
	Serial.println("Menu2");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_menu2(gui_args_vector_t &args)
{
	Serial.println("Menu3");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_menu3(gui_args_vector_t &args)
{
	Serial.println("Menu4");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Settings::Frame_Settings(void)
{
	_frame_name = "Frame_Settings";

	M5.Lcd.drawBitmap(0, 24, 320, 216, Window);

	M5.Lcd.setCursor(24 + 8, 41);
	M5.Lcd.println(_frame_name);

	for (uint8_t i = 0; i < 4; i++)
	{
		sprintf(buffer, "Menu %01d", i + 1);
		_key[i] = new GUI_Button(buffer, 8, (i * (32 + 8)) + 64, 304, 32, ButttonUp, ButtonDown);
	}

	_key[0]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[0]->Bind(GUI_Button::EVENT_RELEASED, &key_menu0);

	_key[1]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[1]->Bind(GUI_Button::EVENT_RELEASED, &key_menu1);

	_key[2]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[2]->Bind(GUI_Button::EVENT_RELEASED, &key_menu2);

	_key[3]->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key[3]->Bind(GUI_Button::EVENT_RELEASED, &key_menu3);

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

	for (uint8_t i = 0; i < 4; i++)
	{
		GUI_AddObject(_key[i]);
		_key[i]->init();
	}

	GUI_AddObject(_key_exit);
	_key_exit->init();

	return 3;
}
