#ifndef _GUI_KEYBOARD_H
#define _GUI_KEYBOARD_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Keyboard-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>

#include "main.h"
#include "gui_base.h"

#include "../gui/gui_button.h"
#include "../gui/gui_string.h"
#include "../gui/gui_switch.h"

/******************************************************************************
* Definitions
*******************************************************************************/
class GUI_Keyboard : public GUI_Base
{
	public:
		GUI_Keyboard();
		~GUI_Keyboard();
		void init();
		void Draw();
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t &));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);
		void setData(String text);
		String getData();
		void setString();

	private:
		void (*_finished_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _finished_cb_args;
		int16_t _event = EVENT_NONE;

		GUI_Button *_but[29];
		GUI_Switch *_sw[2];
		GUI_String *_string;
		String _data;
		uint8_t _layout = 0;
		HotZone *_buttonZone;
};

#endif
