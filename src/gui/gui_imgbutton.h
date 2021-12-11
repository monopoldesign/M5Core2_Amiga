#ifndef _GUI_IMGBUTTON_H
#define _GUI_IMGSBUTTON_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* GUI-Image-Button-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>
#include "gui_base.h"
#include "../resources/ImageResource.h"

/******************************************************************************
* Definitions
*******************************************************************************/
class GUI_ImgButton : public GUI_Base
{
	public:
		GUI_ImgButton(int16_t x, int16_t y, int16_t w, int16_t h, const uint16_t *imgUp, const uint16_t *imgDown);
		~GUI_ImgButton();
		void init();
		void Draw();
		void Bind(int16_t event, void (*func_cb)(gui_args_vector_t&));
		void UpdateState(TouchPoint_t pos);
		void UpdatePosition(TouchPoint_t pos);
		void AddArgs(int16_t event, uint16_t n, void *arg);

	private:
		HotZone *_buttonZone;
		void (*_pressed_cb)(gui_args_vector_t& args) = NULL;
		void (*_released_cb)(gui_args_vector_t& args) = NULL;
		gui_args_vector_t _pressed_cb_args;
		gui_args_vector_t _released_cb_args;
		int16_t _state = EVENT_NONE;
		const uint16_t *_imageUp;
		const uint16_t *_imageDown;
};

#endif
