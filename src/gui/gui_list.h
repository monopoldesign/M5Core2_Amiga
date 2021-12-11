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
		void UpdatePosition(TouchPoint_t pos);

		void setSelectedItem(uint8_t item);
		String getSelectedItem(void);
		uint8_t getSelectedMin(void);
		void setSelectedMin(uint8_t min);
		boolean isSelected(void);
		void selectNextItem(void);
		void selectPrevItem(void);
		uint8_t getSize(void);

		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void AddArgs(int16_t event, uint16_t n, void *arg);

	private:
		HotZone *_itemZone[16];

		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _state[16] = {EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE, EVENT_NONE};

		uint8_t _selectedItem, _selectedMin;
		boolean _readOnly, _isSelected;
		uint8_t _maxChar, _maxItems;
		char _buffer[32];
		LinkedList<ListItem *> _itemList;
};

#endif
