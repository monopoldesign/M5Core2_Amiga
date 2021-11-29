#ifndef _FRAME_AUDIO_H_
#define _FRAME_AUDIO_H_

/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Audio-Test
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Class Definition
*******************************************************************************/
enum AType{AT_NONE, AT_RTTTL, AT_MP3, AT_STREAM, AT_TALKIE, AT_SAM};

class Frame_Audio : public Frame_Base
{
	public:
		Frame_Audio();
		~Frame_Audio();
		int init(gui_args_vector_t &args);
		int run();
		//void updateControl();
		//AudioOutput_t updateAudio();

	private:
		GUI_Button *_but[8];
		uint8_t _atype;
		float _volume;
};

#endif
