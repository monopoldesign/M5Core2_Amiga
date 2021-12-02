/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Cycle-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Core2.h"
#include "gui_cycle.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Cycle::GUI_Cycle(String label, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t placement) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_label = label;
	_placement = placement;

	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Cycle::GUI_Cycle(int16_t x, int16_t y, int16_t w, int16_t h) : GUI_Base(x, y, w, h)
{
	_x = x;
	_y = y;
	_w = w;
	_h = h;

	_buttonZone = new HotZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
GUI_Cycle::~GUI_Cycle()
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::init()
{
	_buttonZone->setZone(_x, _y, _x + _w, _y + _h);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::Draw()
{
	if (_state == EVENT_NONE || _state == EVENT_RELEASED)
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_GRAY);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_WHITE);
		M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);

		// Top-Cycle
		M5.Lcd.drawLine(_x + 7, _y + 2, _x + 13, _y + 2, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + 3, _x + 13, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 14, _y + 3, _x + 14, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + 3, _x + 13, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 11, _y + 5, _x + 16, _y + 5, MWB_BLACK);
		M5.Lcd.drawLine(_x + 12, _y + 6, _x + 15, _y + 6, MWB_BLACK);

		// Cycle-Height	
		M5.Lcd.drawLine(_x + 6, _y + 3, _x + 6, _y + _h - 5, MWB_BLACK);
		M5.Lcd.drawLine(_x + 7, _y + 3, _x + 7, _y + _h - 5, MWB_BLACK);

		// Bottom-Cycle
		M5.Lcd.drawLine(_x + 7, _y + _h - 4, _x + 13, _y + _h - 4, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + _h - 5, _x + 14, _y + _h - 5, MWB_BLACK);

		// Divider
		M5.Lcd.drawLine(_x + 20, _y + 2, _x + 20, _y + _h - 3, MWB_BLACK);
		M5.Lcd.drawLine(_x + 21, _y + 2, _x + 21, _y + _h - 3, MWB_WHITE);

		String _text = getSelectedItemString();
		if (_text.length() > 0)
		{
			M5.Lcd.setFreeFont(&FreeSans9pt7b);
			M5.Lcd.setTextSize(1);
			M5.Lcd.setTextColor(MWB_BLACK);

			uint16_t xtext = _x + 22 + (((_w - 22) / 2) - (M5.Lcd.textWidth(_text) / 2));
			M5.Lcd.setCursor(xtext, _y + 22);
			M5.Lcd.print(_text);
		}

		if (_label.length() > 0)
		{
			M5.Lcd.setFreeFont(&FreeSans9pt7b);
			M5.Lcd.setTextSize(1);
			M5.Lcd.setTextColor(MWB_BLACK);

			switch (_placement)
			{
				case PM_LEFT:
					M5.Lcd.setCursor(_x - M5.Lcd.textWidth(_label) - 4, _y + 22);
					break;
				case PM_RIGHT:
					M5.Lcd.setCursor(_x + _w + 8, _y + 22);
					break;
				case PM_ABOVE:
					M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y - 10);
					break;
				case PM_BELOW:
					M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y + _h + 20);
					break;
			}

			M5.Lcd.print(_label);
		}
	}
	else if (_state == EVENT_PRESSED)
	{
		M5.Lcd.fillRect(_x, _y, _w, _h, MWB_BLUE);
		M5.Lcd.drawLine(_x, _y, _x, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x + 1, _y, _x + 1, _y + _h - 1, MWB_BLACK);
		M5.Lcd.drawLine(_x, _y, _x + _w - 1, _y, MWB_BLACK);
		M5.Lcd.drawLine(_x + 1, _y + _h - 1, _x + _w - 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w - 1, _y, _x + _w - 1, _y + _h - 1, MWB_WHITE);
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_WHITE);

		// Top-Cycle
		M5.Lcd.drawLine(_x + 7, _y + 2, _x + 13, _y + 2, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + 3, _x + 13, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 14, _y + 3, _x + 14, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + 3, _x + 13, _y + 7, MWB_BLACK);
		M5.Lcd.drawLine(_x + 11, _y + 5, _x + 16, _y + 5, MWB_BLACK);
		M5.Lcd.drawLine(_x + 12, _y + 6, _x + 15, _y + 6, MWB_BLACK);

		// Cycle-Height	
		M5.Lcd.drawLine(_x + 6, _y + 3, _x + 6, _y + _h - 5, MWB_BLACK);
		M5.Lcd.drawLine(_x + 7, _y + 3, _x + 7, _y + _h - 5, MWB_BLACK);

		// Bottom-Cycle
		M5.Lcd.drawLine(_x + 7, _y + _h - 4, _x + 13, _y + _h - 4, MWB_BLACK);
		M5.Lcd.drawLine(_x + 13, _y + _h - 5, _x + 14, _y + _h - 5, MWB_BLACK);

		// Divider
		M5.Lcd.drawLine(_x + 20, _y + 2, _x + 20, _y + _h - 3, MWB_WHITE);
		M5.Lcd.drawLine(_x + 21, _y + 2, _x + 21, _y + _h - 3, MWB_BLACK);

		String _text = getSelectedItemString();
		if (_text.length() > 0)
		{
			M5.Lcd.setFreeFont(&FreeSans9pt7b);
			M5.Lcd.setTextSize(1);
			M5.Lcd.setTextColor(MWB_BLACK);

			uint16_t xtext = _x + 22 + (((_w - 22) / 2) - (M5.Lcd.textWidth(_text) / 2));
			M5.Lcd.setCursor(xtext, _y + 22);
			M5.Lcd.print(_text);
		}

		if (_label.length() > 0)
		{
			M5.Lcd.setFreeFont(&FreeSans9pt7b);
			M5.Lcd.setTextSize(1);
			M5.Lcd.setTextColor(MWB_BLACK);
		
			switch (_placement)
			{
				case PM_LEFT:
					M5.Lcd.setCursor(_x - M5.Lcd.textWidth(_label) - 4, _y + 22);
					break;
				case PM_RIGHT:
					M5.Lcd.setCursor(_x + _w + 8, _y + 22);
					break;
				case PM_ABOVE:
					M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y - 10);
					break;
				case PM_BELOW:
					M5.Lcd.setCursor(_x + ((_w / 2) - (M5.Lcd.textWidth(_label) / 2)), _y + _h + 20);
					break;
			}
		
			M5.Lcd.print(_label);
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::UpdateState(TouchPoint_t pos)
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

			_selectedItem++;

			if (_selectedItem > _itemList.size() - 1)
				_selectedItem = 0;

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
void GUI_Cycle::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::addItem(String item)
{
	CycleItem *_ci;

	_ci = new CycleItem();
	sprintf(_ci->item, "%s", item.c_str());
	_itemList.add(_ci);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::clearItemList(void)
{
	CycleItem *_ci;

	if (_itemList.size() > 0)
	{
		while (_itemList.size() > 0)
		{
			_ci = _itemList.get(0);
			_itemList.remove(0);
			delete _ci;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::setSelectedItem(uint8_t item)
{
	_selectedItem = item;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
uint8_t GUI_Cycle::getSelectedItem(void)
{
	return _selectedItem;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
String GUI_Cycle::getSelectedItemString(void)
{
	CycleItem *_ci;

	if (_itemList.size() > 0)
	{
		_ci = _itemList.get(_selectedItem);
		String item = String(_ci->item);
		return item;
	}

	return String("");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_Cycle::AddArgs(int16_t event, uint16_t n, void* arg)
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
