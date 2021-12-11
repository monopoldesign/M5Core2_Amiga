#ifndef _GUI_CYCLE_H
#define _GUI_CYCLE_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Cycle-Class
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
struct CycleItem
{
	char item[32];
};

class GUI_Cycle : public GUI_Base
{
	public:
		GUI_Cycle(String label, int16_t x, int16_t y, int16_t w, int16_t h, uint8_t placement);
		GUI_Cycle(int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_Cycle();
		void init();
		void Draw();
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void addItem(String item);
		void clearItemList(void);
		void setSelectedItem(uint8_t item);
		uint8_t getSelectedItem(void);
		String getSelectedItemString(void);
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void AddArgs(int16_t event, uint16_t n, void *arg);

	private:
		HotZone *_buttonZone;
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _state = EVENT_NONE;
		String _label;
		uint8_t _placement;
		uint8_t _selectedItem;
		boolean _isSelected;
		LinkedList<CycleItem *> _itemList;
};

#endif
