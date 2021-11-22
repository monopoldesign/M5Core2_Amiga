/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Slider-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_slider.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Slider::GUI_Slider(int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h  - (2 * 32);
	_sizeY = _h - 4;
	_knobZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Slider::~GUI_Slider()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::init()
{
	if (_isMoveable)
		_knobZone->setZone(_x + 4, _y + 2 + (_selMin * _step), _x + 4 + _w - 8, _y + 2 + (_selMin * _step) + _sizeY);
	else
		_knobZone->setZone(_x + 4, _y + 2, _x + 4 + _w - 8, _y + 2 + _sizeY);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::setMin(int8_t min)
{
	_min = min;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::setMax(int8_t max)
{
	_max = max;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::setLevel(int8_t level)
{
	_level = level;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
uint8_t GUI_Slider::getSelectedMin(void)
{
	return _selMin;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::setSelectedMin(uint8_t min)
{
	_selMin = min;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::calculate(void)
{
	if (_max > ((_h + (2 * 32))/ 18))
		_isMoveable = true;
	else
		_isMoveable = false;

	if (_isMoveable)
	{
		_sizeY = ((float)_h - 4.0) * ((((float)_h + (2.0 * 32.0) - 4.0) / 18.0) / (float)_max);
		_step = ((float)_h - 4.0 - _sizeY) / ((float)_max - (((float)_h + (2.0 * 32.0) - 4.0) / 18.0));
	}
	else
		_sizeY = (float)_h - (float)4.0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::Draw()
{
	// Frame
	M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
	M5.Lcd.drawRect(_x, _y, _w, _h, MWB_BLACK);
	M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
	M5.Lcd.drawLine(_x, _y, _x + _w - 2, _y, MWB_WHITE);

	// Bevel
	M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 2, MWB_WHITE);
	M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

	// Knob
	if (_state == EVENT_NONE || _state == EVENT_RELEASED)
	{
		if (_isMoveable)
			M5.Lcd.fillRect(_x + 4, _y + 2 + (_selMin * _step), _w - 8, _sizeY, MWB_BLACK);
		else
			M5.Lcd.fillRect(_x + 4, _y + 2, _w - 8, _sizeY, MWB_BLACK);
	}
	else if (_state == EVENT_PRESSED || _state == EVENT_MOVED)
	{
		if (_isMoveable)
			M5.Lcd.fillRect(_x + 4, _y + 2 + (_selMin * _step), _w - 8, _sizeY, MWB_BROWN);
		else
			M5.Lcd.fillRect(_x + 4, _y + 2, _w - 8, _sizeY, MWB_BROWN);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::UpdateState(TouchPoint_t pos)
{
	if (_knobZone->inHotZone(pos))
	{
		if (_state == EVENT_NONE)
		{
			_startPos = pos;
			_state = EVENT_PRESSED;
			Draw();

			if (_pressed_cb != NULL)
				_pressed_cb(_pressed_cb_args);
		}
	}
	else
	{
		if (_state == EVENT_PRESSED || _state == EVENT_MOVED)
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
void GUI_Slider::UpdatePosition(TouchPoint_t pos)
{
	if (_state == EVENT_PRESSED || _state == EVENT_MOVED)
	{
		if (_isMoveable)
		{
			if (pos.y != _oldPos.y)
			{
				_state = EVENT_MOVED;
				_oldPos = pos;
				_selMin += (pos.y - _startPos.y) / _step;

				if (_selMin < 0)
					_selMin = 0;

				if (_selMin > _max - ((_h + (2 * 32))/ 18))
					_selMin = _max - ((_h + (2 * 32))/ 18);

				Draw();

				_knobZone->setZone(_x + 4, _y + 2 + (_selMin * _step), _x + 4 + _w - 8, _y + 2 + (_selMin * _step) + _sizeY);

				if (_pressed_cb != NULL)
					_pressed_cb(_pressed_cb_args);
			}
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED || event == EVENT_MOVED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Slider::AddArgs(int16_t event, uint16_t n, void* arg)
{
	if (event == EVENT_PRESSED || event == EVENT_MOVED)
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
