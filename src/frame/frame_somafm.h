#ifndef _FRAME_SOMAFM_H_
#define _FRAME_SOMAFM_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: SomaFM-Player
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <Arduino.h>
#include <LinkedList.h>

#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Definitions
*******************************************************************************/
class somaStation
{
	public:
		char name[16];
		char url[64];
};

/******************************************************************************
* Class Definition
*******************************************************************************/
class Frame_SomaFM : public Frame_Base
{
	public:
		Frame_SomaFM();
		~Frame_SomaFM();
		int init(gui_args_vector_t &args);
		int run();
		void loadStations(void);
		void loadConfig(void);
		void saveConfig(void);

	private:
		GUI_Cycle *_cycle;
		GUI_String *_string[2];
		GUI_Button *_but;
		boolean _isPlaying;
		uint8_t _volume;
};

#endif
