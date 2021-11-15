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
void Frame_Base::openWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h, const String &title)
{

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
	M5.Lcd.drawRect((320 - 23) + 4, 4, 12, 10, BLACK);
	M5.Lcd.drawRect((320 - 23) + 7, 7, 12, 10, BLACK);
	M5.Lcd.fillRect((320 - 23) + 8, 8, 10, 8, WHITE);

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
