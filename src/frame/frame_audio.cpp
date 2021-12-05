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
#include <AudioFileSourceSPIFFS.h>
#include <AudioFileSourceID3.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorRTTTL.h>
#include <AudioGeneratorMP3.h>
#include <AudioGeneratorTalkie.h>
#include <AudioGeneratorMOD.h>
#include <ESP8266SAM.h>
#include <AudioOutputI2S.h>

#include "main.h"
#include "frame_audio.h"

#include "../enigma.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
AudioGeneratorRTTTL *rtttl;
AudioFileSourcePROGMEM *file;
AudioFileSourceSD *sourceSD;
AudioFileSourceSPIFFS *sourceSPIFFS;
AudioFileSourceID3 *id3;
AudioGeneratorMP3 *mp3;
AudioGeneratorTalkie *talkie;
ESP8266SAM *sam;
AudioFileSourceICYStream *stream;
AudioFileSourceBuffer *streamBuf;
AudioGeneratorMOD *mod;
AudioOutputI2S *out;

const char melody1[] PROGMEM = "Mission Impossible:d=16,o=5,b=100:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c6,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,p,a#4,c";
const char *URL="http://ice4.somafm.com/synphaera-128-mp3";
uint8_t spREADY[] PROGMEM = {0x6A,0xB4,0xD9,0x25,0x4A,0xE5,0xDB,0xD9,0x8D,0xB1,0xB2,0x45,0x9A,0xF6,0xD8,0x9F,0xAE,0x26,0xD7,0x30,0xED,0x72,0xDA,0x9E,0xCD,0x9C,0x6D,0xC9,0x6D,0x76,0xED,0xFA,0xE1,0x93,0x8D,0xAD,0x51,0x1F,0xC7,0xD8,0x13,0x8B,0x5A,0x3F,0x99,0x4B,0x39,0x7A,0x13,0xE2,0xE8,0x3B,0xF5,0xCA,0x77,0x7E,0xC2,0xDB,0x2B,0x8A,0xC7,0xD6,0xFA,0x7F};

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void StatusCallback(void *cbData, int code, const char *string)
{
	const char *ptr = reinterpret_cast<const char *>(cbData);
	// Note that the string may be in PROGMEM, so copy it to RAM for printf
	char s1[64];
	strncpy_P(s1, string, sizeof(s1));
	s1[sizeof(s1)-1] = 0;
	Serial.printf("STATUS(%s) '%d' = '%s'\n", ptr, code, s1);
	Serial.flush();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
	const char *ptr = reinterpret_cast<const char *>(cbData);
	(void) isUnicode; // Punt this ball for now
	// Note that the type and string may be in PROGMEM, so copy them to RAM for printf
	char s1[32], s2[64];
	strncpy_P(s1, type, sizeof(s1));
	s1[sizeof(s1)-1]=0;
	strncpy_P(s2, string, sizeof(s2));
	s2[sizeof(s2)-1]=0;
	Serial.printf("METADATA(%s) '%s' = '%s'\n", ptr, s1, s2);
	Serial.flush();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void stopAny(gui_args_vector_t &args)
{
	switch (*((uint8_t *)(args[0])))
	{
		case AT_MP3:
			mp3->stop();
			delete id3;
			break;
		case AT_RTTTL:
			rtttl->stop();
			delete file;
			break;
		case AT_STREAM:
			mp3->stop();
			delete streamBuf;
			delete stream;
			break;
		case AT_MOD:
			mod->stop();
			delete file;
			break;
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play0(gui_args_vector_t &args)
{
	stopAny(args);

	sourceSD->open("/Yua.mp3");
	id3 = new AudioFileSourceID3(sourceSD);
	mp3->begin(id3, out);

	*((uint8_t *)(args[0])) = AT_MP3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play1(gui_args_vector_t &args)
{
	stopAny(args);

	file = new AudioFileSourcePROGMEM(melody1, strlen_P(melody1));
	rtttl->begin(file, out);
	*((uint8_t *)(args[0])) = AT_RTTTL;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play2(gui_args_vector_t &args)
{
	stopAny(args);

	stream = new AudioFileSourceICYStream(URL);
	stream->RegisterMetadataCB(MDCallback, (void*)"ICY");

	streamBuf = new AudioFileSourceBuffer(stream, 4096);
	streamBuf->RegisterStatusCB(StatusCallback, (void *)"buffer");

	mp3->RegisterMetadataCB(MDCallback, (void *)"mp3");
	mp3->begin(streamBuf, out);

	*((uint8_t *)(args[0])) = AT_STREAM;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play4(gui_args_vector_t &args)
{
	stopAny(args);

	talkie->begin(nullptr, out);
	talkie->say(spREADY, sizeof(spREADY));

	*((uint8_t *)(args[0])) = AT_TALKIE;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play5(gui_args_vector_t &args)
{
	out->begin();

	sam = new ESP8266SAM;
	sam->SetVoice(VOICE_SAM);
	sam->Say(out, "Can you hear me ?");

	*((uint8_t *)(args[0])) = AT_SAM;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_play6(gui_args_vector_t &args)
{
	stopAny(args);

	file = new AudioFileSourcePROGMEM(enigma_mod, sizeof(enigma_mod));
	//sourceSPIFFS->open("/mod.bootup");

	mod->SetBufferSize(3 * 1024);
	mod->SetSampleRate(44100);
	mod->SetStereoSeparation(32);
	//mod->begin(sourceSPIFFS, out);
	mod->begin(file, out);

	*((uint8_t *)(args[0])) = AT_MOD;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_stop(gui_args_vector_t &args)
{
	stopAny(args);
	*((uint8_t *)(args[0])) = AT_NONE;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void but_close(gui_args_vector_t &args)
{
	stopAny(args);
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

	_but[0] = new GUI_Button("MP3-File", 32, 20 + (2 * 24) + (0 * 32), 120, 32);
	_but[1] = new GUI_Button("RTTTL", 32, 20 + (2 * 24) + (1 * 32) + (1 * 8), 120, 32);
	_but[2] = new GUI_Button("Stream", 32, 20 + (2 * 24) + (2 * 32) + (2 * 8), 120, 32);
	_but[3] = new GUI_Button("Stop", 32, 20 + (2 * 24) + (3 * 32) + (3 * 8), 120, 32);

	_but[4] = new GUI_Button("Talkie", 32 + 120 + 16, 20 + (2 * 24) + (0 * 32), 120, 32);
	_but[5] = new GUI_Button("SAM", 32 + 120 + 16, 20 + (2 * 24) + (1 * 32) + (1 * 8), 120, 32);
	_but[6] = new GUI_Button("MOD", 32 + 120 + 16, 20 + (2 * 24) + (2 * 32) + (2 * 8), 120, 32);
	_but[7] = new GUI_Button("Exit", 32 + 120 + 16, 20 + (2 * 24) + (3 * 32) + (3 * 8), 120, 32);

	_but[0]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[0]->Bind(EVENT_RELEASED, &but_play0);

	_but[1]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[1]->Bind(EVENT_RELEASED, &but_play1);

	_but[2]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[2]->Bind(EVENT_RELEASED, &but_play2);

	_but[3]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[3]->Bind(EVENT_RELEASED, &but_stop);

	_but[4]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[4]->Bind(EVENT_RELEASED, &but_play4);

	_but[5]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[5]->Bind(EVENT_RELEASED, &but_play5);

	_but[6]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[6]->Bind(EVENT_RELEASED, &but_play6);

	_but[7]->AddArgs(EVENT_RELEASED, 0, (void *)(&_atype));
	_but[7]->AddArgs(EVENT_RELEASED, 1, (void *)(&_is_run));
	_but[7]->Bind(EVENT_RELEASED, &but_close);

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
	_is_run = 1;
	_volume = 1.0;

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	for (uint8_t i = 0; i < 8; i++)
	{
		GUI_AddObject(_but[i]);
		_but[i]->init();
	}

	sourceSD = new AudioFileSourceSD();
	//sourceSPIFFS = new AudioFileSourceSPIFFS();

	out = new AudioOutputI2S();
	out->SetPinout(12, 0, 2);
	out->SetOutputModeMono(true);
	out->SetGain(_volume * 0.05);

	rtttl = new AudioGeneratorRTTTL();
	mp3 = new AudioGeneratorMP3();
	talkie = new AudioGeneratorTalkie();
	mod = new AudioGeneratorMOD();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Audio::run()
{
	//Frame_Base::run();

	M5.update();

	if (M5.BtnA.wasPressed())
	{
		_volume -= 1.0;
		if (_volume < 0)
			_volume = 10.0;
		out->SetGain(_volume * 0.05);
		Serial.println(_volume);
	}
	else if (M5.BtnC.wasPressed())
	{
		_volume += 1.0;
		if (_volume > 10.0)
			_volume = 1.0;
		out->SetGain(_volume * 0.05);
		Serial.println(_volume);
	}

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
		case AT_STREAM:
			if (mp3->isRunning())
			{
				if (!mp3->loop())
					mp3->stop();
			}
			break;
		case AT_MOD:
			if (mod->isRunning())
			{
				if (!mod->loop())
					mod->stop();
			}
			break;
	}

	return 1;
}
