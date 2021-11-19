#ifndef _GUI_SLIDER_H
#define _GUI_SLIDER_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Slider-Class
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
class GUI_Slider : public GUI_Base
{
	public:
		GUI_Slider(int16_t x, int16_t y, int16_t w, int16_t h, int8_t min, int8_t max, int8_t level);
		~GUI_Slider();
		void init();
		void Draw();
		void UpdateState(TouchPoint_t pos);

	private:
		int8_t _min, _max, _level;
};

#endif
