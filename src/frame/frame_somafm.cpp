/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: SomaFM-Player
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include <M5Core2.h>
#include <AudioFileSourceICYStream.h>
#include <AudioFileSourceBuffer.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

#include "main.h"
#include "frame_somafm.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
LinkedList<somaStation *> stationList = LinkedList<somaStation *>();

AudioFileSourceICYStream *fsfm_stream;
AudioFileSourceBuffer *fsfm_streamBuf;
AudioGeneratorMP3 *fsfm_mp3;
AudioOutputI2S *fsfm_out;

char fsfm_URL[64];
char streamTitle[128];
boolean metaChange;

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void fsfm_MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
	char s2[128];
	strncpy_P(s2, string, sizeof(s2));
	s2[sizeof(s2)-1]=0;
	sprintf(streamTitle, "%s", s2);
	metaChange = true;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void fsfm_play(gui_args_vector_t &args)
{
	somaStation *_ss;

	GUI_Button *_but = (GUI_Button *)(args[2]);

	if (*((boolean *)(args[0])))
	{
		fsfm_mp3->stop();
		delete fsfm_streamBuf;
		delete fsfm_stream;

		_but->setLabel("Play");
		*((boolean *)(args[0])) = false;
	}
	else
	{
		GUI_Cycle *_cy = (GUI_Cycle *)(args[1]);

		if (stationList.size() > 0)
		{
			_ss = stationList.get(_cy->getSelectedItem());
			strlcpy(fsfm_URL, _ss->url, sizeof(fsfm_URL));
		}

		fsfm_stream = new AudioFileSourceICYStream(fsfm_URL);
		fsfm_streamBuf = new AudioFileSourceBuffer(fsfm_stream, 4096);
		fsfm_stream->RegisterMetadataCB(fsfm_MDCallback, (void*)"ICY");
		fsfm_mp3->begin(fsfm_streamBuf, fsfm_out);

		_but->setLabel("Stop");
		*((boolean *)(args[0])) = true;
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_SomaFM::Frame_SomaFM(void)
{
	_frame_name = "Frame_SomaFM";
	_isPlaying = false;
	metaChange = false;

	_string[0] = new GUI_String("\0", "\0", 4 + 8, (2 * 24) + 56 + (3 * 8) + (1 * 32), 320 - 8 - 16, 32);
	_cycle = new GUI_Cycle(4 + 8, (2 * 24) + 8 + 56 + 8, 320 - 8 - 16, 32);
	_but = new GUI_Button("Play", 4 + 8, (2 * 24) + 56 + (4 * 8) + (2 * 32), 144, 32);
	_string[1] = new GUI_String("Volume: ", 10, 4 + 8 + 144 + 8 + 100, (2 * 24) + 56 + (4 * 8) + (2 * 32), 44, 32);

	exitbtn();
	_key_exit->AddArgs(EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(EVENT_RELEASED, &exit_cb);

	_but->AddArgs(EVENT_RELEASED, 0, (void *)(&_isPlaying));
	_but->AddArgs(EVENT_RELEASED, 1, _cycle);
	_but->AddArgs(EVENT_RELEASED, 2, _but);
	_but->Bind(EVENT_RELEASED, &fsfm_play);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_SomaFM::~Frame_SomaFM(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_SomaFM::init(gui_args_vector_t &args)
{
	somaStation *_ss;
	
	_is_run = 1;
	_volume = 10.0;

	loadStations();

	// Window
	openWindow(0, 24, 320, 216, _frame_name);

	// Logo
	bevel(4 + 8, (2 * 24) + 8, 320 - 8 - 16, 56, BT_RECESSED);
	M5.Lcd.drawJpgFile(SPIFFS, "/SomaFM.jpg", 4 + 8 + 1, (2 * 24) + 8 + 1, 294, 54);

	// Cycle
	GUI_AddObject(_cycle);
	_cycle->init();

	_cycle->clearItemList();

	if (stationList.size() > 0)
	{
		for (uint8_t i = 0; i < stationList.size(); i++)
		{
			_ss = stationList.get(i);
			String s = String(_ss->name);
			_cycle->addItem(s);
		}
	}

	_cycle->setSelectedItem(0);
	_cycle->Draw();

	// String
	GUI_AddObject(_but);
	_but->init();

	// Buttons
	for (uint8_t i = 0; i < 2; i++)
	{
		GUI_AddObject(_string[i]);
		_string[i]->init();
	}

	fsfm_out = new AudioOutputI2S();
	fsfm_out->SetPinout(12, 0, 2);
	fsfm_out->SetOutputModeMono(true);
	fsfm_out->SetGain(_volume * 0.05);

	fsfm_mp3 = new AudioGeneratorMP3();

	return 3;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_SomaFM::run()
{
	//Frame_Base::run();

	M5.update();

	if (M5.BtnA.wasPressed())
	{
		_volume -= 1.0;

		if (_volume < 0)
			_volume = 10.0;

		fsfm_out->SetGain(_volume * 0.05);
		_string[1]->setValue((uint32_t)_volume);
	}
	else if (M5.BtnC.wasPressed())
	{
		_volume += 1.0;

		if (_volume > 10.0)
			_volume = 1.0;

		fsfm_out->SetGain(_volume * 0.05);
		_string[1]->setValue((uint32_t)_volume);
	}

	if (_isPlaying)
	{
		if (fsfm_mp3->isRunning())
		{
			if (!fsfm_mp3->loop())
				fsfm_mp3->stop();

			if (metaChange)
			{
				metaChange = false;
				_string[0]->setValue(streamTitle);
			}
		}
	}

	return 1;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_SomaFM::loadStations(void)
{
	String fileName = "/stations.json";
	DynamicJsonDocument doc(1024);
	somaStation *_ss;

	if (SPIFFS.exists(fileName))
	{
		File dataFile = SPIFFS.open(fileName, "r");

		DeserializationError error = deserializeJson(doc, dataFile);

		if (error)
		{
			Serial.println("Failed to read file!");
			return;
		}

		dataFile.close();

		// clear StationList
		if (stationList.size() > 0)
		{
			while (stationList.size() > 0)
			{
				_ss = stationList.get(0);
				stationList.remove(0);
				delete _ss;
			}
		}

		for (uint8_t i = 0; i < doc.size(); i++)
		{
			_ss = new somaStation();
			strlcpy(_ss->name, doc[i]["name"], sizeof(_ss->name));
			strlcpy(_ss->url, doc[i]["url"], sizeof(_ss->url));
			stationList.add(_ss);
		}
	}
}
