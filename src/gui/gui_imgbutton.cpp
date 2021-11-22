/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Image-Button-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_imgbutton.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_ImgButton::GUI_ImgButton(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *imgUp, const uint16_t *imgDown) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);

	_imageUp = imgUp;
	_imageDown = imgDown;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_ImgButton::~GUI_ImgButton()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::init()
{
	_buttonZone->setZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::Draw()
{
	if (_ishide)
		return;

	if (_state == EVENT_NONE || _state == EVENT_RELEASED)
		M5.Lcd.drawBitmap(_x, _y, _w, _h, _imageUp);
	else if (_state == EVENT_PRESSED)
		M5.Lcd.drawBitmap(_x, _y, _w, _h, _imageDown);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::UpdateState(TouchPoint_t pos)
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
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_ImgButton::AddArgs(int16_t event, uint16_t n, void* arg)
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
