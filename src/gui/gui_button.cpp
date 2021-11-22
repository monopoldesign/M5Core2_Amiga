/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Button-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_button.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Button::GUI_Button(uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_type = type;
	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Button::GUI_Button(String label, int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_label = label;

	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_type = BUT_NORMAL;
	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Button::~GUI_Button()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Button::init()
{
	_buttonZone->setZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Button::Draw()
{
	uint8_t _o = 6;	// Offset for "Arrow"-Triangle
	
	if (_state == EVENT_NONE || _state == EVENT_RELEASED)
	{
		switch (_type)
		{
			case BUT_CLOSEW:
				// Gadget
				M5.Lcd.drawRect(_x, _y, _w, _h, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y, _x + _w, _y, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w, _y, _x + _w, _y + _h - 2, MWB_WHITE);

				// Symbol
				M5.Lcd.fillRect(_x + 8, _y + 7, 10, 12, MWB_WHITE);
				M5.Lcd.drawRect(_x + 8, _y + 7, 10, 12, MWB_BLACK);
				break;

			case BUT_ARROWUP:
				// Gadget
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

				// Symbol
				M5.Lcd.fillTriangle(_x + _o, _y + _h - _o, _x + (_w / 2), _y + _o, _x + _w - _o, _y + _h - _o, MWB_BLACK);
				break;

			case BUT_ARROWDOWN:
				// Gadget
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

				// Symbol
				M5.Lcd.fillTriangle(_x + _o, _y + _o, _x + (_w / 2), _y + _h - _o, _x + _w - _o, _y + _o, MWB_BLACK);
				break;

			default:
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

				if (_label.length() > 0)
				{
					M5.Lcd.setFreeFont(&FreeSans9pt7b);
					M5.Lcd.setTextSize(1);
					M5.Lcd.setTextColor(MWB_BLACK);

					uint16_t xtext = _x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2));
					M5.Lcd.setCursor(xtext, _y + 22);
					M5.Lcd.print(_label);
				}
				break;
		}
	}
	else if (_state == EVENT_PRESSED)
	{
		switch (_type)
		{
			case BUT_CLOSEW:
				// Gadget
				M5.Lcd.drawRect(_x, _y, _w, _h, MWB_WHITE);
				M5.Lcd.drawLine(_x, _y, _x + _w, _y, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h, MWB_BLACK);
				M5.Lcd.drawLine(_x + _w, _y, _x + _w, _y + _h - 2, MWB_WHITE);

				// Symbol
				M5.Lcd.fillRect(_x + 8, _y + 7, 10, 12, MWB_GRAY);
				M5.Lcd.drawRect(_x + 8, _y + 7, 10, 12, MWB_BLACK);
				break;

			case BUT_ARROWUP:
				// Gadget
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_BLUE);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

				// Symbol
				M5.Lcd.fillTriangle(_x + _o, _y + _h - _o, _x + (_w / 2), _y + _o, _x + _w - _o, _y + _h - _o, MWB_BLACK);
				break;

			case BUT_ARROWDOWN:
				// Gadget
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_BLUE);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

				// Symbol
				M5.Lcd.fillTriangle(_x + _o, _y + _o, _x + (_w / 2), _y + _h - _o, _x + _w - _o, _y + _o, MWB_BLACK);
				break;

			default:
				M5.Lcd.fillRect(_x, _y, _w, _h, MWB_BLUE);
				M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
				M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
				M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + 1, _y + _h - 2, _x + _w - 1, _y + _h - 2, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
				M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

				if (_label.length() > 0)
				{
					M5.Lcd.setFreeFont(&FreeSans9pt7b);
					M5.Lcd.setTextSize(1);
					M5.Lcd.setTextColor(MWB_BLACK);

					uint16_t xtext = _x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2));
					M5.Lcd.setCursor(xtext, _y + 22);
					M5.Lcd.print(_label);
				}
				break;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Button::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Button::UpdateState(TouchPoint_t pos)
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
void GUI_Button::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Button::AddArgs(int16_t event, uint16_t n, void* arg)
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
