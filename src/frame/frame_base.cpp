/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Frame-Base-Class
*******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "frame_base.h"
#include "../gui/gui.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
const unsigned short *wifiLevel[4] =
{
	wifi1_22x22,
	wifi2_22x22,
	wifi3_22x22,
	wifi4_22x22
};

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Base::Frame_Base()
{
	_frame_id = 0;
	_frame_name = "Frame_Base";
	GUI_UpdateGlobalLastActiveTime();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
Frame_Base::~Frame_Base()
{
	if (_key_exit != NULL)
		delete _key_exit;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exitbtn(void)
{
	_key_exit = new GUI_ImgButton(0, 24, 24, 25, CloseWindowUp, CloseWindowDown);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::bevel(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t type)
{
	switch (type)
	{
		case BT_NORMAL:
			//M5.Lcd.fillRect(x, y, w, h, M5.Lcd.color565(149, 149, 149));
			M5.Lcd.drawLine(x, y, x, y + h - 1, WHITE);
			M5.Lcd.drawLine(x, y, x + w - 1, y, WHITE);
			M5.Lcd.drawLine(x, y + h - 1, x + w - 1, y + h - 1, BLACK);
			M5.Lcd.drawLine(x + w - 1, y, x + w - 1, y + h - 1, BLACK);
			break;

		case BT_RECESSED:
			//M5.Lcd.fillRect(x, y, w, h, M5.Lcd.color565(149, 149, 149));
			M5.Lcd.drawLine(x, y, x, y + h - 1, BLACK);
			M5.Lcd.drawLine(x, y, x + w - 1, y, BLACK);
			M5.Lcd.drawLine(x, y + h - 1, x + w - 1, y + h - 1, WHITE);
			M5.Lcd.drawLine(x + w - 1, y, x + w - 1, y + h - 1, WHITE);
			break;
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::openWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, String title, boolean active)
{
	_winX = x;
	_winY = y;
	_winW = w;
	_winH = h;
	_title = title;

	//M5.Lcd.drawBitmap(_winX, _winY, _winW, _winH, Window);

	// Window-Fill
	if (active)
		M5.Lcd.fillRect(_winX, _winY, _winW, _winH, MWB_BLUE);
	else
		M5.Lcd.fillRect(_winX, _winY, _winW, _winH, MWB_GRAY);

	// Window-Bevel
	M5.Lcd.drawRect(_winX, _winY, _winW, _winH, MWB_BLACK);
	M5.Lcd.drawLine(_winX, _winY, _winX + _winW, _winY, MWB_WHITE);
	M5.Lcd.drawLine(_winX, _winY, _winX, _winY + _winH, MWB_WHITE);

	// Window-Inner-Fill
	M5.Lcd.fillRect(_winX + 4, _winY + 24, _winW - (2 * 4), _winH - 24 - 2, MWB_GRAY);
	M5.Lcd.drawRect(_winX + 4, _winY + 24, _winW - (2 * 4), _winH - 24 - 2, MWB_WHITE);
	M5.Lcd.drawLine(_winX + 4, _winY + 24, _winW - (2 * 4), _winY + 24, MWB_BLACK);
	M5.Lcd.drawLine(_winX + 4, _winY + 24, _winX + 4, _winY - 24 - 2, MWB_BLACK);

	// Depth-Gadget
	M5.Lcd.drawRect(_winX + _winW - 24, _winY, 24, 24 + 1, MWB_BLACK);
	M5.Lcd.drawLine(_winX + _winW - 24, _winY, _winX + _winW, _winY, MWB_WHITE);
	M5.Lcd.drawLine(_winX + _winW - 24, _winY, _winX + _winW - 24, _winY + 24 - 1, MWB_WHITE);
	M5.Lcd.drawLine(_winX + _winW - 24 - 1, _winY + 1, _winX + _winW - 24 - 1, _winY + 24 - 1, MWB_BLACK);

	// Gadget-Content
	M5.Lcd.fillRect((_winX + _winW - 24) + 5, _winY + 5, 12, 10, MWB_GRAY);
	M5.Lcd.drawRect((_winX + _winW - 24) + 5, _winY + 5, 12, 10, MWB_BLACK);
	M5.Lcd.fillRect((_winX + _winW - 24) + 8, _winY + 8, 12, 10, MWB_WHITE);
	M5.Lcd.drawRect((_winX + _winW - 24) + 8, _winY + 8, 12, 10, MWB_BLACK);

	M5.Lcd.setCursor(_winX + 24 + 8, _winY + 17);
	M5.Lcd.println(title);

	exitbtn();
	_key_exit->AddArgs(GUI_Button::EVENT_RELEASED, 0, (void *)(&_is_run));
	_key_exit->Bind(GUI_Button::EVENT_RELEASED, &exit_cb);

	GUI_AddObject(_key_exit);
	_key_exit->init();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
int Frame_Base::run(void)
{
	StatusBar();
	return _is_run;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exit(void)
{
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::StatusBar()
{
	if (globalSettings->isWifiConnected)
	{
		uint8_t level = 0;
		int8_t rssi = WiFi.RSSI();

		if (rssi > -55)
			level = 3;
		else if (rssi > -88)
			level = 2;
		else
			level = 1;

		M5.Lcd.drawBitmap(272, 0, 22, 22, wifiLevel[level]);
	}

	if (millis() - _time > 10000)
	{
		_time = millis();

		if (globalSettings->isWifiConnected)
		{
			globalSettings->isWifiConnected = false;
			WiFi.disconnect(true);
			WiFi.mode(WIFI_OFF);

			M5.Lcd.drawBitmap(272, 0, 22, 22, wifiLevel[0]);
		}

		getLocalTime(&timeinfo);

		if (timeinfo.tm_min != globalSettings->mins)
		{
			globalSettings->hour = timeinfo.tm_hour;
			globalSettings->mins = timeinfo.tm_min;

			M5.Lcd.fillRect(220, 0, 50, 22, WHITE);
			sprintf(buffer, "%02d:%02d", globalSettings->hour, globalSettings->mins);
			M5.Lcd.setCursor(220, 16);
			M5.Lcd.print(buffer);
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::init_StatusBar()
{
	M5.Lcd.clear(M5.Lcd.color565(149, 149, 149));

	// Title-Bar
	M5.Lcd.fillRect(0, 0, 320, 24, WHITE);

	// Title-Bar-Bevel
	M5.Lcd.drawLine(0, 23, 320, 23, BLACK);
	M5.Lcd.drawLine(319, 0, 319, 23, BLACK);

	// Gadget-Fill
	M5.Lcd.fillRect(320 - 23, 0, 23, 23, M5.Lcd.color565(149, 149, 149));

	// Gadget-Content
	M5.Lcd.drawRect((320 - 23) + 5, 5, 12, 10, BLACK);
	M5.Lcd.drawRect((320 - 23) + 8, 8, 12, 10, BLACK);
	M5.Lcd.fillRect((320 - 23) + 9, 9, 10, 8, WHITE);

	// Title-Bar-Text
	M5.Lcd.setFreeFont(&FreeSans9pt7b);
	M5.Lcd.setTextSize(1);
	M5.Lcd.setTextColor(BLACK);
	M5.Lcd.setCursor(8, 15);
	M5.Lcd.println("Amiga Workbench");

	if (globalSettings->isWifiConnected)
	{
		uint8_t level = 0;
		int8_t rssi = WiFi.RSSI();

		if (rssi > -55)
			level = 3;
		else if (rssi > -88)
			level = 2;
		else
			level = 1;

		M5.Lcd.drawBitmap(272, 0, 22, 22, wifiLevel[level]);
	}
	else
		M5.Lcd.drawBitmap(272, 0, 22, 22, wifiLevel[0]);

	_time = millis();
	getLocalTime(&timeinfo);

	globalSettings->hour = timeinfo.tm_hour;
	globalSettings->mins = timeinfo.tm_min;

	M5.Lcd.fillRect(220, 0, 50, 22, WHITE);
	sprintf(buffer, "%02d:%02d", globalSettings->hour, globalSettings->mins);
	M5.Lcd.setCursor(220, 16);
	M5.Lcd.print(buffer);
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void Frame_Base::exit_cb(gui_args_vector_t &args)
{
	GUI_PopFrame();
	*((int *)(args[0])) = 0;
}
