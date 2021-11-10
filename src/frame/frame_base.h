#ifndef _FRAME_BASE_H
#define _FRAME_BASE_H

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame-Base-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <Arduino.h>

#include "../gui/gui_imgbutton.h"
#include "../gui/gui_button.h"
#include "../gui/gui_string.h"

/******************************************************************************
* Definitions
*******************************************************************************/
enum BTYPE {BT_NORMAL, BT_RECESSED};

class Frame_Base
{
	public:
		Frame_Base(bool _has_title = true);
		virtual ~Frame_Base();

		void exitbtn();
		void bevel(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t type);

		virtual int run();
		virtual void exit();
		virtual int init(gui_args_vector_t &args) = 0;
		String GetFrameName() { return _frame_name; }
		int isRun() { return _is_run; }
		void SetFrameID(uint32_t id) { _frame_id = id; }
		uint32_t GetFrameID() { return _frame_id; }

	protected:
		static void exit_cb(gui_args_vector_t &args);
		String _frame_name;
		int _is_run = 1;
		GUI_ImgButton *_key_exit = NULL;
		uint32_t _frame_id;

	private:
};

#endif
