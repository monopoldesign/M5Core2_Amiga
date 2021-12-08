/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Switch-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_switch.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Switch::GUI_Switch(String label0, String label1, int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_label[0] = label0;
	_label[1] = label1;

	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Switch::~GUI_Switch()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::init()
{
	_buttonZone->setZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::Draw()
{
	if (_event == EVENT_NONE || _event == EVENT_RELEASED)
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
		M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

		switch (_state)
		{
			case 0:
				drawLabel(_label[0]);
				break;
			case 1:
				drawLabel(_label[1]);
				break;
		}
	}
	else if (_event == EVENT_PRESSED)
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_BLUE);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
		M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

		switch (_state)
		{
			case 0:
				drawLabel(_label[0]);
				break;
			case 1:
				drawLabel(_label[1]);
				break;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::drawLabel(String label)
{
	if (label.length() > 0)
	{
		M5.Lcd.setFreeFont(&FreeSans9pt7b);
		M5.Lcd.setTextSize(1);
		M5.Lcd.setTextColor(MWB_BLACK);

		uint16_t xtext = _x + ((_w / 2) - (M5.Lcd.textWidth(label) / 2));
		M5.Lcd.setCursor(xtext, _y + (_h / 2) + 6);
		M5.Lcd.print(label);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::UpdateState(TouchPoint_t pos)
{
	if (_buttonZone->inHotZone(pos))
	{
		if (_event == EVENT_NONE)
		{
			_event = EVENT_PRESSED;
			Draw();

			if (_pressed_cb != NULL)
				_pressed_cb(_pressed_cb_args);
		}
	}
	else
	{
		if (_event == EVENT_PRESSED)
		{
			_event = EVENT_NONE;

			_state++;

			if (_state > 1)
				_state = 0;

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
void GUI_Switch::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::AddArgs(int16_t event, uint16_t n, void* arg)
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

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Switch::setLabel(String label)
{
	_label[0] = label;
	Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
boolean GUI_Switch::isPressed()
{
	if (_event == EVENT_PRESSED)
		return true;
	else
		return false;
}
