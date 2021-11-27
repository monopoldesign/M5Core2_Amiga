#ifndef _GUI_CHECKBOX_H
#define _GUI_CHECKBOX_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Checkbox-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "gui_base.h"

#include "../resources/ImageResource.h"

/******************************************************************************
* Definitions
*******************************************************************************/
class GUI_Checkbox : public GUI_Base
{
	public:
		GUI_Checkbox(String label, int16_t x, int16_t y, uint8_t placement);
		GUI_Checkbox(int16_t x, int16_t y);
		~GUI_Checkbox();
		void init();
		void Draw();
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);

	private:
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _state = EVENT_NONE;
		String _label;
		uint8_t _placement;
		boolean _isChecked;
		HotZone *_buttonZone;
};

#endif
