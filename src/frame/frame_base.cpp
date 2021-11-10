/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame-Base-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Base::Frame_Base(bool _has_title)
{
	if (_has_title)
	{

	}
	_frame_id = 0;
	_frame_name = "Frame_Base";
	GUI_UpdateGlobalLastActiveTime();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Base::~Frame_Base()
{
	if (_key_exit != NULL)
		delete _key_exit;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exitbtn(void)
{
	_key_exit = new GUI_ImgButton(0, 24, 24, 25, CloseWindowUp, CloseWindowDown);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::bevel(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t type)
{
	switch (type)
	{
		case BT_NORMAL:
			//M5.Lcd.fillRect(x, y, w, h, M5.Lcd.color565(149, 149, 149));
			M5.Lcd.drawLine(x, y, x, y + h - 1, WHITE);
			M5.Lcd.drawLine(x, y, x + w - 1, y, WHITE);
			M5.Lcd.drawLine(x, y + h - 1, x + w - 1, y + h - 1, BLACK);
			M5.Lcd.drawLine(x + w - 1, y, x + w - 1, y + h - 1, BLACK);
			break;

		case BT_RECESSED:
			//M5.Lcd.fillRect(x, y, w, h, M5.Lcd.color565(149, 149, 149));
			M5.Lcd.drawLine(x, y, x, y + h - 1, BLACK);
			M5.Lcd.drawLine(x, y, x + w - 1, y, BLACK);
			M5.Lcd.drawLine(x, y + h - 1, x + w - 1, y + h - 1, WHITE);
			M5.Lcd.drawLine(x + w - 1, y, x + w - 1, y + h - 1, WHITE);
			break;
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Base::run(void)
{
	return _is_run;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exit(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exit_cb(gui_args_vector_t &args)
{
	GUI_PopFrame();
	*((int *)(args[0])) = 0;
}
