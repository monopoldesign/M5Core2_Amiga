#ifndef _GUI_LIST_H
#define _GUI_LIST_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-List-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>
#include <LinkedList.h>

#include "main.h"
#include "gui_base.h"

/******************************************************************************
* Definitions
*******************************************************************************/
struct ListItem
{
	char item[32];
};

class GUI_List : public GUI_Base
{
	public:
		GUI_List(int16_t x, int16_t y, int16_t w, int16_t h, boolean readonly = false);
		~GUI_List();
		void init();
		void Draw();
		void addItem(String item);
		void clearItemList(void);
		void UpdateState(TouchPoint_t pos);
		String getSelectedItem(void);
		boolean isSelected(void);
		void selectNextItem(void);
		void selectPrevItem(void);

	private:
		HotZone *_itemZone[10];
		uint8_t _selectedItem, _selectedMin;
		boolean _readOnly, _isSelected;
		uint8_t _maxChar, _maxItems;
		char _buffer[32];
		LinkedList<ListItem *> _itemList;
};

#endif
