/******************************************************************************
* M5Core2_Amiga
* (C)2021 M.Volkel
*
* Settings-Functions
*******************************************************************************/

// Comment templates

/******************************************************************************
*
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/

/******************************************************************************/

/******************************************************************************
* Header-Files
*******************************************************************************/
#include "M5Settings.h"

/******************************************************************************
* Global Variables
*******************************************************************************/
struct Settings *globalSettings;
LinkedList<wifiNetwork *> WifiNetworkList = LinkedList<wifiNetwork *>();

/******************************************************************************
* Functions
*******************************************************************************/

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_initSettings(void)
{
	globalSettings = (struct Settings *)calloc(1, sizeof(struct Settings));
	globalSettings->wifiConnected = false;
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_createSettings(void)
{
	wifiNetwork *_wn;

	for (uint8_t i = 0; i < 2; i++)
	{
		_wn = new wifiNetwork();
		sprintf(_wn->ssid, "NSA-Surveillance");
		sprintf(_wn->pwd, "laganas.2017");
		WifiNetworkList.add(_wn);
	}

	_wn = WifiNetworkList.get(0);
	sprintf(_wn->ssid, "%d", WifiNetworkList.size());
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_saveSettings(void)
{
	String fileName = "/config.txt";
	DynamicJsonDocument doc(1024);
	wifiNetwork *_wn;

    File dataFile = SPIFFS.open(fileName, "w");

	// create JSON-Document
	for (uint8_t i = 0; i < WifiNetworkList.size(); i++)
	{
		_wn = WifiNetworkList.get(i);
		doc[i]["ssid"] = _wn->ssid;
		doc[i]["pwd"] = _wn->pwd;
	}

	// serialize JSON
	if (serializeJson(doc, dataFile) == 0)
		Serial.println(F("Failed to write to file"));

	dataFile.close();
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_loadSettings(void)
{
	String fileName = "/config.txt";
	DynamicJsonDocument doc(1024);
	wifiNetwork *_wn;

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

		uint8_t _size = atoi(doc[0]["ssid"]);
		m5set_clearSettings();

		_wn = new wifiNetwork();
		sprintf(_wn->ssid, "%d", _size);
		WifiNetworkList.add(_wn);

		//for (uint8_t i = 1; i < _size; i++)
		//{
		//}

		_wn = new wifiNetwork();
		strlcpy(_wn->ssid, doc[1]["ssid"], sizeof(_wn->ssid));
		strlcpy(_wn->pwd, doc[1]["pwd"], sizeof(_wn->pwd));
		WifiNetworkList.add(_wn);

		_wn = WifiNetworkList.get(1);
		strlcpy(globalSettings->wifiSsid, _wn->ssid, sizeof(globalSettings->wifiSsid));
		strlcpy(globalSettings->wifiPwd, _wn->pwd, sizeof(globalSettings->wifiPwd));

		//m5set_printWifiList();
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_clearSettings(void)
{
	wifiNetwork *_wn;

	if (WifiNetworkList.size() > 0)
	{
		while (WifiNetworkList.size() > 0)
		{
			_wn = WifiNetworkList.get(0);
			WifiNetworkList.remove(0);
			delete _wn;
		}
	}
}

/*------------------------------------------------------------------------------
-
------------------------------------------------------------------------------*/
void m5set_printWifiList(void)
{
	wifiNetwork *_wn;

	for (uint8_t i = 0; i < WifiNetworkList.size(); i++)
	{
		_wn = WifiNetworkList.get(i);
		Serial.print("Network ");
		Serial.print(i);
		Serial.print(": ");
		Serial.print("SSID: ");
		Serial.print(_wn->ssid);
		Serial.print(", PWD: ");
		Serial.println(_wn->pwd);
	}
}
