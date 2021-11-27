/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Audio-Test
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>
#include <AudioFileSourcePROGMEM.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorRTTTL.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceID3.h>
#include <AudioOutputI2S.h>

#include "main.h"
#include "frame_audio.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
AudioGeneratorRTTTL *rtttl;
AudioFileSourcePROGMEM *file;

AudioFileSourceSD *source;
AudioFileSourceID3 *id3;
AudioGeneratorMP3 *mp3;

AudioOutputI2S *out;

const char melody0[] PROGMEM = "Knight Rider:d=32,o=5,b=63:16e,f,e,8b,16e6,f6,e6,8b,16e,f,e,16b,16e6,4d6,8p,4p,16e,f,e,8b,16e6,f6,e6,8b,16e,f,e,16b,16e6,4f6";
const char melody1[] PROGMEM = "Mission Impossible:d=16,o=5,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,p,a#4,c";

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play0(gui_args_vector_t &args)
{
	if (*((uint8_t *)(args[0])) == AT_RTTTL)
	{
		rtttl->stop();
		delete file;
	}

	source->open("/Yua.mp3");
	id3 = new AudioFileSourceID3(source);
	mp3->begin(id3, out);

	*((uint8_t *)(args[0])) = AT_MP3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play1(gui_args_vector_t &args)
{
	if (*((uint8_t *)(args[0])) == AT_MP3)
	{
		mp3->stop();
		delete id3;
	}

	file = new AudioFileSourcePROGMEM(melody1, strlen_P(melody1));
	rtttl->begin(file, out);
	*((uint8_t *)(args[0])) = AT_RTTTL;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_stop(gui_args_vector_t &args)
{
	switch (*((uint8_t *)(args[0])))
	{
		case AT_RTTTL:
			rtttl->stop();
			delete file;
			break;
		case AT_MP3:
			mp3->stop();
			delete id3;
			break;
	}
	*((uint8_t *)(args[0])) = AT_NONE;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_close(gui_args_vector_t &args)
{
	switch (*((uint8_t *)(args[0])))
	{
		case AT_RTTTL:
			rtttl->stop();
			delete file;
			break;
		case AT_MP3:
			mp3->stop();
			delete id3;
			break;
	}

	*((uint8_t *)(args[0])) = AT_NONE;

	GUI_PopFrame();
	*((int *)(args[1])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Audio::Frame_Audio(void)
{
	_frame_name = "Frame_Audio";

	_but[0] = new GUI_Button("MP3-File from SD-Card", 32, 20 + (2 * 24) + (0 * 32), 256, 32);
	_but[1] = new GUI_Button("Mission Impossible", 32, 20 + (2 * 24) + (1 * 32) + (1 * 8), 256, 32);
	_but[2] = new GUI_Button("Stop", 32, 20 + (2 * 24) + (2 * 32) + (2 * 8), 256, 32);
	_but[3] = new GUI_Button("Exit", 32, 20 + (2 * 24) + (3 * 32) + (3 * 8), 256, 32);

	_but[0]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[0]->Bind(EVENT_RELEASED, &but_play0);

	_but[1]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[1]->Bind(EVENT_RELEASED, &but_play1);

	_but[2]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[2]->Bind(EVENT_RELEASED, &but_stop);

	_but[3]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[3]->AddArgs(EVENT_RELEASED, 1, (void *)(&_is_run));
	_but[3]->Bind(EVENT_RELEASED, &but_close);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_key_exit->AddArgs(EVENT_RELEASED, 1, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &but_close);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Audio::~Frame_Audio(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Audio::init(gui_args_vector_t &args)
{
	wifiNetwork *_wn;

	_is_run = 1;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	for (uint8_t i = 0; i < 4; i++)
	{
		GUI_AddObject(_but[i]);
		_but[i]->init();
	}

	out = new AudioOutputI2S();
	out->SetPinout(12, 0, 2);
	out->SetOutputModeMono(true);
	
	rtttl = new AudioGeneratorRTTTL();

	source = new AudioFileSourceSD();
	mp3 = new AudioGeneratorMP3();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Audio::run()
{
	//Frame_Base::run();

	switch (_atype)
	{
		case AT_RTTTL:
			if (rtttl->isRunning())
			{
				if (!rtttl->loop())
					rtttl->stop();
			}
			break;
		case AT_MP3:
			if (mp3->isRunning())
			{
				if (!mp3->loop())
					mp3->stop();
			}
			break;
	}

	return 1;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
//void Frame_Audio::updateControl()
//{
//}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
//AudioOutput_t Frame_Audio::updateAudio()
//{
//}
