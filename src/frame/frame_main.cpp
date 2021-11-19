/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame: Main
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "main.h"
#include "../gui/gui.h"

#include "M5Wifi.h"
#include "frame_main.h"
#include "frame_settings.h"
#include "frame_gfxdemo.h"
#include "frame_sysinfo.h"
#include "frame_wifiman.h"
#include "frame_windemo.h"

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_app0_cb(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_Settings");

	if (frame == NULL)
	{
		frame = new Frame_Settings();
		GUI_AddFrame("Frame_Settings", frame);
	}
	GUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_app1_cb(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_GfxDemo");

	if (frame == NULL)
	{
		frame = new Frame_GfxDemo();
		GUI_AddFrame("Frame_GfxDemo", frame);
	}
	GUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_app2_cb(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_SysInfo");

	if (frame == NULL)
	{
		frame = new Frame_SysInfo();
		GUI_AddFrame("Frame_SysInfo", frame);
	}
	GUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_app3_cb(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_WinDemo");

	if (frame == NULL)
	{
		frame = new Frame_WinDemo();
		GUI_AddFrame("Frame_WinDemo", frame);
	}
	GUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void key_app4_cb(gui_args_vector_t &args)
{
	Frame_Base *frame = GUI_GetFrame("Frame_WifiMan");

	if (frame == NULL)
	{
		frame = new Frame_WifiMan();
		GUI_AddFrame("Frame_WifiMan", frame);
	}
	GUI_PushFrame(frame);
	*((int*)(args[0])) = 0;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Main::Frame_Main(void): Frame_Base()
{
	_frame_name = "Frame_Main";
	_frame_id = 1;

	for (uint8_t y = 0; y < (MAX_APPS / 4); y++)
	{
		for (uint8_t x = 0; x < 4; x++)
		{
			switch ((y * 4) + x)
			{
				case 0:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Font_U, Icon_Font_D);
					break;
				case 1:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_IControl_U, Icon_IControl_D);
					break;
				case 2:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Input_U, Icon_Input_D);
					break;
				case 3:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Locale_U, Icon_Locale_D);
					break;
				case 4:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Overscan_U, Icon_Overscan_D);
					break;
				case 5:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Palette_U, Icon_Palette_D);
					break;
				case 6:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Pointer_U, Icon_Pointer_D);
					break;
				case 7:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_ScreenMode_U, Icon_ScreenMode_D);
					break;
				case 8:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Serial_U, Icon_Serial_D);
					break;
				case 9:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Sound_U, Icon_Sound_D);
					break;
				case 10:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_Time_U, Icon_Time_D);
					break;
				case 11:
					_key[(y * 4) + x] = new GUI_ImgButton((x * (KEY_W + 8)) + 8, (y * (KEY_H + 24)) + 8 + 24, KEY_W, KEY_H, Icon_WBPattern_U, Icon_WBPattern_D);
					break;
			}
		}
	}

	// Button-Functions
	_key[0]->AddArgs(GUI_ImgButton::EVENT_RELEASED, 0, (void*)(&_is_run));
	_key[0]->Bind(GUI_ImgButton::EVENT_RELEASED, key_app0_cb);

	_key[1]->AddArgs(GUI_ImgButton::EVENT_RELEASED, 0, (void*)(&_is_run));
	_key[1]->Bind(GUI_ImgButton::EVENT_RELEASED, key_app1_cb);

	_key[2]->AddArgs(GUI_ImgButton::EVENT_RELEASED, 0, (void*)(&_is_run));
	_key[2]->Bind(GUI_ImgButton::EVENT_RELEASED, key_app2_cb);

	_key[3]->AddArgs(GUI_ImgButton::EVENT_RELEASED, 0, (void*)(&_is_run));
	_key[3]->Bind(GUI_ImgButton::EVENT_RELEASED, key_app3_cb);

	_key[4]->AddArgs(GUI_ImgButton::EVENT_RELEASED, 0, (void*)(&_is_run));
	_key[4]->Bind(GUI_ImgButton::EVENT_RELEASED, key_app4_cb);

	_time = millis();

	if (globalSettings->isNTPTime)
	{
		M5.Lcd.setFreeFont(&FreeSans9pt7b);
		sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
		M5.Lcd.setCursor(220, 16);
		M5.Lcd.print(buffer);
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Main::~Frame_Main()
{
	for (uint8_t i = 0; i < MAX_APPS; i++)
		delete _key[i];
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Main::AppName()
{
	for (uint8_t y = 0; y < (MAX_APPS / 4); y++)
	{
		for (uint8_t x = 0; x < 4; x++)
		{
			switch ((y * 4) + x)
			{
				case 0:
					sprintf(buffer, "Font");
					break;
				case 1:
					sprintf(buffer, "IControl");
					break;
				case 2:
					sprintf(buffer, "Input");
					break;
				case 3:
					sprintf(buffer, "Locale");
					break;
				case 4:
					sprintf(buffer, "Overscan");
					break;
				case 5:
					sprintf(buffer, "Palette");
					break;
				case 6:
					sprintf(buffer, "Pointer");
					break;
				case 7:
					sprintf(buffer, "ScrMode");
					break;
				case 8:
					sprintf(buffer, "Serial");
					break;
				case 9:
					sprintf(buffer, "Sound");
					break;
				case 10:
					sprintf(buffer, "Time");
					break;
				case 11:
					sprintf(buffer, "Pattern");
					break;
			}
			uint16_t xtext = (x * (KEY_W + 8)) + 8 + ((KEY_W / 2) - (M5.Lcd.textWidth(buffer) / 2));
			uint16_t ytext = (y * (KEY_H + 24)) + 8 + 24 + KEY_H + 14;

			M5.Lcd.setCursor(xtext, ytext);
			M5.Lcd.print(buffer);
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Main::init(gui_args_vector_t &args)
{
	_is_run = 1;

	M5.Lcd.setFreeFont(&FreeSans9pt7b);
	Frame_Base::init_StatusBar();

	for (uint8_t i = 0; i < MAX_APPS; i++)
	{
		GUI_AddObject(_key[i]);
		_key[i]->init();
	}

	_time = millis();

	if (globalSettings->isNTPTime)
	{
		sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
		M5.Lcd.setCursor(220, 16);
		M5.Lcd.print(buffer);
	}

	AppName();
	return 9;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Main::run()
{
	Frame_Base::run();
	return 1;
}
