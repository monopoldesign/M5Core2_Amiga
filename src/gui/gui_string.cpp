/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-String-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_string.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_String::GUI_String(String label, String content, int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_label = label;
	_content = content;
	_contentOrig = content;

	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_maxLen = _w / 11;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_String::GUI_String(String label, uint32_t content, int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_label = label;
	_content = String(content, DEC);
	_contentOrig = _content;

	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_String::~GUI_String()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_String::init()
{
	_content = _contentOrig;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_String::Draw()
{
	if (_ishide)
		return;

	M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
	M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
	M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
	M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
	M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
	M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
	M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

	if (_label.length() > 0)
	{
		M5.Lcd.setFreeFont(&FreeSans9pt7b);
		M5.Lcd.setTextSize(1);
		M5.Lcd.setTextColor(MWB_BLACK);

		uint8_t _xs = _x - M5.Lcd.textWidth(_label) - 8;
		M5.Lcd.setCursor(_xs, _y + 16);
		M5.Lcd.print(_label);
	}

	if (_content.length() > 0)
	{
		M5.Lcd.setFreeFont(&FreeSans9pt7b);
		M5.Lcd.setTextSize(1);
		M5.Lcd.setTextColor(MWB_BLACK);

		M5.Lcd.setCursor(_x + 4, _y + 17);
		memset(_buffer, '\0', sizeof(_buffer));
		strncpy(_buffer, _content.c_str(), _maxLen);
		M5.Lcd.print(_buffer);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_String::UpdateState(TouchPoint_t pos)
{
	Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_String::setValue(String newString)
{
	_content = newString;
	Draw();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_String::setValue(uint32_t newValue)
{
	_content = String(newValue, DEC);
	Draw();
}
