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

	_selectedItem = 0;
	_selectedMin = 0;

	_itemList = LinkedList<ListItem *>();

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

	_selectedItem = 0;
	clearItemList();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::Draw()
{
	ListItem *_li;

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
		M5.Lcd.drawLine(_x + _w - 2, _y + 1, _x + _w - 2, _y + _h - 1, MWB_BLACK);
	}

	// mark selected Item
	if (!_readOnly)
	{
		if ((_selectedItem >= _selectedMin) && (_selectedItem < (_selectedMin + _maxItems)))
			M5.Lcd.fillRect(_x + 4, _y + ((_selectedItem - _selectedMin) * 18) + 3, _w - (2 * 4), 16, MWB_BLUE);
	}

	// Items
	if (_itemList.size() > 0)
	{
		for (uint8_t i = _selectedMin; i < _itemList.size(); i++)
		{
			if (i >= _selectedMin + _maxItems)
				break;

			M5.Lcd.setCursor(_x + 4, _y + ((i - _selectedMin) * 18) + 16);
			_li = _itemList.get(i);
			memset(_buffer, '\0', sizeof(_buffer));
			strncpy(_buffer, _li->item, _maxChar);
			M5.Lcd.print(_buffer);
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::addItem(String item)
{
	ListItem *_li;

	_li = new ListItem();
	sprintf(_li->item, "%s", item.c_str());
	_itemList.add(_li);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::clearItemList(void)
{
	ListItem *_li;

	if (_itemList.size() > 0)
	{
		while (_itemList.size() > 0)
		{
			_li = _itemList.get(0);
			_itemList.remove(0);
			delete _li;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::UpdateState(TouchPoint_t pos)
{
	for (uint8_t i = 0; i < _maxItems; i++)
	{
		if (_itemZone[i]->inHotZone(pos) && (i + _selectedMin) < _itemList.size())
		{
			if (_state[i] == EVENT_NONE)
			{
				_state[i] = EVENT_PRESSED;
				Draw();

				if (_pressed_cb != NULL)
					_pressed_cb(_pressed_cb_args);
			}
		}
		else
		{
			if (_state[i] == EVENT_PRESSED)
			{
				_selectedItem = i + _selectedMin;
				_isSelected = true;

				_state[i] = EVENT_NONE;
				Draw();

				if (_released_cb != NULL)
					_released_cb(_released_cb_args);
			}
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::UpdatePosition(TouchPoint_t pos)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::setSelectedItem(uint8_t item)
{
	_selectedItem = item;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
String GUI_List::getSelectedItem(void)
{
	ListItem *_li;

	if (_itemList.size() > 0)
	{
		_li = _itemList.get(_selectedItem);
		String item = String(_li->item);
		_isSelected = false;
		return item;
	}

	return String("");
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
uint8_t GUI_List::getSelectedMin(void)
{
	return _selectedMin;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::setSelectedMin(uint8_t min)
{
	_selectedMin = min;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
boolean GUI_List::isSelected(void)
{
	return _isSelected;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::selectNextItem(void)
{
	if (_selectedItem + 1 < _itemList.size())
	{
		_selectedItem++;
		_isSelected = true;

		if (_selectedItem >= _selectedMin + _maxItems)
			_selectedMin++;

		Draw();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::selectPrevItem(void)
{
	if (_selectedItem > 0)
	{
		_selectedItem--;
		_isSelected = true;

		if (_selectedItem < _selectedMin)
			_selectedMin--;

		Draw();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
uint8_t GUI_List::getSize(void)
{
	return _itemList.size();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::Bind(int16_t event, void (*func_cb)(gui_args_vector_t&))
{
	if (event == EVENT_PRESSED)
		_pressed_cb = func_cb;
	else if (event == EVENT_RELEASED)
		_released_cb = func_cb;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void GUI_List::AddArgs(int16_t event, uint16_t n, void* arg)
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
