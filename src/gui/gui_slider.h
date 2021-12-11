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
		GUI_Slider(int16_t x, int16_t y, int16_t w, int16_t h);
		~GUI_Slider();
		void init();

		void setMin(int8_t min);
		void setMax(int8_t max);
		void setLevel(int8_t level);
		uint8_t getSelectedMin(void);
		void setSelectedMin(uint8_t min);
		void calculate(void);

		void Draw();
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void AddArgs(int16_t event, uint16_t n, void *arg);

	private:
		HotZone *_knobZone;
		TouchPoint_t _startPos, _oldPos;
		boolean _isMoveable;

		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _state = EVENT_NONE;

		int8_t _min, _max, _level;
		float _sizeY, _step;
		int8_t _selMin;
};

#endif
