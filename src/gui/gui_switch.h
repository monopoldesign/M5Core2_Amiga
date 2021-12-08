#ifndef _GUI_SWITCH_H
#define _GUI_SWITCH_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Switch-Class
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
class GUI_Switch : public GUI_Base
{
	public:
		GUI_Switch(String label0, String label1, int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_Switch();
		void init();
		void Draw();
		void drawLabel(String label);
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t &));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);
		void setLabel(String label);
		boolean isPressed();

	private:
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _event = EVENT_NONE;
		uint8_t _state = 0;
		String _label[2];
		HotZone *_buttonZone;
};

#endif
