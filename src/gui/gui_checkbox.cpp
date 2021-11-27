/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Checkbox-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_checkbox.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Checkbox::GUI_Checkbox(String label, int16_t x, int16_t y, uint8_t placement) : GUI_Base(x, y)
{
	_x = x;
	_y = y;
	_w = 24;
	_h = 24;

	_label = label;
	_placement = placement;

	_isChecked = false;
	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Checkbox::GUI_Checkbox(int16_t x, int16_t y) : GUI_Base(x, y)
{
	_x = x;
	_y = y;
	_w = 24;
	_h = 24;

	_isChecked = false;
	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Checkbox::~GUI_Checkbox()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::init()
{
	_buttonZone->setZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::Draw()
{
	M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
	M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
	M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
	M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
	M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
	M5.Lcd.drawLine(_x, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_BLACK);
	M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
	M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

	if (_isChecked)
		M5.Lcd.drawBitmap(_x + 2, _y + 1, 20, 21, cb_Tick);

	if (_label.length() > 0)
	{
		M5.Lcd.setFreeFont(&FreeSans9pt7b);
		M5.Lcd.setTextSize(1);
		M5.Lcd.setTextColor(MWB_BLACK);

		switch (_placement)
		{
			case PM_LEFT:
				M5.Lcd.setCursor(_x - M5.Lcd.textWidth(_label) - 4, _y + 18);
				break;
			case PM_RIGHT:
				M5.Lcd.setCursor(_x + _w + 8, _y + 18);
				break;
			case PM_ABOVE:
				M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y - 10);
				break;
			case PM_BELOW:
				M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y + 24 + 20);
				break;
		}

		M5.Lcd.print(_label);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::UpdateState(TouchPoint_t pos)
{
	if (_buttonZone->inHotZone(pos))
	{
		if (_state == EVENT_NONE)
		{
			_state = EVENT_PRESSED;
			Draw();

			if (_pressed_cb != NULL)
				_pressed_cb(_pressed_cb_args);
		}
	}
	else
	{
		if (_state == EVENT_PRESSED)
		{
			_state = EVENT_NONE;

			if (_isChecked)
				_isChecked = false;
			else
				_isChecked = true;

			Draw();

			if (_released_cb != NULL)
				_released_cb(_released_cb_args);
		}
		else
			Draw();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Checkbox::AddArgs(int16_t event, uint16_t n, void* arg)
{
	if (event == EVENT_PRESSED)
	{
		if (_pressed_cb_args.size() > n)
			_pressed_cb_args[n] = arg;
		else
			_pressed_cb_args.push_back(arg);
	}
	else if (event == EVENT_RELEASED)
	{
		if (_released_cb_args.size() > n)
			_released_cb_args[n] = arg;
		else
			_released_cb_args.push_back(arg);
	}
}
