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
		static const int16_t EVENT_NONE = 0;
		static const int16_t EVENT_PRESSED = 1;
		static const int16_t EVENT_RELEASED = 2;

	public:
		GUI_String(String label, String content, int16_t x, int16_t y, int16_t w, int16_t h);
		GUI_String(String label, uint32_t content, int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_String();
		void init();
		void Draw();
		void UpdateState(TouchPoint_t pos);

		void setValue(String newString);
		void setValue(uint32_t newValue);

	private:
		gui_args_vector_t _cb_args;
		String _label;
		String _content;
		String _contentOrig;
		uint8_t _maxLen;
		char _buffer[32];
};

#endif
