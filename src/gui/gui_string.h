#ifndef _GUI_STRING_H
#define _GUI_STRING_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-String-Class
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
class GUI_String : public GUI_Base
{
	public:
		GUI_String(String label, String content, int16_t x, int16_t y, int16_t w, int16_t h);
		GUI_String(String label, uint32_t content, int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_String();
		void init();
		void Draw();
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t &));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);
		void setValue(String newString);
		void setValue(uint32_t newValue);
		String getValue();

	private:
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _event = EVENT_NONE;

		String _label;
		String _content;
		String _contentOrig;
		uint8_t _maxLen;
		char _buffer[32];

		HotZone *_buttonZone;
};

#endif
