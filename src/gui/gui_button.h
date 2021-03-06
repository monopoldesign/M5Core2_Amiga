#ifndef _GUI_BUTTON_H
#define _GUI_BUTTON_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Button-Class
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
enum BUTTYPE {BUT_NORMAL, BUT_CLOSEW, BUT_ARROWUP, BUT_ARROWDOWN};

class GUI_Button : public GUI_Base
{
	public:
		GUI_Button(uint8_t type, int16_t x, int16_t y, int16_t w, int16_t h);
		GUI_Button(String label, int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_Button();
		void init();
		void Draw();
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);
		void setLabel(String label);
		boolean isPressed();

	private:
		HotZone *_buttonZone;
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _event = EVENT_NONE;
		uint8_t _type;
		String _label;
};

#endif
