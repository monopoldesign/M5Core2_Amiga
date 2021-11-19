/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-List-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_list.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_List::GUI_List(int16_t x, int16_t y, int16_t w, int16_t h, boolean readonly) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_readOnly = readonly;

	_maxChar = _w / 11;
	_maxItems = _h / 18;

	for (uint8_t i = 0; i < _maxItems; i++)
		_itemZone[i] = new HotZone(_x + 4, _y + (i * 18) + 3, _x + 4 + _w - (2 * 4), _y + (i * 18) + 3 + 16);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_List::~GUI_List()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::init()
{
	for (uint8_t i = 0; i < _maxItems; i++)
		_itemZone[i]->setZone(_x + 4, _y + (i * 18) + 3, _x + 4 + _w - (2 * 4), _y + (i * 18) + 3 + 16);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::Draw()
{
	// Listview-Frame
	if (_readOnly)
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
		M5.Lcd.drawRect(_x, _y, _w, _h, MWB_WHITE);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
	}
	else
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
		M5.Lcd.drawRect(_x, _y, _w, _h, MWB_BLACK);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x, _y, _x + _w - 2, _y, MWB_WHITE);

		M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 2, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w, _y, _x + _w, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);
	}

	M5.Lcd.fillRect(_x + 4, _y + (_selectedItem * 18) + 3, _w - (2 * 4), 16, MWB_BLUE);

	// Items
	for (uint8_t i = 0; i < _maxItems; i++)
	{
		M5.Lcd.setCursor(_x + 4, _y + (i * 18) + 16);
		sprintf(buffer, "Item%01d", i + 1);
		strncpy(_buffer, buffer, _maxChar);
		M5.Lcd.print(buffer);
	}

}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::UpdateState(TouchPoint_t pos)
{
	for (uint8_t i = 0; i < _maxItems; i++)
	{
		if (_itemZone[i]->inHotZone(pos))
			_selectedItem = i;
	}

	Draw();
}
