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

#include "main.h"
#include "gui_base.h"

/******************************************************************************
* Definitions
*******************************************************************************/
class GUI_List : public GUI_Base
{
	public:
		GUI_List(int16_t x, int16_t y, int16_t w, int16_t h, boolean readonly = false);
		~GUI_List();
		void init();
		void Draw();
		void UpdateState(TouchPoint_t pos);

	private:
		HotZone *_itemZone[10];
		uint8_t _selectedItem;
		boolean _readOnly;
		uint8_t _maxChar, _maxItems;
		char _buffer[32];
};

#endif
